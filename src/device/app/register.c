#include <string.h>
#include <stdlib.h>

#include "application_api.h"
#include "algorithm_api.h"

#include "types.h"
#include "data_process.h"
#include "exam.h"
#include "hexutils.h"
#include "exam_config.h"
#include "http_client.h"

extern UINT8 *access_id;
extern UINT8 *access_key;

#if 0
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
#endif

/*****************************************************************************
 * @Function	: GpenRegister
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 智能笔注册函数
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 GpenRegister()
{
	data_package package;
	UINT8 tmp_key_str[132];
	UINT8 time_str[64];
	UINT8 pri_key[32];
	UINT8 pub_key[65];
	UINT8 GpenId[32];
	UINT8 gpen_file[64];
	INT32 ret = 0;
	gpen_config config;

	memset(pri_key, 0, sizeof(pri_key));
	memset(pub_key, 0, sizeof(pub_key));
	memset(GpenId, 0, sizeof(GpenId));

	memset(gpen_file, 0, sizeof(gpen_file));
	GetGpenId(GpenId, gpen_file);

	memset(&config, 0, sizeof(config));
	ret = read_gpen_info(gpen_file, &config);
	if(ret)
	{
		KeyPairCreate(ENC_TYPE_ECDSA, pri_key, pub_key);
		printHexData("pri_key:", pri_key, 32);
		printHexData("pub_key:", pub_key, 65);
		
		sprintf(config.GpenId, "%s", GpenId);
		
		memset(tmp_key_str, 0, sizeof(tmp_key_str));
		HexToStr((char *)tmp_key_str, pri_key, sizeof(pri_key));
		sprintf(config.DevPriKey, "%s", tmp_key_str);

		memset(tmp_key_str, 0, sizeof(tmp_key_str));
		HexToStr((char *)tmp_key_str, pub_key, sizeof(pub_key));
		sprintf(config.DevPubKey, "%s", tmp_key_str);
	}
	
	memset(&package, 0, sizeof(package));
	
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_ID, strlen((char *)GpenId), GpenId);
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_PUBKEY, strlen((char *)tmp_key_str), tmp_key_str);
	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);
	
	//GpenDataToRegister(&package, gpen_id);
	ExamDataToPlatform(&package, PKG_TYPE_GPEN_REG);

	save_gpen_info(gpen_file, &config);
	
	return 0;
}

UINT32 examinee_register_data_to_package(stu_config *data, data_package *package)
{	
	UINT8 time_str[64];  
	UINT8 tmp_str[64];  

	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	
	AddDataToPackage(package, PKG_DATA_TYPE_STU_ID, 
						strlen((char *)data->stu_id), data->stu_id);
	AddDataToPackage(package, PKG_DATA_TYPE_STU_NAME, 
						strlen((char *)data->stu_name), data->stu_name);
	memset(data->stu_pic, 0, sizeof(data->stu_pic));
	memset(tmp_str, 0, sizeof(tmp_str));
	sprintf(tmp_str, "%s_%s.jpg", data->stu_name, time_str);
	sprintf(data->stu_pic, "data/pic/%s", tmp_str);
	GetPic(0, data->stu_pic);
	AddDataToPackage(package, PKG_DATA_TYPE_STU_PIC, strlen(tmp_str), tmp_str);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_ADDR, 
						strlen((char *)data->exam_addr), data->exam_addr);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_NAME, 
						strlen((char *)data->exam_name), data->exam_name);
	AddDataToPackage(package, PKG_DATA_TYPE_EXAM_TIME, 
						strlen((char *)data->exam_time), data->exam_time);

	
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

/*****************************************************************************
 * @Function	: ExamineeRegistration
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 模拟考生报名函数
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 ExamineeRegistration()
{
	stu_config stu_info;
	data_package package;
	UINT8 stu_file[128];
	POE_SAVE_RESPONE save_resp;
	UINT32 package_len = 0;
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;
	UINT8 url[128];
	INT32 pic_id;
	
	memset(stu_file, 0, sizeof(stu_file));
	GetStuFile(stu_file);
	
	memset(&stu_info, 0, sizeof(stu_info));
	read_stu_info(stu_file, &stu_info);

	/*1.将数据封装成数据包*/
	memset(&package, 0, sizeof(package));
	package_len = examinee_register_data_to_package(&stu_info, &package);

	/*2.计算整个TLV数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package_len, SIGN_ECDSA, 
						 stu_info.DevPriKey, signed_msg);
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

	/*4.将整个数据包，区块链存证*/
	PkgPoESave(&package, access_id, access_key);
	
	/*6.将整个数据包发送给物联网数据平台*/
	UINT8 picname[128];
	memset(picname, 0, sizeof(picname));
	pic_id = FilePutToPlatform(stu_info.stu_pic, picname);
	memset(tmp_str, 0, sizeof(tmp_str));
	sprintf(tmp_str, "%d", pic_id);
	AddDataToPackage(&package, PKG_DATA_TYPE_PIC_ID, strlen(tmp_str), tmp_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_PIC_NAME, strlen(picname), picname);
	ExamDataToPlatform(&package, PKG_TYPE_STU_REG);

	return 0;
}

