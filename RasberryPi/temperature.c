#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include "common.h"

void *temperatureSensorFun(void *arg)
{
    while(1){ 
        // share_var이 0가 될 때까지 대기
        while(share_var != 0) {}


        pthread_mutex_lock(&mutex); // 뮤텍스 잠금

        //printf("share_var : %d", share_var);
        share_var = 1;

        // ============== TODO : 측정 데이터 읽어오기 ============== //
        printf("temperature : %d\n", 0);

        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
        sleep(2);

        delay(500);
    }

    return NULL;
}