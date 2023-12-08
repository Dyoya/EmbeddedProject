#include "common.h"

// 변수 초기화
int share_var = 0;

// 뮤텍스 초기화
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;