#include "iccelog.h"
#include <stdio.h>

const char *LogName="log.txt";
/** 日志相关 **/
void log4c(char *fmt,...)
{
	FILE *fp;
	va_list ap;
	va_start(ap,fmt);
	
	fp=fopen(LogName,"a+");
	int res=vfprintf(fp,fmt,ap);
	
	fclose(fp);
	va_end(ap);
//	return res;
}
