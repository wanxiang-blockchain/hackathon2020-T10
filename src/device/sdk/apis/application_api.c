/***********************************************************************************
 * @file   			: application_api.c
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This document implements the application layer interfaces, 
 					  users do not need to care about the protocol layer and 
 					  algorithm layer interfaces.
 * @Copyright   	: Copyright (c) 2008-2020  Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#include "protocol_http.h"
#include "algorithm_api.h"
#include "samurai_client.h"

/*****************************************************************************
 * @Function	: CustomerKeyInit
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : User key structure initialization
 * @param[in]  	: UINT8 *access_id          user's access ID
               	  UINT8 *priv_key           user's private key
 * @param[out]  : ACCESS_KEY *customer_key  Structure pointer used to store 
 											user's private key and access ID 
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE CustomerKeyInit(ACCESS_KEY *customer_key, 
						 UINT8 *access_id, 
						 UINT8 *priv_key)
{
	UINT8 *tmp = NULL;
	
	if((customer_key == NULL) || (access_id == NULL) || (priv_key == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	if((strlen((char *)access_id) >= ACCESS_KEY_LEN) ||
	   (strlen((char *)priv_key) >= ACCESS_KEY_LEN))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	/*private base64 decode*/
	tmp = DataBase64Decode(priv_key, strlen((char *)priv_key));
	if(tmp == NULL)
	{
		return ERR_BASE64_DECODE_FAIL;
	}

	memcpy(customer_key->PrivKey, tmp, PRIVATE_KEY_LEN);
	sprintf((char *)customer_key->AccessId, "%s", access_id);

	if(tmp)
	{
		free(tmp);
		tmp = NULL;
	}

	return ERR_SUCCESS;
}


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
						   POE_SAVE_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_SAVE_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	memset(&poe_param, 0, sizeof(poe_param));
	/*1.Prepare parameter structure*/
	poe_param.Data = data;
	poe_param.DataLen = data_len;
	poe_param.PoeType = POE_TYPE_TEXT_VERIFIABLE;
	poe_param.AccessKey = &customer_key;
	poe_param.HashMethod = HASH_MD5;
	poe_param.SignMethod = SIGN_ECDSA;
	memcpy(poe_param.PriKey, priv_key, sizeof(poe_param.PriKey));

	/*2.PoE save operation	*/
	ret = PoeSave(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}


	return ret;
}


/*****************************************************************************
 * @Function	: TextVerifiPoeQuery
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Text verifiable poe query
 * @param[in]  	: UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
							POE_QUERY_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_QUERY_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));
	poe_param.AccessKey = &customer_key;
	poe_param.PoeType = POE_TYPE_TEXT_VERIFIABLE;
	poe_param.SignMethod = SIGN_ECDSA;
	poe_param.Data = poe_hash;
	poe_param.DataLen = strlen((char *)poe_hash);

	/*2.PoE query operation	*/
	ret = PoeQuery(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	return ret;
}



/*****************************************************************************
 * @Function	: TextHashPoeSave
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Hash the data first, then save the hash value to the 
 				  samurai system.
 * @param[in]  	: UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
						 POE_SAVE_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_SAVE_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));

	poe_param.Data = data;
	poe_param.DataLen = data_len;
	poe_param.PoeType = POE_TYPE_TEXT_HASH;
	poe_param.AccessKey = &customer_key;
	poe_param.HashMethod = HASH_MD5;
	poe_param.SignMethod = SIGN_ECDSA;

	/*2.PoE save operation	*/
	ret = PoeSave(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	return ret;
}

/*****************************************************************************
 * @Function	 : TextHashPoeQuery
 * @author 	 	 : xqchen
 * @date		 : 2020-9-14
 * @brief		 : Text hash poe query
 * @param[in]	 : UINT8 *access_id           user's access ID
               	   UINT8 *access_key          user's private key
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
						  POE_QUERY_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_QUERY_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));
	poe_param.AccessKey = &customer_key;
	poe_param.PoeType = POE_TYPE_TEXT_HASH;
	poe_param.SignMethod = SIGN_ECDSA;
	poe_param.Data = poe_hash;
	poe_param.DataLen = strlen((char *)poe_hash);

	/*2.PoE query operation	*/
	ret = PoeQuery(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	return ret;
}

UINT8 *TextHashPoeQueryTest(UINT8 *access_id, 
							UINT8 *access_key,  
							UINT8 *poe_hash)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_QUERY_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return NULL;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));
	poe_param.AccessKey = &customer_key;
	poe_param.PoeType = POE_TYPE_TEXT_HASH;
	poe_param.SignMethod = SIGN_ECDSA;
	poe_param.Data = poe_hash;
	poe_param.DataLen = strlen((char *)poe_hash);

	/*2.PoE query operation	*/
	return PoeQueryTest(&poe_param);
	
}

/*****************************************************************************
 * @Function	: TextPlainPoeSave
 * @author	  	: xqchen
 * @date		: 2020-9-14
 * @brief 	  	: save the plain data to the samurai system.
 * @param[in]   : UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
						  POE_SAVE_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_SAVE_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));

	poe_param.Data = data;
	poe_param.DataLen = data_len;
	poe_param.PoeType = POE_TYPE_TEXT_PLAIN;
	poe_param.AccessKey = &customer_key;
	poe_param.SignMethod = SIGN_ECDSA;

	/*2.PoE save operation	*/
	ret = PoeSave(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	return ret;
}

/*****************************************************************************
 * @Function	: TextPlainPoeQuery
 * @author	   	: xqchen
 * @date		: 2020-9-14
 * @brief 	   	: Text plain poe query
 * @param[in]   : UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
						   POE_QUERY_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_QUERY_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));
	poe_param.AccessKey = &customer_key;
	poe_param.PoeType = POE_TYPE_TEXT_PLAIN;
	poe_param.SignMethod = SIGN_ECDSA;
	poe_param.Data = poe_hash;
	poe_param.DataLen = strlen((char *)poe_hash);

	/*2.PoE query operation	*/
	ret = PoeQuery(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}
	
	return ret;
}

