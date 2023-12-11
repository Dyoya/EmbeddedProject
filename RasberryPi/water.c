#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include "common.h"

#define SLAVE_ADDR_01 0x48 // 슬레이브 주소
static const char* I2C_DEV = "/dev/i2c-1"; // 장치 파일

void *waterSensorFun(void *arg)
{
    //int i2c_fd;  main에서 초기화하고 전역변수 처리함
    int preVal = 0; // 이전 값
    int curVal = 0; // 현재 값
    int adcChannel = 2; // 채널 번호

    // wiringPi 셋업 ===== main으로 옮김
    /*
    if(wiringPiSetupGpio() < 0 ){
        printf("wiringPiSetup() is failed\n");
        return NULL;
    }

    // i2C 연결
    if ((i2c_fd = wiringPiI2CSetupInterface (I2C_DEV, SLAVE_ADDR_01)) < 0 ){
        printf("wiringPi2CSetup Failed: \n"); 
        return NULL;
    }
    printf("I2C start....\n");
    */

    while(1){ 
        // share_var이 2가 될 때까지 대기
        while(share_var != 2) {}

        //Analog Input Programming: 00 (4 single ended inputs),
        //A/D Channel Number: 10 (AIN2 활성화)
        wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel); 

        preVal = wiringPiI2CRead(i2c_fd); // 이전 활성화 값
        curVal = wiringPiI2CRead(i2c_fd); // 현재 ADC 값 

        pthread_mutex_lock(&mutex); // 뮤텍스 잠금

        //printf("share_var : %d", share_var);
        share_var = 3;

        // ============== TODO : 측정 데이터 읽어오기 curVal ============== //
        printf("water : %d\n", curVal);

        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
        sleep(2);

        delay(500);
    }

    return NULL;
}
