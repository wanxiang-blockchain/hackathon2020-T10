/***********************************************************************************
 * @file   			: exam.h
 * @author   		: xqchen
 * @date   			: 2020-10-24
 * @brief   		: exam.c header file
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __EXAM_H__
#define __EXAM_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "data_process.h"


UINT32 Examing();
UINT32 ExamStart();
UINT32 ExamCheck();
UINT32 ExamEnd();
void StartExam();


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __EXAM_H__ */
