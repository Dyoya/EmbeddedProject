#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pthread.h>
#include <curl/curl.h>
#include <json-c/json.h>
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
    pthread_t send;
    pthread_t buz;

    // 초기화
    share_var = 0;

    if (wiringPiSetupGpio() < 0)
    {
        printf("wiringPiSetup() is failed\n");
        return -1;
    }
    if ((i2c_fd = wiringPiI2CSetupInterface(i2c_dev, adc_slave_addr)) < 0)
    {
        printf("wiringPi2CSetup Failed: \n");
        return -1;
    }

    // 테스트
    // share_var = 0;

    temperature = pthread_create(&temperature, NULL, temperatureSensorFun, NULL);
    gas = pthread_create(&gas, NULL, gasSensorFun, NULL);
    water = pthread_create(&water, NULL, waterSensorFun, NULL);
    nfc = pthread_create(&nfc, NULL, NFCReaderFun, NULL);
    send = pthread_create(&send, NULL, dataToServer, NULL);

    buz = pthread_create(&buz, NULL, waring, NULL);

    if (temperature < 0 || water < 0 || gas < 0 || nfc < 0)
    {
        perror("pthread_create() error\n");
        exit(0);
    }

    while (1)
        ;
}

void *dataToServer(void *arg)
{
    CURL *curl;
    CURLcode res;

    // CURL 초기화
    curl = curl_easy_init();

    while (1)
    {
        while (share_var != 4)
        {
            // share_var이 4가 될 때까지 대기
        }

        // JSON 객체 생성
        struct json_object *json_obj = json_object_new_object();
        json_object_object_add(json_obj, "temperature", json_object_new_double(temValue));
        json_object_object_add(json_obj, "gas", json_object_new_int(gasValue));
        json_object_object_add(json_obj, "water", json_object_new_int(waterValue));
        json_object_object_add(json_obj, "nfc", json_object_new_string(nfcId));

        // 문자열 변환
        const char *json_str = json_object_to_json_string_ext(json_obj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);

        pthread_mutex_lock(&mutex); // 뮤텍스 잠금

         // TODO : 측정 데이터 전송
        printf(json_str);
        printf("\n");
        char temp[200];
        char json[100];
        // sprintf(json, "\"temperature\":%f, ")
        sprintf(temp, "curl -d \'{\"temperature\":%f,\"water\":%d,\"gas\":%d,\"nfc\":\"%s\"}\' -H \"Content-Type: application/json\" -X POST https://port-0-embedded-system-1gksli2alppq2t18.sel4.cloudtype.app/sensor", temValue,waterValue,gasValue,nfcId);

        // printf(temp);
        // printf("\n%s",temp);

        system(temp);

        delay(100);

        share_var = 0;

        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
        sleep(2);
    }

    return NULL;
}