/***********************************************************************************
 * @file   			: application_api.h
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @brief   		: This header file mainly defines the data structure,
                      macro and external interface of application layer.
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __APPLICATION_API_H__
#define __APPLICATION_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#include "types.h"

/*****************************************************************************
 * @Function	: CustomerKeyInit
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : User key structure initialization
 * @param[in]  	: UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
 * @param[out]  : ACCESS_KEY *customer_key  Structure pointer used to store 
 											user's private key and access ID 
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE CustomerKeyInit(ACCESS_KEY *customer_key, 
						 UINT8 *access_id, 
						 UINT8 *access_key);

/*****************************************************************************
 * @Function	: TextVerifiPoeSave
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Hash the data first, then use the user's private key to sign 
 				  the hash value, and then save the signture value to the 
 				  samurai system.
 * @param[in]  	: UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
               	  UINT8 *data                The plain text to save
               	  UINT32 data_len            The length of the plain text
 * @param[out]  : POE_SAVE_RESPONE *respone  The result returned by the samurai
               	  							 system.The space must be released 
               	  							 by calling the corresponding 
               	  							 function.
 * @return  	: ERR_CODE
 * @relation  	: PoeSaveResponeDestroy
 * @other  		: 

*****************************************************************************/
ERR_CODE TextVerifiPoeSave(UINT8 *access_id, 
						   UINT8 *access_key, 
						   UINT8 *data, 
						   UINT32 data_len,
						   UINT8 *priv_key, 
						   POE_SAVE_RESPONE *respone);

/*****************************************************************************
 * @Function	: TextVerifiPoeQuery
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Text verifiable poe query
 * @param[in]  	: UINT8 *access_id            user's access ID
               	  UINT8 *access_key           user's private key
               	  UINT8 *poe_hash             The poe hash to be quired
 * @param[out]  : POE_QUERY_RESPONE *respone  The result returned by the samurai
               	  							  system.The space must be released 
               	  							  by calling the corresponding 
               	  							  function.
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE TextVerifiPoeQuery(UINT8 *access_id, 
						    UINT8 *access_key,
							UINT8 *poe_hash, 
							POE_QUERY_RESPONE *respone);

/*****************************************************************************
 * @Function	: TextHashPoeSave
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Hash the data first, then save the hash value to the 
 				  samurai system.
 * @param[in]  	: UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
               	  UINT8 *data                The plain text to save
               	  UINT32 data_len            The length of the plain text
 * @param[out]  : POE_SAVE_RESPONE *respone  The result returned by the samurai
               	  							 system.
               	  							 The space must be released 
               	  							 by calling the corresponding 
               	  							 function.
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE TextHashPoeSave(UINT8 *access_id, 
						 UINT8 *access_key, 
						 UINT8 *data, 
						 UINT32 data_len,
						 POE_SAVE_RESPONE *respone);

/*****************************************************************************
 * @Function	 : TextHashPoeQuery
 * @author 	 	 : xqchen
 * @date		 : 2020-9-14
 * @brief		 : Text hash poe query
 * @param[in]	 : UINT8 *access_id          user's access ID
               	   UINT8 *access_key           user's private key
				   UINT8 *poe_hash			   The poe hash to be quired
 * @param[out]   : POE_QUERY_RESPONE *respone  The result returned by the samurai
											   system.The space must be released 
											   by calling the corresponding 
											   function.
 * @return 	 	 : ERR_CODE
 * @relation	 : 
 * @other		 : 

*****************************************************************************/
ERR_CODE TextHashPoeQuery(UINT8 *access_id, 
						  UINT8 *access_key, 
						  UINT8 *poe_hash,
						  POE_QUERY_RESPONE *respone);

/*****************************************************************************
 * @Function	: TextPlainPoeSave
 * @author	  	: xqchen
 * @date		: 2020-9-14
 * @brief 	  	: save the plain data to the samurai system.
 * @param[in]   : UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
 				  UINT8 *data 			    The plain text to save
 				  UINT32 data_len 		    The length of the plain text, the 
 											size of text data cannot exceed 30K
 * @param[out]  : POE_SAVE_RESPONE *respone The result returned by the samurai
 										    system.
 										    The space must be released 
 										    by calling the corresponding 
 										    function.
 * @return	  	: ERR_CODE
 * @relation	: PoeSaveResponeDestroy
 * @other		: 
 
 *****************************************************************************/
ERR_CODE TextPlainPoeSave(UINT8 *access_id, 
						  UINT8 *access_key, 
						  UINT8 *data, 
						  UINT32 data_len,  
						  POE_SAVE_RESPONE *respone);

/*****************************************************************************
 * @Function	: TextPlainPoeQuery
 * @author	   	: xqchen
 * @date		: 2020-9-14
 * @brief 	   	: Text plain poe query
 * @param[in]   : UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
 				  UINT8 *poe_hash			 The poe hash to be quired
 * @param[out]  : POE_QUERY_RESPONE *respone The result returned by the samurai
 											 system.The space must be released 
 											 by calling the corresponding 
 											 function.
 * @return	   	: ERR_CODE
 * @relation	: 
 * @other 	   	: 
 
 *****************************************************************************/
ERR_CODE TextPlainPoeQuery(UINT8 *access_id, 
						   UINT8 *access_key, 
						   UINT8 *poe_hash,
						   POE_QUERY_RESPONE *respone);

