/***********************************************************************************
 * @file   			: examing_simulate.h
 * @author   		: xqchen
 * @date   			: 2020-10-21
 * @brief   		: examing_simulate.c header file
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __EXAMING_SIMULATE_H__
#define __EXAMING_SIMULATE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

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

UINT32 Examing(UINT8 *access_id, UINT8 *access_key, UINT8 *gpen_key, UINT8 *gpen_id);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __EXAMING_SIMULATE_H__ */
