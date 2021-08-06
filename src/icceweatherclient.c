#include "icceweatherclient.h"


/*******************************************************************************************************
** 函数: cJSON_NowWeatherParse，解析天气实况数据
** 参数: JSON：天气数据包   result：数据解析的结果
** 返回: void
********************************************************************************************************/
int cJSON_NowWeatherParse(char *JSON, Weather *result)
{
	cJSON *json,*arrayItem,*object,*subobject,*item;
	
	json = cJSON_Parse(JSON); //解析JSON数据包
	if(json == NULL)		  //检测JSON数据包是否存在语法上的错误，返回NULL表示数据包无效
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr()); //打印数据包语法错误的位置
		return 1;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL); //匹配字符串"results",获取数组内容
		{
			int size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数

#if DEBUG

			printf("cJSON_GetArraySize: size=%d\n",size); 
#endif
			if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//获取父对象内容
			{
				/* 匹配子对象1：城市地区相关 */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{
					// 匹配id
					if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)   
					{
						memcpy(result->id, item->valuestring,strlen(item->valuestring)); 		// 保存数据供外部调用
					}
					// 匹配城市名
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) 
					{
						memcpy(result->name, item->valuestring,strlen(item->valuestring)); 		// 保存数据供外部调用
					}
					// 匹配城市所在的国家
					if((item = cJSON_GetObjectItem(subobject,"country")) != NULL)
					{
						memcpy(result->country, item->valuestring,strlen(item->valuestring)); 	// 保存数据供外部调用
					}
					// 匹配完整地名路径
					if((item = cJSON_GetObjectItem(subobject,"path")) != NULL)  
					{
						memcpy(result->path, item->valuestring,strlen(item->valuestring)); 		// 保存数据供外部调用	
					}
					// 匹配时区
					if((item = cJSON_GetObjectItem(subobject,"timezone")) != NULL)
					{
						memcpy(result->timezone, item->valuestring,strlen(item->valuestring)); 	// 保存数据供外部调用	
					}
					// 匹配时差
					if((item = cJSON_GetObjectItem(subobject,"timezone_offset")) != NULL)
					{
						memcpy(result->timezone_offset, item->valuestring,strlen(item->valuestring)); 	// 保存数据供外部调用
					}
				}
				/* 匹配子对象2：今天的天气情况 */
				if((subobject = cJSON_GetObjectItem(object,"now")) != NULL)
				{
					// 匹配天气现象文字
					if((item = cJSON_GetObjectItem(subobject,"text")) != NULL)
					{
						memcpy(result->text, item->valuestring,strlen(item->valuestring));  // 保存数据供外部调用
					}
					// 匹配天气现象代码
					if((item = cJSON_GetObjectItem(subobject,"code")) != NULL)
					{
						memcpy(result->code, item->valuestring,strlen(item->valuestring));  // 保存数据供外部调用
					}
					// 匹配气温
					if((item = cJSON_GetObjectItem(subobject,"temperature")) != NULL) 
					{
						memcpy(result->temperature, item->valuestring,strlen(item->valuestring));   // 保存数据供外部调用
					}	
				}
				/* 匹配子对象3：数据更新时间（该城市的本地时间） */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					memcpy(result->last_update, subobject->valuestring,strlen(subobject->valuestring));   // 保存数据供外部调用
				}
			} 
		}
	}
	
	cJSON_Delete(json); //释放cJSON_Parse()分配出来的内存空间
	
	return 0;
}