/*****************************************************************************
 * @Function	: FileVerifiPoeSave
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Hash the data first, then use the user's private key to
                  sign the hash value, and then save the signture value
                  to the samurai system.
 * @param[in]  	: UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
               	  UINT8 *file                The plain file to save
               	  POE_SAVE_RESPONE *respone  The result returned by the samurai 
               	  							 system.The space must be released 
               	  							 by calling the corresponding 
               	  							 function.
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE FileVerifiPoeSave(UINT8 *access_id, 
						   UINT8 *access_key, 
						   UINT8 *file, 
						   UINT8 *priv_key, 
						   POE_SAVE_RESPONE *respone);

/*****************************************************************************
* @Function    : FileVerifiPoeQuery
* @author	   : xqchen
* @date 	   : 2020-9-14
* @brief	   : File verifiable poe query
* @param[in]   : UINT8 *access_id          user's access ID
               	 UINT8 *access_key           user's private key
				 UINT8 *poe_hash			 The poe hash to be quired
* @param[out]  : POE_QUERY_RESPONE *respone  The result returned by the samurai
											 system.The space must be released 
											 by calling the corresponding 
											 function.
* @return	   : ERR_CODE
* @relation    : 
* @other	   : 

*****************************************************************************/
ERR_CODE FileVerifiPoeQuery(UINT8 *access_id, 
						    UINT8 *access_key, 
							UINT8 *poe_hash,
							POE_QUERY_RESPONE *respone);

/*****************************************************************************
 * @Function	: FileHashPoeSave
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief		: Hash the file first, then save the hash value to the 
				  samurai system.
 * @param[in]	: UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
				  UINT8 *file				 The file to save
				  POE_SAVE_RESPONE *respone  The result returned by the samurai
											 system.
											 The space must be released 
											 by calling the corresponding 
											 function.
 * @param[out]	: None
 * @return		: ERR_CODE
 * @relation	: 
 * @other		: 
							
 *****************************************************************************/
ERR_CODE FileHashPoeSave(UINT8 *access_id, 
						 UINT8 *access_key, 
						 UINT8 *file,
						 POE_SAVE_RESPONE *respone);

 /*****************************************************************************
 * @Function	: FileHashPoeQuery
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief		: File hash poe query
 * @param[in]	: UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
				  UINT8 *poe_hash			  The poe hash to be quired
 * @param[out]	: POE_QUERY_RESPONE *respone  The result returned by the samurai
											  system.The space must be released 
											  by calling the corresponding 
											  function.
 * @return		: ERR_CODE
 * @relation	: 
 * @other		: 
 
 *****************************************************************************/
ERR_CODE FileHashPoeQuery(UINT8 *access_id, 
						  UINT8 *access_key, 
						  UINT8 *poe_hash,
						  POE_QUERY_RESPONE *respone);

/*****************************************************************************
* @Function	  : FilePlainPoeSave
* @author	  : xqchen
* @date		  : 2020-9-14
* @brief 	  : save the plain data to the samurai system.
* @param[in]  : UINT8 *access_id          user's access ID
               	UINT8 *access_key           user's private key
				UINT8 *file 			  The plain text to save, the 
										  size of text data cannot exceed 30M
* @param[out] : POE_SAVE_RESPONE *respone The result returned by the samurai
										  system.The space must be released 
										  by calling the corresponding function.
* @return	  : ERR_CODE
* @relation	  : PoeSaveResponeDestroy
* @other 	  : 

*****************************************************************************/
ERR_CODE FilePlainPoeSave(UINT8 *access_id, 
						  UINT8 *access_key, 
						  UINT8 *file,
						  POE_SAVE_RESPONE *respone);

/*****************************************************************************
 * @Function	: FileVerifiPoeQuery
 * @author 	 	: xqchen
 * @date		: 2020-9-14
 * @brief		: File verifiable poe query
 * @param[in]	: UINT8 *access_id          user's access ID
               	  UINT8 *access_key           user's private key
 			   	  UINT8 *poe_hash			  The poe hash to be quired
 * @param[out]  : POE_QUERY_RESPONE *respone  The result returned by the samurai
 										   	  system.The space must be released 
 										   	  by calling the corresponding 
 										      function.
 * @return 	 	: ERR_CODE
 * @relation	: 
 * @other		: 

*****************************************************************************/
ERR_CODE FilePlainPoeQuery(UINT8 *access_id, 
						   UINT8 *access_key, 
						   UINT8 *poe_hash,
						   POE_QUERY_RESPONE *respone);

/*****************************************************************************
 * @Function	: PoeSaveResponeDestroy
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief       : free the poe save respone space
 * @param[in]  	: POE_SAVE_RESPONE *respone  The result returned by the samurai
 										   	 system.
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void PoeSaveResponeDestroy(POE_SAVE_RESPONE *respone);

/*****************************************************************************
 * @Function	: PoeQueryResponeDestroy
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief       : free the poe query respone space
 * @param[in]  	: POE_SAVE_RESPONE *respone  The result returned by the samurai
 										   	 system.
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void PoeQueryResponeDestroy(POE_QUERY_RESPONE *respone);

UINT8 *TextHashPoeQueryTest(UINT8 *access_id, 
							UINT8 *access_key,  
							UINT8 *poe_hash);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif



