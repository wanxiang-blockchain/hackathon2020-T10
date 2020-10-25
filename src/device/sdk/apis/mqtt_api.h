#ifndef __MQTT_API_H__
#define __MQTT_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#include "types.h"


typedef struct _MQTT_PARAM
{
	UINT8 *clientid;
	UINT8 *username;
	UINT8 *password;
	UINT8 *host;
	UINT32 qos;
	UINT32 port;
}MQTT_PARAM, *P_MQTT_PARAM;

UINT32 MqttPublish(UINT8 *topic, UINT8 *payload, UINT32 len, MQTT_PARAM *para);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif



