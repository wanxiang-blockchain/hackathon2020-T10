/***********************************************************************************
 * @file   			: protocol_http.h
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This header file mainly defines the data structure,
                      macro and external interface of protocol layer.
                     
 * @Copyright   	: Copyright (c) 2008-2020  Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __PROTOCOL_HTTP_H__
#define __PROTOCOL_HTTP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "samurai_client.h"

/*****************************************************************************
 * @Function	: PoeSave
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief       : poe save
 * @param[in]  	: POE_SAVE_PARAM *poe_param    The parameters of poe save
               	  POE_SAVE_RESPONE *respone    The poe save respone
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE PoeSave(POE_SAVE_PARAM *poe_param, POE_SAVE_RESPONE *respone);

/*****************************************************************************
 * @Function	: PoeQuery
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief       : poe save
 * @param[in]  	: POE_QUERY_PARAM *poe_param   	The parameters of poe query
               	  POE_QUERY_RESPONE *respone 	The poe query respone
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE PoeQuery(POE_QUERY_PARAM *poe_param, POE_QUERY_RESPONE *respone);

UINT8 *PoeQueryTest(POE_QUERY_PARAM *poe_param);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif



