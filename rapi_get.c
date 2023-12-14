#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>

#include "wiringPi.h"
#include "wiringSerial.h"
// #include "pn532_rpi.h"
// #include "pn532.h"

#define _RESET_PIN (20)
#define _REQ_PIN (16)

typedef struct _PN532
{
    int (*reset)(void);
    int (*read_data)(uint8_t *data, uint16_t count);
    int (*write_data)(uint8_t *data, uint16_t count);
    bool (*wait_ready)(uint32_t timeout);
    int (*wakeup)(void);
    void (*log)(const char *log);
} PN532;

const uint8_t PN532_ACK[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};

int PN532_WriteFrame(PN532 *pn532, uint8_t *data, uint16_t length)
{
    if (length > 255 || length < 1)
    {
        return -1; // Data must be array of 1 to 255 bytes.
    }

    uint8_t frame[255 + 7];
    uint8_t checksum = 0;
    frame[0] = 0x00;
    frame[1] = 0x00;
    frame[2] = 0xFF;
    for (uint8_t i = 0; i < 3; i++)
    {
        checksum += frame[i];
    }
    frame[3] = length & 0xFF;
    frame[4] = (~length + 1) & 0xFF;
    for (uint8_t i = 0; i < length; i++)
    {
        frame[5 + i] = data[i];
        checksum += data[i];
    }
    frame[length + 5] = ~checksum & 0xFF;
    frame[length + 6] = 0x00;
    if (pn532->write_data(frame, length + 7) != 0)
    {
        return -1;
    }
    return 0;
}

int PN532_ReadFrame(PN532 *pn532, uint8_t *response, uint16_t length)
{
    uint8_t buff[255 + 7];
    uint8_t checksum = 0;
    // Read frame with expected length of data.
    pn532->read_data(buff, length + 7);
    // Swallow all the 0x00 values that preceed 0xFF.
    uint8_t offset = 0;
    while (buff[offset] == 0x00)
    {
        offset += 1;
        if (offset >= length + 8)
        {
            pn532->log("Response frame preamble does not contain 0x00FF!");
            return -1;
        }
    }
    if (buff[offset] != 0xFF)
    {
        pn532->log("Response frame preamble does not contain 0x00FF!");
        return -1;
    }
    offset += 1;
    if (offset >= length + 8)
    {
        pn532->log("Response contains no data!");
        return -1;
    }
    // Check length & length checksum match.
    uint8_t frame_len = buff[offset];
    if (((frame_len + buff[offset + 1]) & 0xFF) != 0)
    {
        pn532->log("Response length checksum did not match length!");
        return -1;
    }
    // Check frame checksum value matches bytes.
    for (uint8_t i = 0; i < frame_len + 1; i++)
    {
        checksum += buff[offset + 2 + i];
    }
    checksum &= 0xFF;
    if (checksum != 0)
    {
        pn532->log("Response checksum did not match expected checksum");
        return -1;
    }
    // Return frame data.
    for (uint8_t i = 0; i < frame_len; i++)
    {
        response[i] = buff[offset + 2 + i];
    }
    return frame_len;
}

int PN532_CallFunction(
    PN532 *pn532,
    uint8_t command,
    uint8_t *response,
    uint16_t response_length,
    uint8_t *params,
    uint16_t params_length,
    uint32_t timeout)
{
    // Build frame data with command and parameters.
    uint8_t buff[255];
    buff[0] = 0xD4;
    buff[1] = command & 0xFF;
    for (uint8_t i = 0; i < params_length; i++)
    {
        buff[2 + i] = params[i];
    }
    // Send frame and wait for response.
    if (PN532_WriteFrame(pn532, buff, params_length + 2) != 0)
    {
        pn532->wakeup();
        pn532->log("Trying to wakeup");
        return -1;
    }
    if (!pn532->wait_ready(timeout))
    {
        return -1;
    }
    // Verify ACK response and wait to be ready for function response.
    pn532->read_data(buff, sizeof(PN532_ACK));
    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++)
    {
        if (PN532_ACK[i] != buff[i])
        {

            pn532->log("Did not receive expected ACK from PN532!");
            return -1;
        }
    }
    if (!pn532->wait_ready(timeout))
    {
        return -1;
    }
    // Read response bytes.
    int frame_len = PN532_ReadFrame(pn532, buff, response_length + 2);

    // Check that response is for the called function.
    if (!((buff[0] == 0xD5) && (buff[1] == (command + 1))))
    {
        pn532->log("Received unexpected command response!");
        return -1;
    }
    // Return response data.
    for (uint8_t i = 0; i < response_length; i++)
    {
        response[i] = buff[i + 2];
    }
    // The the number of bytes read
    return frame_len - 2;
}

