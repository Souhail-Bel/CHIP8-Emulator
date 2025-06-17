#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG_MODE
	#define DEBUG_PRINT(fmt, ...) \
		do { \
			fprintf(stderr, "\033[30;106m ►\033[3mDEBUG \033[97;105m %s:%d:%s() \033[m " fmt "\n", \
			__FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		} while(0)
#else
	#define DEBUG_PRINT(fmt, ...) do {} while(0)
#endif

enum msgType{
	ERROR = 0,
	WARNING,
	INFORMATION
};

void writeMessage(enum msgType n, const char* format, ...);

#endif
