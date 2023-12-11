#ifndef __COMMON_H__
#define __COMMON_H__

#include <pthread.h>



extern int share_var;
extern pthread_mutex_t mutex;
extern unsigned int adc_slave_addr;
extern const char* i2c_dev;
extern int i2c_fd;
extern unsigned int gas_data;


extern void *temperatureSensorFun(void *); // 온습도 센서 쓰레드 함수
extern void *gasSensorFun(void *); // 가스 센서 쓰레드 함수
extern void *waterSensorFun(void *); // 수위 센서 쓰레드 함수
extern void *NFCReaderFun(void *); // NFC 리더기 쓰레드 함수
extern void *dataToServer(void *); // 서버로 데이터 전송하는 함수

#endif