int PN532_ReadPassiveTarget(
    PN532 *pn532,
    uint8_t *response,
    uint8_t card_baud,
    uint32_t timeout)
{
    // Send passive read command for 1 card.  Expect at most a 7 byte UUID.
    uint8_t params[] = {0x01, card_baud};
    uint8_t buff[19];
    int length = PN532_CallFunction(pn532, 0x4A,
                                    buff, sizeof(buff), params, sizeof(params), timeout);
    if (length < 0)
    {
        return -1; // No card found
    }
    // Check only 1 card with up to a 7 byte UID is present.
    if (buff[0] != 0x01)
    {
        pn532->log("More than one card detected!");
        return -1;
    }
    if (buff[5] > 7)
    {
        pn532->log("Found card with unexpectedly long UID!");
        return -1;
    }
    for (uint8_t i = 0; i < buff[5]; i++)
    {
        response[i] = buff[6 + i];
    }
    return buff[5];
}

static int fd = 0;

/**************************************************************************
 * Reset and Log implements
 **************************************************************************/
int PN532_Reset(void)
{
    digitalWrite(_RESET_PIN, HIGH);
    delay(100);
    digitalWrite(_RESET_PIN, LOW);
    delay(500);
    digitalWrite(_RESET_PIN, HIGH);
    delay(100);
    return 0;
}

void PN532_Log(const char *log)
{
    printf(log);
    printf("\r\n");
}

int PN532_UART_ReadData(uint8_t *data, uint16_t count)
{
    int index = 0;
    int length = count; // length of frame (data[3]) might be shorter than the count
    while (index < 4)
    {
        if (serialDataAvail(fd))
        {
            data[index] = serialGetchar(fd);
            index++;
        }
        else
        {
            delay(5);
        }
    }
    if (data[3] != 0)
    {
        length = data[3] + 7;
    }
    while (index < length)
    {
        if (serialDataAvail(fd))
        {
            data[index] = serialGetchar(fd);
            if (index == 3 && data[index] != 0)
            {
                length = data[index] + 7;
            }
            index++;
        }
        else
        {
            delay(5);
        }
    }
    return 0;
}

int PN532_UART_WriteData(uint8_t *data, uint16_t count)
{
    // clear FIFO queue of UART
    while (serialDataAvail(fd))
    {
        serialGetchar(fd);
    }
    write(fd, data, count);
    return 0;
}

bool PN532_UART_WaitReady(uint32_t timeout)
{
    struct timespec timenow;
    struct timespec timestart;
    clock_gettime(CLOCK_MONOTONIC, &timestart);
    while (1)
    {
        if (serialDataAvail(fd) > 0)
        {
            return true;
        }
        else
        {
            delay(50);
        }
        clock_gettime(CLOCK_MONOTONIC, &timenow);
        if ((timenow.tv_sec - timestart.tv_sec) * 1000 +
                (timenow.tv_nsec - timestart.tv_nsec) / 1000000 >
            timeout)
        {
            break;
        }
    }
    // Time out!
    return false;
}

int PN532_UART_Wakeup(void)
{
    // Send any special commands/data to wake up PN532
    uint8_t data[] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
    write(fd, data, sizeof(data));
    delay(50);
    return 0;
}

void PN532_UART_Init(PN532 *pn532)
{
    // init the pn532 functions
    pn532->reset = PN532_Reset;
    pn532->read_data = PN532_UART_ReadData;
    pn532->write_data = PN532_UART_WriteData;
    pn532->wait_ready = PN532_UART_WaitReady;
    pn532->wakeup = PN532_UART_Wakeup;
    pn532->log = PN532_Log;
    // UART setup
    fd = serialOpen("/dev/ttyS0", 115200);
    if (fd < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return;
    }
    if (wiringPiSetupGpio() < 0)
    { // using Broadcom GPIO pin mapping
        return;
    }
    pinMode(_RESET_PIN, OUTPUT);
    // hardware reset
    pn532->reset();
    // hardware wakeup
    pn532->wakeup();
}

char *NFC_Reader()
{
    // uint8_t uid[10];
    // int32_t uid_len = 0;
    printf("Hello!\r\n");
    PN532 pn532;
    PN532_UART_Init(&pn532);
    printf("Hello1!\r\n");
    printf("Waiting for RFID/NFC card...\r\n");
    uint8_t static uid[10];
    static char nfcID[16];
    if (PN532_ReadPassiveTarget(&pn532, uid, 0x00, 1000) == -1)
    {
        // 측정값 없음
        return "error";
    }
    else
    {
        sprintf(nfcID, "%x %x %x %x\n", uid[0], uid[1], uid[2], uid[3]);
        // 측정 값
    }

    return nfcID;
}
