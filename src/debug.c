#include <debug.h>

void writeMessage(enum msgType n, const char* msg){
	switch(n){
		case 0:
			printf("\033[30;101m ►\033[3mERROR:\033[0m %s\n", msg);
			break;
		
		case 1:
			printf("\033[30;103m ►\033[3mWARNING:\033[0m %s\n", msg);
			break;
		
		default:	
			printf("\033[30;104m ►\033[3mINFORMATION:\033[0m %s\n", msg);
	}
}