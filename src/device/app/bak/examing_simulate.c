/***********************************************************************************
 * @file   			: examing_simulate.c
 * @author   		: xqchen
 * @date   			: 2020-10-21
 * @Description   	: 本文件主要模拟考试者答题的过程
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/
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
#include "mqtt_api.h"


UINT32 exam_data_to_package(EXAMING_DATA *data, data_package *package)
{	
	AddDataToPackage(package, PKG_DATA_TYPE_GPEN_ID, 
						strlen((char *)data->GpenID), data->GpenID);
	AddDataToPackage(package, PKG_DATA_TYPE_STU_NAME, 
						strlen((char *)data->StuName), data->StuName);
	AddDataToPackage(package, PKG_DATA_TYPE_STU_ID, 
						strlen((char *)data->StuIDCardNo), data->StuIDCardNo);
	AddDataToPackage(package, PKG_DATA_TYPE_TOPIC, 
						strlen((char *)data->Topic), data->Topic);
	AddDataToPackage(package, PKG_DATA_TYPE_ANSWER, 
						strlen((char *)data->Answer), data->Answer);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_ADDR, 
						strlen((char *)data->ExamAddr), data->ExamAddr);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_SUBJECT, 
						strlen((char *)data->ExamSubject), data->ExamSubject);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_START_TIME, 
						strlen((char *)data->StartTime), data->StartTime);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_CURR_TIME, 
						strlen((char *)data->CurrTime), data->CurrTime);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_END_TIME, 
						strlen((char *)data->EndTime), data->EndTime);

	return package->data_len;
}

UINT32 package_to_exam_data(data_package *package, EXAMING_DATA *data)
{
	UINT8 *tmp = NULL;
	UINT32 len = 0;
	
	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);
		memcpy(&(data->GpenID), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_STU_NAME, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);
		memcpy(&(data->StuName), tmp, len);
		free(tmp);
	}
	
	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_STU_ID, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->StuIDCardNo), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_TOPIC, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->Topic), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_ANSWER, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->Answer), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_ADDR, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->ExamAddr), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_SUBJECT, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(&(data->ExamSubject), tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_START_TIME, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(data->StartTime, tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_CURR_TIME, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(data->CurrTime, tmp, len);
		free(tmp);
	}

	tmp = GetDataFromPackage(package, PKG_DATA_TYPE_EXAM_END_TIME, &len);
	if(tmp)
	{
		printHexData("tmp:", tmp, len);

		memcpy(data->EndTime, tmp, len);
		free(tmp);
	}

	
	return 0;
}



UINT32 ExamDataToPlatform(data_package *package, UINT8 *gpen_id, PKG_TYPE_E pkg_type)
{
	MQTT_PARAM para;
	UINT8 topic[32];

	memset(topic, 0, sizeof(topic));

	switch (pkg_type)
	{
		case PKG_TYPE_DATA:
			sprintf(topic, " /exam/%s/data", gpen_id);
			break;
		case PKG_TYPE_REGISTER:
			sprintf(topic, " /exam/%s/register", gpen_id);
			break;
	}

	memset(&para, 0, sizeof(MQTT_PARAM));

	para.clientid 	= gpen_id;
	para.host 		= config.broker_addr;
	para.port 		= config.broker_port;
	para.username 	= config.broker_username;
	para.password 	= config.broker_pwd;
	para.qos = 0;

	return MqttPublish(topic, package->data, package->data_len, &para);
}

UINT32 ExamDataPoESave(EXAMING_DATA *data, UINT8 *access_id, UINT8 *access_key)
{
	data_package package;
	POE_SAVE_RESPONE save_resp;
	UINT32 package_len = 0;
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;

	/*1.将数据封装成TLV数据包*/
	memset(&package, 0, sizeof(package));
	package_len = exam_data_to_package(data, &package);

	/*2.计算整个TLV数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package_len, SIGN_ECDSA, 
						 data->DevPriKey, signed_msg);
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

	/*4.将整个TLV数据包，哈希存证*/
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
	ExamDataToPlatform(&package, data->GpenID, PKG_TYPE_DATA);


	/*7.释放相关资源*/
	//free_tlv(&package);
	PoeSaveResponeDestroy(&save_resp);
	
	return 0;
}

