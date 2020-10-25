/***********************************************************************************
 * @file   			: examinee_registration.h
 * @author   		: xqchen
 * @date   			: 2020-10-22
 * @brief   		: examinee_registration.c header file
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __EXAMINEE_REGISTRATION_H__
#define __EXAMINEE_REGISTRATION_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "exam_data_process.h"

UINT32 ExamineeRegisterDataPoESave(stu_config *stu_info, UINT8 *access_id, UINT8 *access_key);
UINT32 ExamineeRegisterToPlatform(data_package *package);
UINT32 ExamineeRegistration(UINT8 *access_id, UINT8 *access_key);
UINT32 examinee_register_data_to_package(stu_config *data, data_package *package);
UINT32 package_to_examinee_register(data_package *package, stu_config *data);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __EXAMINEE_REGISTRATION_H__ */
