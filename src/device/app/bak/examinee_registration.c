#include <string.h>
#include <stdlib.h>


#include "types.h"
#include "exam_data_process.h"
#include "examing_simulate.h"
#include "hexutils.h"
#include "exam_config.h"

#include "cJSON.h"
#include "http_client.h"
#include "mqtt_api.h"
#include "application_api.h"
#include "algorithm_api.h"


UINT32 examinee_register_data_to_package(stu_config *data, data_package *package)
{	
	UINT8 time_str[64];  
	
	AddDataToPackage(package, PKG_DATA_TYPE_STU_ID, 
						strlen((char *)data->stu_id), data->stu_id);
	AddDataToPackage(package, PKG_DATA_TYPE_STU_NAME, 
						strlen((char *)data->stu_name), data->stu_name);
	AddDataToPackage(package, PKG_DATA_TYPE_STU_PIC, 
						strlen((char *)data->stu_pic), data->stu_pic);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_ADDR, 
						strlen((char *)data->exam_addr), data->exam_addr);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_NAME, 
						strlen((char *)data->exam_name), data->exam_name);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_TIME, 
						strlen((char *)data->exam_time), data->exam_time);

	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);

	return package->data_len;
}

UINT32 package_to_examinee_register(data_package *package, stu_config *data)
{
	UINT8 *tmp = NULL;
	UINT32 len = 0;
	
	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_STU_ID, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);
		memcpy(&(data->stu_id), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_STU_NAME, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);
		memcpy(&(data->stu_name), tmp, len);
		free(tmp);
	}
	
	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_STU_PIC, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->stu_pic), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_ADDR, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->exam_addr), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_NAME, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->exam_name), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_ADDR, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->exam_addr), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_TIME, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->exam_time), tmp, len);
		free(tmp);
	}
	
	return 0;
}

UINT32 ExamineeRegisterToPlatform(data_package *package)
{
	MQTT_PARAM para;
	UINT8 topic[64];
	UINT8 *tmp = NULL;
	UINT32 len = 0;
	UINT32 ret = 0;

	memset(&para, 0, sizeof(MQTT_PARAM));
	memset(topic, 0, sizeof(topic));

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_STU_ID, &len);
	if(tmp)
	{
		printf("tmp:%s\r\n", tmp);
	}

	sprintf(topic, "/register/%s/stu", tmp);

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


UINT32 ExamineeRegisterDataPoESave(stu_config *stu_info, UINT8 *access_id, UINT8 *access_key)
{
	data_package package;
	POE_SAVE_RESPONE save_resp;
	UINT32 package_len = 0;
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;
	UINT8 url[128];

	/*1.将数据封装成数据包*/
	memset(&package, 0, sizeof(package));
	package_len = examinee_register_data_to_package(stu_info, &package);

	/*2.计算整个TLV数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package_len, SIGN_ECDSA, 
						 stu_info->DevPriKey, signed_msg);
	if(ret != ERR_SUCCESS)
	{
		return 0;
	}
	//printHexData("signed_msg:", signed_msg, sizeof(signed_msg));

	/*3.将签名数据添加到TLV数据包中*/
	memset(tmp_str, 0, sizeof(tmp_str));
	HexToStr((char *)tmp_str, signed_msg, sizeof(signed_msg));
	AddDataToPackage(&package, PKG_DATA_TYPE_SIGN, strlen(tmp_str), tmp_str);
	package_len = package.data_len;

	/*4.将整个数据包，哈希存证*/
	memset(&save_resp, 0, sizeof(save_resp));
	TextHashPoeSave(access_id, access_key, (UINT8 *)&package, package_len, &save_resp);
	printf("SaveRetCode:%d\r\n", save_resp.SaveRetCode);
	printf("SaveRetDesc:%s\r\n", save_resp.SaveRetDesc);
	printHexData("poe hash:", save_resp.SavePoeHash, save_resp.HashLen);

	/*5.将哈希存证凭证，添加到TLV数据包中*/
	memset(tmp_str, 0, sizeof(tmp_str));
	HexToStr((char *)tmp_str, save_resp.SavePoeHash, save_resp.HashLen);
	AddDataToPackage(&package, PKG_DATA_TYPE_POE_HASH, strlen(tmp_str), tmp_str);
	package_len = package.data_len;

	
	/*6.将整个TLV数据包发送给物联网数据平台*/
	ExamineeRegisterToPlatform(&package);

	memset(url, 0, sizeof(url));
	sprintf(url, "http://%s:%d/%s?token=%s", config.file_put_addr, 
											 config.file_put_port,
											 config.file_put_url,
											 config.file_put_token);
	FilePutToPlatform(stu_info->stu_pic, url);


	/*7.释放相关资源*/
	//free_tlv(&package);
	PoeSaveResponeDestroy(&save_resp);
	
	return 0;
}


UINT32 ExamineeRegistration(UINT8 *access_id, UINT8 *access_key)
{
	stu_config stu_info;
	UINT8 cmd[128];
	data_package package;
	
	
	printf("请输入考生信息：\n");
	getchar();
	memset(&stu_info, 0, sizeof(stu_info));
	read_stu_info("examinee.ini", &stu_info);
	printf("请正视摄像头，系统将采集您的照片\n");
	printf("准备好之后，请按任意键继续\n");
	getchar();
	memset(cmd, 0, sizeof(cmd));
	memset(stu_info.stu_pic, 0, sizeof(stu_info.stu_pic));
	sprintf(stu_info.stu_pic, "%s.jpg", stu_info.stu_name);
	sprintf(cmd, "fswebcam -S 10 %s 2>null", stu_info.stu_pic);
	system(cmd);

	memset(&package, 0, sizeof(package));
	ExamineeRegisterDataPoESave(&stu_info, access_id, access_key);
	return 0;
}


