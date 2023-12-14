#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include "common.h"

#define MAX_TIME 100
#define DHT11PIN 17

void *temperatureSensorFun(void *arg)
{
    int dht11_temp[5] = {0, 0, 0, 0, 0};
    int dht11_result[5] = {0, 0, 0, 0, 0};

    while (1)
    {
        // share_var이 0가 될 때까지 대기
        while (share_var != 0)
        {
        }

        // 값 읽기
        int dht11_val[5] = {0, 0, 0, 0, 0};

        uint8_t lststate = HIGH;
        uint8_t counter = 0;
        uint8_t j = 0, i;

        for (i = 0; i < 5; i++)
        {
            dht11_val[i] = 0;

            pinMode(DHT11PIN, OUTPUT);
            digitalWrite(DHT11PIN, LOW);
            delay(18);
            digitalWrite(DHT11PIN, HIGH);
            delayMicroseconds(40);
            pinMode(DHT11PIN, INPUT);

            for (i = 0; i < MAX_TIME; i++)
            {
                counter = 0;
                while (digitalRead(DHT11PIN) == lststate)
                {
                    counter++;
                    delayMicroseconds(1);
                    //printf("%d")
                    if (counter == 255)
                        break;
                }
                lststate = digitalRead(DHT11PIN);
                if (counter == 255)
                    break;
                if ((i >= 4) && (i % 2 == 0))
                {
                    dht11_val[j / 8] <<= 1;

                    if (counter > 16)
                    {
                        dht11_val[j / 8] |= 1;
                    }
                    j++;
                }
            }
        }

        if (dht11_val[0] == 0)
        {
            //printf("if\n");
            //printf("Humidity = %d.%d %% Temperature = %d.%d *C\n", dht11_temp[0], dht11_temp[1], dht11_temp[2], dht11_temp[3]);
            for (i = 0; i < 5; i++)
                dht11_result[i] = dht11_temp[i];
        }
        else if ((j >= 40) && (dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3]) & 0xFF)))
        {
            //printf("else if\n");
            //printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n", dht11_val[0], dht11_val[1], dht11_val[2], dht11_val[3]);
            for (i = 0; i < 5; i++)
            {
                dht11_result[i] = dht11_val[i];
                dht11_temp[i] = dht11_val[i];
            }
        }
        else
        {
            //printf("else\n");
            //printf("Humidity = %d.%d %% Temperature = %d.%d *C\n", dht11_temp[0], dht11_temp[1], dht11_temp[2], dht11_temp[3]);
            for (i = 0; i < 5; i++)
            {
                dht11_result[i] = dht11_temp[i];
            }
        }

        pthread_mutex_lock(&mutex); // 뮤텍스 잠금

        printf("temperature : %d.%d\n", dht11_result[2], dht11_result[3]);

        char temp[16];
        sprintf(temp, "%d.%d", dht11_result[2], dht11_result[3]);

        char *endptr;
        float t;
        temValue = strtof(temp, &endptr);

        //temValue = (float)(round(t) / 10);

        delay(100);

        share_var = 1;

        pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
        sleep(2);

        delay(500);
    }

    return NULL;
}