UINT32 Examing(UINT8 *access_id, UINT8 *access_key, UINT8 *gpen_key, UINT8 *gpen_id)
{
	EXAMING_DATA data;
	
	memset(&data, 0, sizeof(data));

	memcpy(data.DevPriKey, gpen_key, sizeof(gpen_key));
	sprintf(data.GpenID, 		"%s", gpen_id);
    sprintf(data.StuName, 		"%s", "张三");
    sprintf(data.StuIDCardNo, 	"%s", "36233");
    sprintf(data.ExamAddr, 		"%s", "上海");
    sprintf(data.ExamSubject, 	"%s", "化学");
    sprintf(data.StartTime, 	"%s", "2020-10-23 14:00");
	sprintf(data.CurrTime, 		"%s", "2020-10-23 15:00");
	sprintf(data.EndTime, 		"%s", "2020-10-23 16:00");
    sprintf(data.Topic, 		"%s", "考题1");
    sprintf(data.Answer, 		"%s", "A");

	ExamDataPoESave(&data, access_id, access_key);
}



#if 0
UINT32 ExamDataPoEVerify(void *data, UINT32 data_len, UINT8 *access_id, UINT8 *access_key)
{
	data_package package;
	data_package new_package;
	POE_QUERY_RESPONE query_result;
	UINT32 package_len = 0;
	UINT8 hasher[32];
	UINT32 hash_len = 0;
	UINT8 PoeHashContent[32];
	UINT8 pub_key[65];
	UINT32 ret = ERR_SUCCESS;
	UINT8 *sig = NULL;
	UINT8 *PoeHash = NULL;
	UINT8 PoeHashStr[33];
	UINT32  len = 0;
	EXAMING_DATA exam_data;

	/*1.解析TLV数据包*/
	memset(&package, 0, sizeof(package));
	package.data_len = data_len;
	memcpy(package.data, data, data_len);
	
	sig = GetDataFromPackage(&package, PKG_DATA_TYPE_SIGN, &len);
	if(sig)
	{
		printHexData("sig:", sig, len);
	}

	PoeHash = GetDataFromPackage(&package, PKG_DATA_TYPE_POE_HASH, &len);
	if(PoeHash)
	{
		printHexData("PoeHash:", PoeHash, len);
	}
	memset(PoeHashStr, 0, sizeof(PoeHashStr));
	HexToStr((char *)PoeHashStr, PoeHash, len);

	memset(&exam_data, 0, sizeof(exam_data));
	package_to_exam_data(&package, &exam_data);

	/*2.使用需要签名部分数据生成新的数据包*/
	memset(&new_package, 0, sizeof(new_package));
	package_len = exam_data_to_package(&exam_data, &new_package);
	
	/*3.利用整个TLV数据包及传递进来的签名值恢复出设备的公钥*/
	memset(pub_key, 0, sizeof(pub_key));
	PublicKeyRecover(ENC_TYPE_ECDSA, sig,  
					 (UINT8 *)&new_package, package_len, pub_key);

	/*4.将签名数据添加到TLV数据包中*/
	AddDataToPackage(&new_package, PKG_DATA_TYPE_SIGN, 65, sig);
	package_len = new_package.data_len;

	/*5.计算整个TLV数据包的哈希值*/
	memset(hasher, 0, sizeof(hasher));
	hash_len = DataHash((UINT8 *)&new_package, package_len, HASH_MD5, hasher);
	if(hash_len == 0)
	{
		printf("poe verify fail: hash fail.\r\n");
		return -1;
	}

	/*6.根据存证凭证到区块链上查询相关数据*/
	memset(&query_result, 0, sizeof(query_result));
	
	do
	{
		ret = TextHashPoeQuery(access_id, access_key, PoeHashStr, &query_result);
	}while (query_result.PoeStatus == POE_STATUS_PROCESSING);
	
	if(ret != ERR_SUCCESS)
	{
		printf("poe verify fail: PoE query fail.\r\n");
		return -1;
	}
	
	/*7.比较区块链上查询到的哈希值与本地计算的哈希值是否一致*/
	/*若一致，则继续后续操作；若不一致，则返回失败*/
	StrToHex(PoeHashContent, (char *)query_result.PoeContent, 
			 strlen((char *)query_result.PoeContent)/2);
	if(memcmp(hasher, PoeHashContent, sizeof(hasher) != 0))
	{
		printHexData("hasher:", hasher, hash_len);
		printf("poe verify fail：data hash verify fail\r\n");
		return -1;
	}

	/*8.发送恢复出来的公钥至公钥认证服务器，以便确认该设备是否经过认证 */
	printHexData("public key", pub_key, sizeof(pub_key));
	
	/*9.释放相关资源*/
	free(sig);
	free(PoeHash);
	PoeQueryResponeDestroy(&query_result);
	printf("poe verify ok\r\n");

	return 0;
}
#endif


