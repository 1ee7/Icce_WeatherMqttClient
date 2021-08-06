#ifndef ICCEMQTTCLIENT_H
#define ICCEMQTTCLIENT_H


#include <MQTTAsync.h>
#include <MQTTClient.h>
#include "icceglobal.h"
/*  MQTT  */
#define ADDRESS     "www.sukon-cloud.com:9006"
#define CLIENTID    "20210728"

#define TIMEOUT     10000L

static MQTTClient client = NULL;
volatile MQTTClient_deliveryToken deliveredtoken;

 int mqttInit();
 int mqtt_client_publish(char *top, int qos, char *msg, int len);
#endif
