#include "iccemqttclient.h"
const char *UserName="20210728";
const char *PassWord="-----"; //改为自己的密码

void connlost(void *context, char *cause)
{
//    printf("\nConnection lost\n");
//    printf("     cause: %s\n", cause);
	log4c("Connection lost cause %s\n",cause);

}

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    log4c("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    log4c("Message arrived\n");
  //  printf("     topic: %s\n", topicName);
   // printf("   message: ");

  //  payloadptr = message->payload;
//    for(i=0; i<message->payloadlen; i++)
  //  {
  //      putchar(*payloadptr++);
  //  }
  //  putchar('\n');

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

int mqttInit()
{
	int rc;
	MQTTClient_connectOptions mqtt_conn_opts = MQTTClient_connectOptions_initializer;
   
    MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    mqtt_conn_opts.keepAliveInterval = 20;
    mqtt_conn_opts.cleansession = 1;
	mqtt_conn_opts.username=UserName;
	mqtt_conn_opts.password=PassWord;
	
	 if ((rc = MQTTClient_connect(client, &mqtt_conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }
    printf("Success to connect!\n");
    return 0;
}

int mqtt_client_subscribe(char *top, int qos)
{
    if (client == NULL)
	return -1;

    MQTTClient_subscribe(client, top, qos);
    return 0;
}


int mqtt_client_publish(char *top, int qos, char *msg, int len)
{
	MQTTClient_deliveryToken token;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    if (client == NULL)
        return -1;

    pubmsg.payload = msg;
    pubmsg.payloadlen = len;
    pubmsg.qos = qos;
    pubmsg.retained = 0;
    deliveredtoken = 0;
    MQTTClient_publishMessage(client, top, &pubmsg, &token);
  //  printf("Waiting for publication of %s\n"
    //        "on topic %s for client\n",
      //      msg, top);
			
	//log4c("Waiting for publication of %s\n  on topic %s for client\n",msg, top);
    while (deliveredtoken != token);
    return 0;
}
