#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <pthread.h>
#include "common.h"

int fd = 0;

int PN532_UART_ReadData(uint8_t* data, uint16_t count) {
    int index = 0;
    int length = count;
    while (index < 4) {
        if (serialDataAvail(fd)) {
            printf("data[%d] : %02x\n",index,data[index]);
            data[index] = serialGetchar(fd);
            index++;
        } else {
            delay(5);
        }
    } 
    if (data[3] != 0) {
        length = data[3] + 7;
    }
    // printf("length : %d\n",length);
    while (index < length) {
        if (serialDataAvail(fd)) {
            data[index] = serialGetchar(fd);
            printf("data[%d] : %02x\n",index,data[index]);
            if (index == 3 && data[index] != 0) {
                length = data[index] + 7;
            }
            index++;
        } else {
            delay(5);
        }
    }
    return 0;
}
/*
void PN532_UART_Init() {
    if (wiringPiSetup () < 0) return ;
    if ((fd = serialOpen("/dev/ttyS0", 115200)) < 0) {
        printf ("Unable to open serial device.\n") ;
        return ;
    }

    uint8_t data[] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
    write(fd, data, sizeof(data));
    delay(50);
} 
*/

/*
=================================================================================
*/

int PN532_ReadFrame(uint8_t* response, uint16_t length) {
    uint8_t buff[255 + 7];
    uint8_t checksum = 0;
    uint8_t offset = 0;
    PN532_UART_ReadData(buff,length+7);
    while (buff[offset] == 0x00) {
        offset += 1;
    }
    offset += 1;
    uint8_t frame_len = buff[offset];
    for (uint8_t i = 0; i < frame_len + 1; i++) {
        checksum += buff[offset + 2 + i];
    }
    checksum &= 0xFF;
    for (uint8_t i = 0; i < frame_len; i++) {
        response[i] = buff[offset + 2 + i];
    }
    return frame_len;
}

int PN532_WriteFrame(uint8_t* data, uint16_t length) {
    uint8_t frame[255 + 7];
    uint8_t checksum = 0;
    frame[0] = 0x00;
    frame[1] = 0x00;
    frame[2] = 0xFF;
    for (uint8_t i = 0; i < 3; i++) {
        checksum += frame[i];
    }
    frame[3] = length & 0xFF;
    frame[4] = (~length + 1) & 0xFF;
    for (uint8_t i = 0; i < length; i++) {
        frame[5 + i] = data[i];
        checksum += data[i];
    }
    frame[length + 5] = ~checksum & 0xFF;
    frame[length + 6] = 0x00;
    while (serialDataAvail(fd)) {
        serialGetchar(fd);
    }
    write(fd, frame, length+7);
    return 0;
}

int PN532_CallFunction(
    uint8_t command,
    uint8_t* response,
    uint16_t response_length,
    uint8_t* params,
    uint16_t params_length
) {
    uint8_t buff[255];
    buff[0] = 0xD4;
    buff[1] = command & 0xFF;
    for (uint8_t i = 0; i < params_length; i++) {
        buff[2 + i] = params[i];
    }
    PN532_WriteFrame(buff, params_length + 2);

    PN532_UART_ReadData(buff,6);

    int frame_len = PN532_ReadFrame(buff, response_length + 2);

    for (uint8_t i = 0; i < response_length; i++) {
        response[i] = buff[i + 2];
    }
    return frame_len - 2;
}

int PN532_ReadPassiveTarget(uint8_t* response, uint8_t card_baud, uint32_t timeout) {
    uint8_t params[] = {0x01, card_baud};
    uint8_t buff[19];
    PN532_CallFunction(0x4A,buff, sizeof(buff), params, sizeof(params));
    for (uint8_t i = 0; i < buff[5]; i++) {
        response[i] = buff[6 + i];
    }
    return buff[5];
}

char * NFC_Reader(){
    uint8_t static uid[10];
    int32_t uid_len = 0;
    static char response[4];
    //PN532_UART_Init(); 
    while(PN532_ReadPassiveTarget(uid, 0x00, 1000)==-1);
    sprintf(response,"%x %x %x %x\n",uid[0],uid[1],uid[2],uid[3]);
    return response;
}

void *NFCReaderFun(void *arg)
{
    while(1){ 
        // share_var이 3이 될 때까지 대기
        while(share_var != 3) {}


        pthread_mutex_lock(&mutex); // 뮤텍스 잠금

        //printf("share_var : %d", share_var);
        share_var = 0; // 나중에 3으로 바꿔야함

        // ============== TODO : 측정 데이터 읽어오기 ============== //

        
        printf("nfc : %s\n", NFC_Reader());

        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
        sleep(2);

        delay(500);
    }

    return NULL;
}