/*****************************************************************************
 * @Function	: ExamineeGpenBind
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 考生与智能笔绑定函数
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 ExamineeGpenBind()
{
	data_package package;
	UINT8 time_str[64];  
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;
	UINT8 stu_file[128];
	stu_config stu_info;
	UINT8 GpenId[32];
	UINT8 gpen_file[64];
	gpen_config gpen_info;
	INT32 pic_id;

	memset(stu_file, 0, sizeof(stu_file));
	GetStuFile(stu_file);

	memset(GpenId, 0, sizeof(GpenId));
	memset(gpen_file, 0, sizeof(gpen_file));
	GetGpenId(GpenId, gpen_file);
	
	memset(&stu_info, 0, sizeof(stu_info));
	read_stu_info(stu_file, &stu_info);

	memset(&gpen_info, 0, sizeof(gpen_info));
	read_gpen_info(gpen_file, &gpen_info);
	
	memset(&package, 0, sizeof(package));
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_ID, strlen(GpenId), GpenId);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_NAME, strlen(stu_info.stu_name), stu_info.stu_name);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_ID, strlen(stu_info.stu_id), stu_info.stu_id);

	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);
	
	memset(stu_info.stu_pic, 0, sizeof(stu_info.stu_pic));
	memset(tmp_str, 0, sizeof(tmp_str));
	sprintf(tmp_str, "%s_%s.jpg", stu_info.stu_name, time_str);
	sprintf(stu_info.stu_pic, "data/pic/%s", tmp_str);
	GetPic(0, stu_info.stu_pic);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_PIC, strlen(tmp_str), tmp_str);

	/*2.计算整个数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package.data_len, SIGN_ECDSA, 
						 gpen_info.DevPriKey, signed_msg);
	if(ret != ERR_SUCCESS)
	{
		return 0;
	}
	//printHexData("signed_msg:", signed_msg, sizeof(signed_msg));

	/*3.将签名数据添加到数据包中*/
	memset(tmp_str, 0, sizeof(tmp_str));
	HexToStr((char *)tmp_str, signed_msg, sizeof(signed_msg));
	AddDataToPackage(&package, PKG_DATA_TYPE_SIGN, strlen(tmp_str), tmp_str);

	/*4.将整个数据包，区块链存证*/
	PkgPoESave(&package, access_id, access_key);
	

	/*5.将整个数据包发送给物联网数据平台*/
	UINT8 picname[128];
	memset(picname, 0, sizeof(picname));
	pic_id = FilePutToPlatform(stu_info.stu_pic, picname);
	memset(tmp_str, 0, sizeof(tmp_str));
	sprintf(tmp_str, "%d", pic_id);
	AddDataToPackage(&package, PKG_DATA_TYPE_PIC_ID, strlen(tmp_str), tmp_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_PIC_NAME, strlen(picname), picname);
	ExamDataToPlatform(&package, PKG_TYPE_GPEN_STU_REG);
	//FilePutToPlatform(stu_info.stu_pic);

	sprintf(stu_info.DevPriKey, "%s", gpen_info.DevPriKey);
	sprintf(stu_info.GpenId, "%s", gpen_info.GpenId);
	save_stu_info(stu_file, &stu_info);
}


