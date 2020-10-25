/***********************************************************************************
 * @file   			: exam.c
 * @author   		: xqchen
 * @date   			: 2020-10-24
 * @Description   	: 本文件主要模拟考试者答题的过程
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "application_api.h"
#include "algorithm_api.h"
	
#include "types.h"
#include "data_process.h"
#include "exam.h"
#include "hexutils.h"
#include "exam_config.h"
#include "http_client.h"
#include "mqtt_api.h"

extern UINT8 *access_id;
extern UINT8 *access_key;

UINT32 exam_start = 0;
UINT8 stu_file[128];


void exam_status_set(UINT32 status)
{
	exam_start = status;
}

UINT32 exam_status_get()
{
	return exam_start;
}

/*****************************************************************************
 * @Function	: Examing
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现实时模拟考试数据
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 Examing()
{
	data_package package;
	UINT8 cmd[128];
	UINT8 time_str[64];  
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;
	//UINT8 stu_file[128];
	stu_config stu_info;


	//memset(stu_file, 0, sizeof(stu_file));
	//GetStuFile(stu_file);

	printf("\r\n考试正在进行中，正在采集考试数据...\r\n");
	memset(&stu_info, 0, sizeof(stu_info));
	read_stu_info(stu_file, &stu_info);
	
	memset(&package, 0, sizeof(package));
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_ID, strlen(stu_info.GpenId), stu_info.GpenId);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_NAME, strlen(stu_info.stu_name), stu_info.stu_name);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_ID, strlen(stu_info.stu_id), stu_info.stu_id);

	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_TOPIC, strlen("考题1"), "考题1");
	AddDataToPackage(&package, PKG_DATA_TYPE_ANSWER, strlen("A"), "A");

	/*2.计算整个数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package.data_len, SIGN_ECDSA, 
						 stu_info.DevPriKey, signed_msg);
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
	ExamDataToPlatform(&package, PKG_TYPE_EXAM_ANSWER);
}

/*****************************************************************************
 * @Function	: ExamStart
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现考试考试报文
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 ExamStart()
{
	data_package package;
	UINT8 time_str[64];  
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;
	//UINT8 stu_file[128];
	stu_config stu_info;
	INT32 pic_id;


	memset(stu_file, 0, sizeof(stu_file));
	GetStuFile(stu_file);
	
	memset(&stu_info, 0, sizeof(stu_info));
	read_stu_info(stu_file, &stu_info);
	
	memset(&package, 0, sizeof(package));
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_ID, strlen(stu_info.GpenId), stu_info.GpenId);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_NAME, strlen(stu_info.stu_name), stu_info.stu_name);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_ID, strlen(stu_info.stu_id), stu_info.stu_id);

	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_START_TIME, strlen(time_str), time_str);
	
	memset(stu_info.stu_pic, 0, sizeof(stu_info.stu_pic));
	memset(tmp_str, 0, sizeof(tmp_str));
	sprintf(tmp_str, "%s_%s.jpg", stu_info.stu_name, time_str);
	sprintf(stu_info.stu_pic, "data/pic/%s", tmp_str);
	GetPic(0, stu_info.stu_pic);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_PIC, strlen(tmp_str), tmp_str);

	/*2.计算整个数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package.data_len, SIGN_ECDSA, 
						 stu_info.DevPriKey, signed_msg);
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
	ExamDataToPlatform(&package, PKG_TYPE_EXAM_START);

	exam_status_set(1);
	printf("\r\n考试即将开始。\r\n");
	
}

/*****************************************************************************
 * @Function	: ExamCheck
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现考试过程中，身份校验报文
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 ExamCheck()
{
	data_package package;
	UINT8 time_str[64];  
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;
	//UINT8 stu_file[128];
	stu_config stu_info;
	INT32 pic_id;

	//memset(stu_file, 0, sizeof(stu_file));
	//GetStuFile(stu_file);

	printf("\r\n考试正在进行中，正在校验学生身份信息...\r\n");
	memset(&stu_info, 0, sizeof(stu_info));
	read_stu_info(stu_file, &stu_info);
	
	memset(&package, 0, sizeof(package));
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_ID, strlen(stu_info.GpenId), stu_info.GpenId);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_NAME, strlen(stu_info.stu_name), stu_info.stu_name);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_ID, strlen(stu_info.stu_id), stu_info.stu_id);

	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_START_TIME, strlen(time_str), time_str);

	memset(stu_info.stu_pic, 0, sizeof(stu_info.stu_pic));
	memset(tmp_str, 0, sizeof(tmp_str));
	sprintf(tmp_str, "%s_%s.jpg", stu_info.stu_name, time_str);
	sprintf(stu_info.stu_pic, "data/pic/%s", tmp_str);
	GetPic(0, stu_info.stu_pic);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_PIC, strlen(tmp_str), tmp_str);

	/*2.计算整个数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package.data_len, SIGN_ECDSA, 
						 stu_info.DevPriKey, signed_msg);
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
	ExamDataToPlatform(&package, PKG_TYPE_EXAM_CHECK);
}

/*****************************************************************************
 * @Function	: ExamEnd
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现考试结束报文
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 ExamEnd()
{
	data_package package;
	UINT8 time_str[64];  
	UINT8 signed_msg [ 65 ];
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;
	//UINT8 stu_file[128];
	stu_config stu_info;
	INT32 pic_id;

	//memset(stu_file, 0, sizeof(stu_file));
	//GetStuFile(stu_file);
	
	memset(&stu_info, 0, sizeof(stu_info));
	read_stu_info(stu_file, &stu_info);
	
	memset(&package, 0, sizeof(package));
	AddDataToPackage(&package, PKG_DATA_TYPE_GPEN_ID, strlen(stu_info.GpenId), stu_info.GpenId);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_NAME, strlen(stu_info.stu_name), stu_info.stu_name);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_ID, strlen(stu_info.stu_id), stu_info.stu_id);

	memset(time_str, 0, sizeof(time_str));
	GetTimeString(time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_CURR_TIME, strlen(time_str), time_str);
	AddDataToPackage(&package, PKG_DATA_TYPE_EXAM_START_TIME, strlen(time_str), time_str);
	
	memset(stu_info.stu_pic, 0, sizeof(stu_info.stu_pic));
	memset(tmp_str, 0, sizeof(tmp_str));
	sprintf(tmp_str, "%s_%s.jpg", stu_info.stu_name, time_str);
	sprintf(stu_info.stu_pic, "data/pic/%s", tmp_str);
	GetPic(0, stu_info.stu_pic);
	AddDataToPackage(&package, PKG_DATA_TYPE_STU_PIC, strlen(tmp_str), tmp_str);

	/*2.计算整个数据包的签名*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature((UINT8 *)&package, package.data_len, SIGN_ECDSA, 
						 stu_info.DevPriKey, signed_msg);
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
	ExamDataToPlatform(&package, PKG_TYPE_END);
	//FilePutToPlatform(stu_info.stu_pic);

	sprintf(stu_info.exam_end_time, "%s", tmp_str);
	save_stu_info(stu_file, &stu_info);
	exam_status_set(0);
	sleep(5);
	printf("\r\n考试即将结束.\r\n");
}

/*****************************************************************************
 * @Function	: do_examing
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 模拟考试线程
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void *do_examing()
{
	int ret = 0;

	while (1)
	{
		if(exam_status_get())
		{
			Examing(access_id, access_key);
			//sleep(5);
			ExamCheck(access_id, access_key);
			sleep(3);
		}
		else
		{
			sleep(10);
		}
	};
	
}


/*****************************************************************************
 * @Function	: StartExam
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现开始考试线程
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void StartExam()
{
	pthread_t tid;
	int ret = 0;

	ret = pthread_create(&tid, NULL, do_examing, NULL);
    if (ret != 0)
    {
        printf("can't create do examing thread, ret = %d\r\n", ret);
    }
}


