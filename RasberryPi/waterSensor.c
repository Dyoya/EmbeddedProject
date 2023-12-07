#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#define SLAVE_ADDR_01 0x48 // 슬레이브 주소
static const char* I2C_DEV = "/dev/i2c-1"; // 장치 파일
int main(){ 
    int i2c_fd;  
    int cnt = 0;
    int preVal = 0; // 이전 값
    int curVal = 0; // 현재 값
    int adcChannel = 2; // 채널 번호

    if(wiringPiSetupGpio() < 0 ){
        printf("wiringPiSetup() is failed\n");
        return 1;
    }

    // i2C 연결 시도
    if ((i2c_fd = wiringPiI2CSetupInterface (I2C_DEV, SLAVE_ADDR_01)) < 0 ){
        printf("wiringPi2CSetup Failed: \n"); 
        return 1;
    }
    printf("I2C start....\n");

    while(1){ 
        //Analog Input Programming: 00 (4 single ended inputs),
        //A/D Channel Number: 10 (AIN2 활성화)
        wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel); 
        preVal= wiringPiI2CRead(i2c_fd); // 이전 활성화 값
        curVal = wiringPiI2CRead(i2c_fd); // 현재 ADC 값 
        printf("[%d] Previous value = %d, ", cnt, preVal); // 이전 값 출력
        printf("Current value= %d, ", curVal); // 현재 값 출력

        if(curVal > 10) printf("Water!\n");
        else printf("Hmm...\n");
        delay(500);
        cnt++;
    }
    return 0;
}
