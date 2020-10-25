#include <string.h>
#include <stdlib.h>

#include "application_api.h"
#include "algorithm_api.h"

#include "types.h"
#include "exam_data_process.h"
#include "examing_simulate.h"
#include "hexutils.h"
#include "exam_config.h"
#include "http_client.h"


UINT32 GpenDataToRegister(data_package *package, UINT8 *gpen_id)
{
	http_respone post_return;
	ERR_CODE ret = ERR_SUCCESS;
	data_package package_return;
	UINT8 *value = NULL;
	UINT32 len = 0;

	/*4.HTTP data post*/
	memset(&post_return, 0, sizeof(post_return));
	ret = http_post(config.register_url, (char *)package->data,  
					package->data_len, (void *)&post_return);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	memset(&package_return, 0, sizeof(package_return));
	package_return.data_len = post_return.data_len;
	memcpy(package_return.data, post_return.data, post_return.data_len);
	if(post_return.data)
	{
		free(post_return.data);
		post_return.data = NULL;
	}
	
	value = GetDataFromPackage(&package_return, PKG_DATA_TYPE_RESULT, &len);
	if(value == NULL)
	{
		return -1;
	}

	printf("register result:%s\r\n", value);
	return 0;
}

UINT32 GpenDataToPlatform(data_package *package)
{
	MQTT_PARAM para;
	UINT8 topic[64];
	UINT8 *tmp = NULL;
	UINT32 len = 0;
	UINT32 ret = 0;

	memset(&para, 0, sizeof(MQTT_PARAM));

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
	if(tmp)
	{
		printf("tmp:%s\r\n", tmp);
	}

	memset(topic, 0, sizeof(topic));
	sprintf(topic, "/register/%s/gpen", tmp);

	para.clientid 	= tmp;
	para.host 		= config.broker_addr;
	para.port 		= config.broker_port;
	para.username 	= config.broker_username;
	para.password 	= config.broker_pwd;
	para.qos = 0;

	ret = MqttPublish(topic, package->data, package->data_len, &para);
	if(tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	
	return ret;
}



UINT32 GpenRegister(UINT8 *gpen_id, UINT8 *pub_key)
{
	data_package package;
	UINT8 pub_key_str[132];
	UINT8 time_str[64];

	memset(&package, 0, sizeof(package));
	
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_ID, strlen((char *)gpen_id), gpen_id);
	HexToStr((char *)pub_key_str, pub_key, sizeof(pub_key));
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_PUBKEY, strlen((char *)pub_key_str), pub_key_str);
	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);
	
	//GpenDataToRegister(&package, gpen_id);
	GpenDataToPlatform(&package);
	return 0;
}

