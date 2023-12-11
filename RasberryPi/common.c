#include "common.h"

// 변수 초기화
int share_var = 0;
unsigned int adc_slave_addr = 0x48;
const char* i2c_dev = "/dev/i2c-1";

// 뮤텍스 초기화
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
