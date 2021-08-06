#ifndef ICCEWETHERCLIENT_H
#define ICCEWETHERCLIENT_H

#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>

#include "cJSON.h"
#include "iccelog.h"

/* 心知天气（www.seniverse.com）IP及端口 */
#define  WEATHER_IP_ADDR   "116.62.81.138"
#define  WEATHER_PORT	   80

/* 秘钥，注意！！如果要用这一份代码，这个一定要改为自己的，因为这个我已经故意改错了，防止有人与我公用一个KEY */
#define  KEY    "*********"		// 这是在心知天气注册后，每个用户自己的一个key

/* GET请求包 */
#define  GET_REQUEST_PACKAGE     \
         "GET https://api.seniverse.com/v3/weather/%s.json?key=%s&location=%s&language=zh-Hans&unit=c\r\n\r\n"
	
/* JSON数据包 */	
#define  NOW_JSON     "now"
#define  DAILY_JSON   "daily"


/* 天气数据结构体 */
typedef struct
{
	/* 实况天气数据 */
	char id[32];				//id
	char name[32];				//地名
	char country[32];			//国家
	char path[32];				//完整地名路径
	char timezone[32];			//时区
	char timezone_offset[32];   //时差
	char text[32];				//天气预报文字
	char code[32];				//天气预报代码
	char temperature[32];   	//气温
	char last_update[32];		//最后一次更新的时间
	
	
	/* 今天、明天、后天天气数据 */
	char date[3][32];			//日期
	char text_day[3][64];	    //白天天气现象文字
	char code_day[3][32];		//白天天气现象代码
	char code_night[3][64]; 	//晚间天气现象代码
	char high[3][32];			//最高温
	char low[3][32];			//最低温
	char wind_direction[3][64]; //风向
	char wind_speed[3][32];  	//风速，单位km/h（当unit=c时）
	char wind_scale[3][32];  	//风力等级
}Weather;

// 函数声明
 void GetWeather(char *weather_json, char *location, Weather *result);
 int cJSON_NowWeatherParse(char *JSON, Weather *result);
 int cJSON_DailyWeatherParse(char *JSON, Weather *result);
 char* mqttCreatePayload(Weather *weather_data);

#endif