/*******************************************************************************************************
** 函数: cJSON_DailyWeatherParse，解析近三天天气数据
**------------------------------------------------------------------------------------------------------
** 参数: JSON：天气数据包   result：数据解析的结果
** 返回: void
********************************************************************************************************/
int cJSON_DailyWeatherParse(char *JSON, Weather *result)
{
	int i;
	cJSON *json,*arrayItem,*object,*subobject,*item,*sub_child_object,*child_Item;
	
	json = cJSON_Parse(JSON); //解析JSON数据包
	if(json == NULL)		  //检测JSON数据包是否存在语法上的错误，返回NULL表示数据包无效
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr()); //打印数据包语法错误的位置
		return 1;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL); //匹配字符串"results",获取数组内容
		{
			int size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数
#if DEBUG

			printf("Daily--- Get Array Size: size=%d\n",size); 
#endif
			if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//获取父对象内容
			{
				/* 匹配子对象1------结构体location */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //匹配子对象1成员"name"
					{
						memcpy(result->name, item->valuestring,strlen(item->valuestring)); 		// 保存数据供外部调用
					}
				}
				/* 匹配子对象2------数组daily */
				if((subobject = cJSON_GetObjectItem(object,"daily")) != NULL)
				{
					int sub_array_size = cJSON_GetArraySize(subobject);
#if DEBUG
					printf("Daily-- Get Sub Array Size: sub_array_size=%d\n",sub_array_size);
#endif
					for(i = 0; i < sub_array_size; i++)
					{
						if((sub_child_object = cJSON_GetArrayItem(subobject,i))!=NULL)
						{
							// 匹配日期
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"date")) != NULL)
							{
								memcpy(result->date[i], child_Item->valuestring,strlen(child_Item->valuestring)); 		// 保存数据
							}
							// 匹配白天天气现象文字
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"text_day")) != NULL)
							{
								memcpy(result->text_day[i], child_Item->valuestring,strlen(child_Item->valuestring)); 	// 保存数据
							}
							// 匹配白天天气现象代码
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"code_day")) != NULL)
							{
								memcpy(result->code_day[i], child_Item->valuestring,strlen(child_Item->valuestring)); 	// 保存数据
							}
							// 匹配夜间天气现象代码
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"code_night")) != NULL)
							{
								memcpy(result->code_night[i], child_Item->valuestring,strlen(child_Item->valuestring)); // 保存数据
							}
							// 匹配最高温度
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"high")) != NULL)
							{
								memcpy(result->high[i], child_Item->valuestring,strlen(child_Item->valuestring)); 		//保存数据
							}
							// 匹配最低温度
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"low")) != NULL)
							{
								memcpy(result->low[i], child_Item->valuestring,strlen(child_Item->valuestring)); 		// 保存数据
							}
							// 匹配风向
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"wind_direction")) != NULL)
							{
								memcpy(result->wind_direction[i],child_Item->valuestring,strlen(child_Item->valuestring)); //保存数据
							}
							// 匹配风速，单位km/h（当unit=c时）
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"wind_speed")) != NULL)
							{
								memcpy(result->wind_speed[i], child_Item->valuestring,strlen(child_Item->valuestring)); // 保存数据
							}
							// 匹配风力等级
							if((child_Item = cJSON_GetObjectItem(sub_child_object,"wind_scale")) != NULL)
							{
								memcpy(result->wind_scale[i], child_Item->valuestring,strlen(child_Item->valuestring)); // 保存数据
							}
						}
					}
				}
				/* 匹配子对象3------最后一次更新的时间 */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					//printf("%s:%s\n",subobject->string,subobject->valuestring);
				}
			} 
		}
	}
	
	cJSON_Delete(json); //释放cJSON_Parse()分配出来的内存空间
	
	return 0;
}

