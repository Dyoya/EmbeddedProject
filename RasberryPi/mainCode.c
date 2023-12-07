#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
    share_var에 따라 센서 값 받아오거나 서버로 전송
    0 : 온습도 센서
    1 : 가스 센서
    2 : 수위 센서
    3 : NFC
    4 : 데이터 전송
*/
int share_var = 0;
pthread_mutex_t mutex; //뮤텍스 오브젝트

void *temperatureSensorFun(void *); // 온습도 센서 쓰레드 함수
void *gasSensorFun(void *); // 가스 센서 쓰레드 함수
void *waterSensorFun(void *); // 수위 센서 쓰레드 함수
void *NFCReaderFun(void *); // NFC 리더기 쓰레드 함수
void *dataToServer(void *); // 서버로 데이터 전송하는 함수

void main()
{

}

void *temperatureSensorFun(void *arg)
{

    while(share_var != 0)
    {
        // share_var이 0이 될 때까지 대기
    }

    pthread_mutex_lock(&mutex); // 뮤텍스 잠금

    printf("share_var : %d", share_var);
    share_var = 1;

    // TODO : 측정 데이터 읽어오기

    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    sleep(2);

    return NULL;
}

void *gasSensorFun(void *arg)
{

    while(share_var != 1)
    {
        // share_var이 1이 될 때까지 대기
    }

    pthread_mutex_lock(&mutex); // 뮤텍스 잠금

    printf("share_var : %d", share_var);
    share_var = 2;

    // TODO : 측정 데이터 읽어오기

    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    sleep(2);

    return NULL;
}

void *waterSensorFun(void *arg)
{

    while(share_var != 2)
    {
        // share_var이 2가 될 때까지 대기
    }

    pthread_mutex_lock(&mutex); // 뮤텍스 잠금

    printf("share_var : %d", share_var);
    share_var = 3;

    // TODO : 측정 데이터 읽어오기

    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    sleep(2);

    return NULL;
}

void *NFCReaderFun(void *arg)
{

    while(share_var != 3)
    {
        // share_var이 3이 될 때까지 대기
    }

    pthread_mutex_lock(&mutex); // 뮤텍스 잠금

    printf("share_var : %d", share_var);
    share_var = 4;

    // TODO : 측정 데이터 읽어오기

    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    sleep(2);

    return NULL;
}

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