/*****************************************************************************
 * @Function	: FileVerifiPoeSave
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Hash the data first, then use the user's private key to
                  sign the hash value, and then save the signture value
                  to the samurai system.
 * @param[in]  	: UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
						   POE_SAVE_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_SAVE_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));

	poe_param.Data = file;
	poe_param.DataLen = strlen((char *)file);
	poe_param.PoeType = POE_TYPE_FILE_VERIFIABLE;
	poe_param.AccessKey = &customer_key;
	poe_param.HashMethod = HASH_MD5;
	poe_param.SignMethod = SIGN_ECDSA;
	memcpy(poe_param.PriKey, priv_key, sizeof(poe_param.PriKey));

	/*2.PoE save operation	*/
	ret = PoeSave(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}
	
	return ret;
}

/*****************************************************************************
* @Function    : FileVerifiPoeQuery
* @author	   : xqchen
* @date 	   : 2020-9-14
* @brief	   : File verifiable poe query
* @param[in]   : UINT8 *access_id           user's access ID
               	 UINT8 *access_key          user's private key
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
							POE_QUERY_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_QUERY_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));
	poe_param.AccessKey = &customer_key;
	poe_param.PoeType = POE_TYPE_FILE_VERIFIABLE;
	poe_param.SignMethod = SIGN_ECDSA;
	poe_param.Data = poe_hash;
	poe_param.DataLen = strlen((char *)poe_hash);

	/*2.PoE query operation	*/
	ret = PoeQuery(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	return ret;
}

/*****************************************************************************
 * @Function	: FileHashPoeSave
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief		: Hash the file first, then save the hash value to the 
				  samurai system.
 * @param[in]	: UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
						 POE_SAVE_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_SAVE_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));

	poe_param.Data = file;
	poe_param.DataLen = strlen((char *)file);
	poe_param.PoeType = POE_TYPE_FILE_HASH;
	poe_param.AccessKey = &customer_key;
	poe_param.HashMethod = HASH_MD5;
	poe_param.SignMethod = SIGN_ECDSA;

	/*2.PoE save operation	*/
	ret = PoeSave(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	return ret;
}

 /*****************************************************************************
 * @Function	: FileHashPoeQuery
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief		: File hash poe query
 * @param[in]	: UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
						  POE_QUERY_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_QUERY_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));
	poe_param.AccessKey = &customer_key;
	poe_param.PoeType = POE_TYPE_FILE_HASH;
	poe_param.SignMethod = SIGN_ECDSA;
	poe_param.Data = poe_hash;
	poe_param.DataLen = strlen((char *)poe_hash);

	/*2.PoE query operation	*/
	ret = PoeQuery(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}
	

	return ret;
}

/*****************************************************************************
* @Function	  : FilePlainPoeSave
* @author	  : xqchen
* @date		  : 2020-9-14
* @brief 	  : save the plain data to the samurai system.
* @param[in]  : UINT8 *access_id           user's access ID
               	UINT8 *access_key          user's private key
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
						  POE_SAVE_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_SAVE_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));

	poe_param.Data = file;
	poe_param.DataLen = strlen((char *)file);
	poe_param.PoeType = POE_TYPE_FILE_PLAIN;
	poe_param.AccessKey = &customer_key;
	poe_param.HashMethod = HASH_MD5;
	poe_param.SignMethod = SIGN_ECDSA;

	/*2.PoE save operation	*/
	ret = PoeSave(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	return ret;
}

/*****************************************************************************
 * @Function	: FileVerifiPoeQuery
 * @author 	 	: xqchen
 * @date		: 2020-9-14
 * @brief		: File verifiable poe query
 * @param[in]	: UINT8 *access_id           user's access ID
               	  UINT8 *access_key          user's private key
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
						   POE_QUERY_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	POE_QUERY_PARAM poe_param;
	ACCESS_KEY customer_key;

	memset(&customer_key, 0, sizeof(customer_key));
	ret = CustomerKeyInit(&customer_key, access_id, access_key);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*1.Prepare parameter structure*/
	memset(&poe_param, 0, sizeof(poe_param));
	poe_param.AccessKey = &customer_key;
	poe_param.PoeType = POE_TYPE_FILE_PLAIN;
	poe_param.SignMethod = SIGN_ECDSA;
	poe_param.Data = poe_hash;
	poe_param.DataLen = strlen((char *)poe_hash);

	/*2.PoE query operation	*/
	ret = PoeQuery(&poe_param, respone);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}
		
	return ret;
}

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
void PoeSaveResponeDestroy(POE_SAVE_RESPONE *respone)
{
	if(respone == NULL)
	{
		return;
	}
	if(respone->SaveRetDesc)
	{
		free(respone->SaveRetDesc);
		respone->SaveRetDesc = NULL;
	}
}

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
void PoeQueryResponeDestroy(POE_QUERY_RESPONE *respone)
{
	if(respone == NULL)
	{
		return;
	}

	if(respone->QueryRetDesc)
	{
		free(respone->QueryRetDesc);
		respone->QueryRetDesc = NULL;
	}

	if(respone->PoeContent)
	{
		free(respone->PoeContent);
		respone->PoeContent = NULL;
	}

	if(respone->FileDesc)
	{
		free(respone->FileDesc);
		respone->FileDesc = NULL;
	}

	if(respone->FileOssDownloadPath)
	{
		free(respone->FileOssDownloadPath);
		respone->FileOssDownloadPath = NULL;
	}
}



