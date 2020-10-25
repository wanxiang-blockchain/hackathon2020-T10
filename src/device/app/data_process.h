/***********************************************************************************
 * @file   			: exam_data_process.h
 * @author   		: xqchen
 * @date   			: 2020-10-21
 * @brief   		: exam_simulate.c header file
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __EXAM_DATA_PROCESS_H__
#define __EXAM_DATA_PROCESS_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "types.h"


#define MESSAGE_MAX_LENGTH		1024

#define JSON_EXAM_GPEN_ID		"GpenID"
#define JSON_EXAM_GPEN_PUBKEY	"GpenPubKey"
#define JSON_EXAM_STU_NAME		"StuName"
#define JSON_EXAM_STU_PIC		"StuPic"
#define JSON_EXAM_STU_ID		"StuIDCardNo"
#define JSON_EXAM_TOPIC			"Topic"
#define JSON_EXAM_ANSWER		"Answer"
#define JSON_EXAM_SIGN			"Sign"
#define JSON_EXAM_POE_HASH		"PoEHash"
#define JSON_EXAM_RESULT		"Result"

#define JSON_EXAM_ADDR			"ExamAddr"
#define JSON_EXAM_NAME			"ExamName"
#define JSON_EXAM_TIME			"ExamTime"

#define JSON_EXAM_SUBJECT		"ExamSubject"	
#define JSON_EXAM_START_TIME	"StartTime"
#define JSON_EXAM_CURR_TIME		"CurrTime"
#define JSON_EXAM_END_TIME		"EndTime"

#define JSON_EXAM_PIC_ID		"StuPicID"
#define JSON_EXAM_PIC_NAME		"StuPicName"


typedef enum
{
	PKG_DATA_TYPE_GPEN_ID			= 0x100,
	PKG_DATA_TYPE_GPEN_PUBKEY		= 0x101,
	PKG_DATA_TYPE_STU_NAME			= 0x102,
	PKG_DATA_TYPE_STU_ID			= 0x103,
	PKG_DATA_TYPE_TOPIC 			= 0x104,
	PKG_DATA_TYPE_ANSWER			= 0x105,
	PKG_DATA_TYPE_SIGN				= 0x106,
	PKG_DATA_TYPE_POE_HASH			= 0x107,
	PKG_DATA_TYPE_RESULT			= 0x108,
	PKG_DATA_TYPE_EXAM_ADDR			= 0x109,		
	PKG_DATA_TYPE_EXAM_SUBJECT		= 0x110,	
	PKG_DATA_TYPE_EXAM_START_TIME	= 0x111,
	PKG_DATA_TYPE_EXAM_CURR_TIME	= 0x112,	
	PKG_DATA_TYPE_EXAM_END_TIME		= 0x113,
	PKG_DATA_TYPE_STU_PIC			= 0x114,
	PKG_DATA_TYPE_EXAM_NAME			= 0x115,
	PKG_DATA_TYPE_EXAM_TIME			= 0x116,
	PKG_DATA_TYPE_PIC_ID			= 0x117,
	PKG_DATA_TYPE_PIC_NAME			= 0x118,
	
}DATA_TYPE_E;

typedef enum
{
	PKG_TYPE_STU_REG,
	PKG_TYPE_GPEN_REG,
	PKG_TYPE_GPEN_STU_REG,
	PKG_TYPE_EXAM_START,
	PKG_TYPE_EXAM_ANSWER,
	PKG_TYPE_EXAM_CHECK,
	PKG_TYPE_END,
}PKG_TYPE_E;

typedef struct _EXAMING_DATA
{
	UINT8 DevPriKey [32];
	UINT8 GpenID[32];		//智能笔ID
    UINT8 StuName[32];		//考试者姓名
    UINT8 StuIDCardNo[32];	//考试者身份证
    UINT8 ExamAddr[32];		//考试场编号
    UINT8 ExamSubject[32];	//开始科目
    UINT8 StartTime[32];	//考试开始时间
	UINT8 CurrTime[32];		//当前时间
	UINT8 EndTime[32];		//考试结束时间，考试过程中，该值为空
    UINT8 Topic[32];		//正在作答题目
    UINT8 Answer[32];		//考试者答题内容
}EXAMING_DATA ,*P_EXAMING_DATA; 



typedef struct data_package_struct{
	UINT32 data_len;
	UINT8 data[MESSAGE_MAX_LENGTH];
} data_package;

UINT32 AddDataToPackage(data_package *package, DATA_TYPE_E type, UINT32 value_len, UINT8* value);
UINT8 *GetDataFromPackage(data_package *package, DATA_TYPE_E type, UINT32 *len);
INT32 FilePutToPlatform(UINT8 *filename, UINT8 *picname);
void GetTimeString(UINT8 *str);
UINT32 PkgPoESave(data_package *package, UINT8 *access_id, UINT8 *access_key);
UINT32 ExamDataToPlatform(data_package *package, PKG_TYPE_E pkg_type);
void GetPic(UINT32 with_info, UINT8 *picname);
void GetStuFile(UINT8 *filename);
void GetGpenId(UINT8 *GpenId, UINT8 *gpen_file);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __EXAM_DATA_PROCESS_H__ */
