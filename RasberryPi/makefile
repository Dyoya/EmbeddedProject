# Makefile

# 컴파일러 설정
CC = gcc

# 컴파일 옵션 설정
CFLAGS = -Wall -pthread -lwiringPi -lcurl

# 빌드할 실행 파일 이름
TARGET = myprogram

# 소스 파일 목록
SRCS = main.c common.c water.c temperature.c gas.c nfc.c

# 헤더 파일 목록
HDRS = common.h

# 오브젝트 파일 목록
OBJS = $(SRCS:.c=.o)

# 빌드 규칙
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

%.o: %.c $(HDRS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
