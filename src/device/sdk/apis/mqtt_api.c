#include <sys/time.h>
#include <memory.h>
#include <signal.h>

#include "mqtt/MQTTClient.h"
#include "mqtt_api.h"

#define SEND_BUFF_MAX	(1024*2)
#define READ_BUFF_MAX	(1024*2)

#if 0
static void printf_buf(UINT8 *buff, int len)
{
	int i = 0;

	for(i = 0; i< len; i++)
	{
		if(i%16 == 0)
		{
			printf("\r\n");
		}
		printf("%02x ", buff[i]);
	}
	printf("\r\n");
}
#endif


UINT32 MqttPublish(UINT8 *topic, UINT8 *payload, UINT32 len, MQTT_PARAM *para)
{
	UINT32 rc = 0;
	UINT8 sendbuf[SEND_BUFF_MAX];
	UINT8 readbuf[READ_BUFF_MAX];
	MQTTMessage pubmsg;
	
	Network n;
	MQTTClient c;

	//printf("payload len:%d\r\n", len);

	memset(sendbuf, 0, sizeof(sendbuf));
	memset(readbuf, 0, sizeof(readbuf));
	NetworkInit(&n);
	//"172.25.3.208"
	NetworkConnect(&n, (char *)para->host, (int)para->port);
	MQTTClientInit(&c, &n, 1000, (unsigned char*)sendbuf, sizeof(sendbuf), 
								 (unsigned char*)readbuf, sizeof(readbuf));
 
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.clientID.cstring = (char *)para->clientid;
	data.username.cstring = (char *)para->username;
	data.password.cstring = (char *)para->password;

	data.keepAliveInterval = 10;
	data.cleansession = 1;
	//printf("Connecting to %s %d\n", para->host, para->port);
	
	rc = MQTTConnect(&c, &data);
	if(rc)
	{
		printf("Connect fail %d\n", rc);
		return rc;
	}
	//printf("Connected ok\n");

	memset(&pubmsg, '\0', sizeof(pubmsg));
	pubmsg.payload = (void*)payload;
	pubmsg.payloadlen = len;
	pubmsg.qos = QOS0;
	pubmsg.retained = 0;
	pubmsg.dup = 0;
	//printf_buf((UINT8 *)pubmsg.payload, pubmsg.payloadlen);
	rc = MQTTPublish(&c, (char *)topic, &pubmsg);
	if(rc)
	{
		printf("Publish fail %d\n", rc);
		return rc;
	}
	//printf("Publish ok\n");

	MQTTDisconnect(&c);
	NetworkDisconnect(&n);

	return 0;
}


