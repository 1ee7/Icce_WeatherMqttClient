#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<signal.h>
#include "icceglobal.h"
#include "iccelog.h"
#include "iccemqttclient.h"
#include "icceweatherclient.h"
#include "cJSON.h"

#define TOPIC       "sys/device/20210728/variant_data"
#define QOS         1

timer_t timeid;  //定义一个全局的定时器id
Weather weather_data = {0};
char location[32] = {"linyi"};

char *cPayloadMsg;
static int i=0,j=0;


 /********************** 
 **    30s定时任务 
 **********************/
void task()  
{  
   	char *cPayloadMsg;
   	if(i++ == 720) //6h一次记录
	{
	  i=0;
	  log4c("\n******task start*********%d\n ",j++);
	}
	
  	memset(&weather_data, 0, sizeof(weather_data));  // weather_data清零 
	GetWeather(NOW_JSON, location, &weather_data);   // GET 并解析实况天气数据
    GetWeather(DAILY_JSON, location, &weather_data); // GET 并解析近三天天气数据
	
	
	
	cPayloadMsg=mqttCreatePayload(&weather_data);
	mqtt_client_publish(TOPIC, QOS, cPayloadMsg, strlen(cPayloadMsg));
	
	
	free(cPayloadMsg);			
	
}


 /********************** 
 **    创建定时器 
 **********************/
void create_timer()
 {

    struct sigevent evp;  //环境结构体
    int ret = 0;

    memset(&evp, 0, sizeof(struct sigevent));

    evp.sigev_value.sival_ptr = &timeid;    //绑定i定时器
    evp.sigev_notify = SIGEV_SIGNAL;  //设置定时器到期后触发的行为是为发送信号
    evp.sigev_signo = SIGUSR1;  //设置信号为用户自定义信号1
    signal(SIGUSR1, task);  //绑定产生信号后调用的函数
  
    ret = timer_create(CLOCK_REALTIME, &evp, &timeid);  //创建定时器
    if( ret  == 0)   
        log4c("timer_create ok\n");  
 } 
 /********************** 
 **    配置定时器 
 **********************/ 
void init_timer()
{
    int ret = 0;
    struct itimerspec ts;  
     ts.it_interval.tv_sec = 30;  //设置定时器的定时周期是30s
    ts.it_interval.tv_nsec = 0;  
    ts.it_value.tv_sec = 20;  //设置定时器20s后启动
    ts.it_value.tv_nsec = 0;  
  
    ret = timer_settime(timeid, 0, &ts, NULL);  //初始化定时器
    if( ret ==0)   
        log4c("timer_settime ok\n");  
}


int main(void)
{
	mqttInit();
	
	create_timer();
    init_timer();
	
	//定时调用 添加
	while (1) // 读入地名拼音
	{
	
		sleep(100000);
	}	
	
	return 0;	
}

