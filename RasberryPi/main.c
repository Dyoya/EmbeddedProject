#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

/*
    share_var에 따라 센서 값 받아오거나 서버로 전송
    0 : 온습도 센서
    1 : 가스 센서
    2 : 수위 센서
    3 : NFC
    4 : 데이터 전송
*/

int main()
{
    pthread_t temperature;
    pthread_t water;
    pthread_t gas;
    pthread_t nfc;

    // 초기화
    share_var = 0;
    if(wiringPiSetupGpio() < 0 ){
        printf("wiringPiSetup() is failed\n");
        return NULL;
    }
    if ((i2c_fd = wiringPiI2CSetupInterface (i2c_dev, adc_slave_addr)) < 0 ){
        printf("wiringPi2CSetup Failed: \n"); 
        return NULL;
    }


    //테스트
    share_var = 0;

    temperature = pthread_create(&temperature, NULL, temperatureSensorFun, NULL);
    gas = pthread_create(&gas, NULL, gasSensorFun, NULL);
    water = pthread_create(&water, NULL, waterSensorFun, NULL);
    nfc = pthread_create(&nfc, NULL, NFCReaderFun, NULL);

    if(water < 0)
    {
        perror("pthread_create() error\n");
        exit(0);
    }

    while(1);
}

/*
void *dataToServer(void *arg)
{

    while(share_var != 4)
    {
        // share_var이 4가 될 때까지 대기
    }

    pthread_mutex_lock(&mutex); // 뮤텍스 잠금

    printf("share_var : %d", share_var);
    share_var = 0;

    // TODO : 측정 데이터 전송

    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    sleep(2);

    return NULL;
}
*/
