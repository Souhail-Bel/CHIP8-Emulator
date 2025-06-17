#include <debug.h>
#include <stdarg.h>


void writeMessage(enum msgType n, const char* format, ...){
	va_list args;
	va_start(args, format);
	
	switch(n){
		case 0:
			printf("\033[30;101m ►\033[3mERROR:\033[0m ");
			break;
		
		case 1:
			printf("\033[30;103m ►\033[3mWARNING:\033[0m ");
			break;
		
		default:	
			printf("\033[30;104m ►\033[3mINFORMATION:\033[0m ");
	}
	
	vprintf(format, args);
	putchar('\n');
	va_end(args);
}