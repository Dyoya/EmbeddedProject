#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include "common.h"


void *gasSensorFun(void *arg)
{
    const int adc_channel = 3;
    int fd = i2c_dev;
    while(1){ 
        // share_var이 1가 될 때까지 대기
        while(share_var != 1) {}


        pthread_mutex_lock(&mutex); // 뮤텍스 잠금

        //printf("share_var : %d", share_var);
        share_var = 2;

        // ============== TODO : 측정 데이터 읽어오기 ============== //
        // ADC 설정
        // Analog input programming: 0b00 (singled ended)
        // A/D channel: 0b11
        wiringPiI2CWrite(fd, 0x40 | adc_channel);
        
        // 가스 센서 read
        // 주의: read 된 값은 단순 전압으로 농도가 아님
        gas_data = wiringPiI2CRead(fd);
        printf("gas : %d\n", 0);

        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
        sleep(2);

        delay(500);
    }

    return NULL;
}