/***********************************************************************************************
** 函数: GetWeather，获取天气数据并解析
** 参数: weather_json：需要解析的json包   location：地名   result：数据解析的结果
** 返回: void
***********************************************************************************************/
void GetWeather(char *weather_json, char *location, Weather *result)
{
	int ClientSock;
	char GetRequestBuf[256] = {0};
	char WeatherRecvBuf[2*1024] = {0};
	char GbkRecvBuf[2*1024] = {0};
	int  gbk_recv_len = 0;
	int  connect_status = 0;
	
	
	/* 设置要访问的服务器的信息 */
    struct sockaddr_in  ServerSockAddr;
    memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));  		  // 每个字节都用0填充
    ServerSockAddr.sin_family = PF_INET;						  // IPv4
    ServerSockAddr.sin_addr.s_addr = inet_addr(WEATHER_IP_ADDR);  // 心知天气服务器IP
    ServerSockAddr.sin_port = htons(WEATHER_PORT);   			  // 端口
	
	/* 创建客户端socket */
	if (-1 == (ClientSock = socket(PF_INET, SOCK_STREAM, 0)))
	{
		log4c("socket error!\n");
		
		exit(1);
	}
	
	/* 连接服务端 */
	if (-1 == (connect_status = connect(ClientSock, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr))))
	{
		log4c("connect error!\n");
		exit(1);
	}
	
	/* 组合GET请求包 */
	sprintf(GetRequestBuf, GET_REQUEST_PACKAGE, weather_json, KEY, location);
	
	/* 发送数据到服务端 */
	send(ClientSock, GetRequestBuf, strlen(GetRequestBuf), 0);
		
	/* 接受服务端的返回数据 */
	recv(ClientSock,WeatherRecvBuf, 2*1024, 0);
	
	/* utf-8转为gbk */
//	SwitchToGbk((const unsigned char*)WeatherRecvBuf, strlen((const char*)WeatherRecvBuf), (unsigned char*)GbkRecvBuf, &gbk_recv_len);	
#if DEBUG
	printf("服务端返回的数据为：%s\n", WeatherRecvBuf);
#endif
	
	/* 解析天气数据并保存到结构体变量weather_data中 */
	if (0 == strcmp(weather_json,NOW_JSON))
	{
		cJSON_NowWeatherParse(WeatherRecvBuf, result);	
	}
	else if(0 == strcmp(weather_json, DAILY_JSON)) // 未来三天天气
	{
		cJSON_DailyWeatherParse(WeatherRecvBuf, result);	
	}
	
	/* 清空缓冲区 */
	memset(GetRequestBuf, 0, 256);   
	memset(WeatherRecvBuf, 0, 2*1024);   
	memset(GbkRecvBuf, 0, 2*1024); 
	
	/* 关闭套接字 */
	close(ClientSock);  
	
	/* 终止使用 DLL */
//	WSACleanup();  

}



/** 打包payload数据  **/
char* mqttCreatePayload(Weather *weather_data)
{
	cJSON *pldJson,*itemJson;	
	char *cdata;
	
	
	pldJson=cJSON_CreateObject();
	itemJson=cJSON_CreateObject();
	 
    cJSON_AddNumberToObject(itemJson,"NowT",atof(weather_data->temperature));
	cJSON_AddNumberToObject(itemJson,"WthCode",atoi(weather_data->code));
	
    cJSON_AddNumberToObject(itemJson,"HT1",atof(weather_data->high[0]));  
	cJSON_AddNumberToObject(itemJson,"LT1",atof(weather_data->low[0])); 
    cJSON_AddNumberToObject(itemJson,"Code1",atoi(weather_data->code_day[0])); 	
	
	cJSON_AddNumberToObject(itemJson,"HT2",atof(weather_data->high[1]));  
	cJSON_AddNumberToObject(itemJson,"LT2",atof(weather_data->low[1])); 
    cJSON_AddNumberToObject(itemJson,"Code2",atoi(weather_data->code_day[1])); 	
	
	
	cJSON_AddNumberToObject(itemJson,"HT3",atof(weather_data->high[2]));  
	cJSON_AddNumberToObject(itemJson,"LT3",atof(weather_data->low[2])); 
    cJSON_AddNumberToObject(itemJson,"Code3",atoi(weather_data->code_day[2])); 	
  
	
    cJSON_AddStringToObject(pldJson, "type","variant_data");
	cJSON_AddStringToObject(pldJson, "version","1.0");
	cJSON_AddNumberToObject(pldJson, "time",1600324099000);
	cJSON_AddItemToObject(pldJson,"params",itemJson);
		
	cdata=cJSON_Print(pldJson);
	
	
//	log4c("\n******debug ************\n  %s\n",result);
	
	
	cJSON_Delete(pldJson);
	return cdata; 
	
}
  
