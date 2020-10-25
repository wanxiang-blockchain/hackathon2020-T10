/***********************************************************************************
 * @file   			: auth_simulate.h
 * @author   		: xqchen
 * @date   			: 2020-10-21
 * @brief   		: auth_simulate.c header file
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __AUTH_SIMULATE_H__
#define __AUTH_SIMULATE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

UINT32 Examing(UINT8 *access_id, UINT8 *access_key, UINT8 *gpen_key, UINT8 *gpen_id);
UINT32 GpenRegister(UINT8 *gpen_id, UINT8 *pub_key);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __AUTH_SIMULATE_H__ */
