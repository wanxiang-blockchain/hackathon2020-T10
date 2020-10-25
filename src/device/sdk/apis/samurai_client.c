/***********************************************************************************
 * @file   			: samurai_client.c
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This document mainly implements data encapsulation,
                      including data hash calculation and data signature,
                      so as to form a standard data structure of poe save
                      query. The corresponding interface is generally called
                      by protocol layer. 
                      External interfaces:
                      PoeSaveDataDestroy
                      PoeSaveDataCreate
                      PoeQueryDataCreate
 * @Copyright   	: Copyright (c) 2008-2020  Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>


#include "types.h"
#include "samurai_client.h"
#include "cJSON.h"
#include "json_define.h"
#include "hexutils.h"
#include "algorithm_api.h"

static UINT8 *g_context_str[] = {	
									(UINT8 *)JSON_POEHASH_STR, 
								  	(UINT8 *)JSON_CONTEXT_STR, 
								  	NULL
								 };

/*****************************************************************************
 * @Function	: get_timestamp
 * @author		: xqchen
 * @date		: 2020-8-10
 * @brief       : get the timestap
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: UINT64
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT64 get_timestamp(void)
{
	struct timeval tv;
	UINT64 tv_sec = 0;
	UINT64 tv_usec = 0;
	UINT64 timestamp = 0;

    gettimeofday(&tv, NULL);
	tv_sec = tv.tv_sec;
	tv_usec = tv.tv_usec;
	
	timestamp = tv_sec * 1000 + tv_usec / 1000;
	
    DEBUG_PRINT("get_timestamp:%" PRId64 "\r\n", timestamp);
	return timestamp;
}

/*****************************************************************************
 * @Function	: signature_data_base64_encode
 * @author		: Preh
 * @date		: 2020-8-10
 * @brief       : encoding signature data of json format by Base64
 * @param[in]  	: UINT8* sig_data  signature data of files or contexts
 * @param[out]  : None
 * @return  	: encoded signature data of json format by Base64
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT8 *signature_data_base64_encode(UINT8* sig_data)
{
	UINT8 *rout = NULL;
	UINT8 *sout = NULL;
	UINT8 *allout = NULL;
	UINT8 *json_str = NULL;
	cJSON *root= NULL;

	if(sig_data == NULL)
	{
		return NULL;
	}
	
	/*1.encoding R part of signature data by Base64*/
	rout = DataBase64Encode(sig_data, 32);
	if(rout == NULL)
	{
		return NULL;
	}


	/*2.encoding S part of signature data by Base64*/
	sout = DataBase64Encode(sig_data+32, 32);
	if(rout == NULL)
	{
		return NULL;
	}

	/*3.three parts of signature data encapsulated into JSON data format*/
	root = cJSON_CreateObject();	
    if(root == NULL)
    {
        return NULL;
    }

	cJSON_AddItemToObject(root, "r", cJSON_CreateString((char *)rout));
    cJSON_AddItemToObject(root, "s", cJSON_CreateString((char *)sout));
	/*In Ethereum, the value of v is recid + 27*/
	cJSON_AddItemToObject(root, "v", cJSON_CreateNumber(sig_data[64]+27));

    /*4.Avoid converting characters such as Spaces to JSON format strings*/
    json_str = (UINT8 *)cJSON_PrintUnformatted(root);
    if(json_str == NULL)
    {
        cJSON_Delete(root);	
        return NULL;
    }
	DEBUG_PRINT("json_str:%s\r\n", json_str);

	/*5.encoding above-mentioned JSON data */ 
	allout = DataBase64Encode(json_str, strlen((char *)json_str));
	

	/*free temporary malloc space*/
	if(rout)
	{
		free(rout);
		rout = NULL;
	}

	if(sout)
	{
		free(sout);
		sout = NULL;
	}

	if(json_str)
	{
		free(json_str);
		json_str = NULL;
	}

	cJSON_Delete(root);	
	return allout;

}


/*****************************************************************************
 * @Function	: assemble_data_to_signed
 * @author		: Preh
 * @date		: 2020-8-10
 * @brief       : joint signed data with time stamp
 * @param[in]  	: INT64 timestamp        time stamp
               	  INT8* data             signed data of files or contents
               	  INT16 data_len         length of signed data
               	  POE_METHOD method      joint method
 * @param[out]  : UINT32 *assemble_data_len
 * @return  	: UINT8
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT8 *assemble_data_to_signed(UINT8 *timestamp, 
									  UINT8* data, 
							   		  UINT32 data_len, 	
							   		  POE_METHOD method,
							   		  UINT32 *assemble_data_len)
{
	UINT32 total_len;
	UINT32 tmp_len;
	UINT32 timestamp_str_len;
	UINT32 timestamp_len;
	UINT32 context_len;
	UINT8 *sign_data = NULL;
	

	if((data == NULL) || ((method != POE_METHOD_QUERY) 
		&& (method != POE_METHOD_SAVE)))
	{
		return NULL;
	}
	
	/*1.Calculate the data length after assembly*/
	timestamp_str_len 	= strlen((char *)JSON_TIMESTAP_STR);
	timestamp_len 		= strlen((char *)timestamp);
	context_len 		= strlen((char *)g_context_str[method]);
	total_len = timestamp_str_len + timestamp_len + context_len + data_len;

	/*2.malloc the data space after assembly*/		   
	sign_data = malloc(total_len + 1);
	if(sign_data == NULL)
	{
		return NULL;
	}
	memset(sign_data, 0, total_len + 1);

	/*3.assemble the data*/
	tmp_len = 0;
	memcpy(sign_data + tmp_len, JSON_TIMESTAP_STR, timestamp_str_len);
	tmp_len = tmp_len + timestamp_str_len;
	memcpy(sign_data + tmp_len, timestamp, timestamp_len);
	tmp_len = tmp_len + timestamp_len;
	memcpy(sign_data + tmp_len, g_context_str[method], context_len);
	tmp_len = tmp_len + context_len;
	memcpy(sign_data + tmp_len, data, data_len);
	tmp_len = tmp_len + data_len;
	*assemble_data_len = tmp_len;
	//sprintf((char *)sign_data, "%s%s%s%s", JSON_TIMESTAP_STR, 
	//		timestamp, g_context_str[method], data);
	
	return sign_data;
}


/*****************************************************************************
 * @Function	: data_signature
 * @author		: Preh
 * @date		: 2020-8-12
 * @brief       : Calculate the signature value of the data, using the 
 				  spec-ified signature algorithm
 * @param[in]  	: UINT8 *data            data to be signed
               	  UINT32 data_len        data length
               	  SIGN_TYPE sign_method  signature algorithm
               	  UINT8 *pri_key         signature private key
               	  UINT64 timestamp       the timestamp when function is executed
 * @param[out]  : None
 * @return  	: encoded the signature data by base64
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT8 *data_signature(UINT8 *data, 
							 UINT32 data_len, 
							 SIGN_TYPE sign_method, 
					  		 UINT8 *pri_key, 
					  		 UINT64 timestamp, 
					  		 POE_METHOD method)
{
	ERR_CODE ret = ERR_SUCCESS;
	UINT8 signed_msg[65];
	UINT8 *encode_msg = NULL;
	UINT8 timestamp_str[32];
	UINT8 *assemble_data = NULL;
	UINT32 assemble_data_len = 0;
	
    if((data == NULL) || (pri_key == NULL))
	{
		return NULL;
	}

	/*1.convert timestamp to string*/
	memset(timestamp_str, 0u, sizeof(timestamp_str));
	sprintf((char *)timestamp_str, "%" PRId64 "", timestamp);

	/*2.assemble the data to signature*/
	assemble_data = assemble_data_to_signed(timestamp_str, 
											data, 
											data_len,
											method, 
											&assemble_data_len);
	if(assemble_data == NULL)
	{
		/*ERR_DATA_ASSEMBLE_FAIL*/
		return NULL;
	}

	ret = DataSignature(assemble_data, 
						assemble_data_len, 
						sign_method, 
						pri_key, 
						signed_msg);
	if(ret != ERR_SUCCESS)
	{
		return NULL;
	}

	/*4.the signature data base64 encode*/
	encode_msg = signature_data_base64_encode(signed_msg);
	if(encode_msg == NULL)
	{
		return NULL;
	}

	if(assemble_data)
	{
		free(assemble_data);
		assemble_data = NULL;
	}

	return encode_msg;
}

/*****************************************************************************
 * @Function	: data_create_plain_save
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief       : This function implements plain PoE save. It will sign the data 
 				  (if text data, it is the original data; if file, it is the   
 				  hash value of the file content) together with the time stamp, 
 				  and generate a standard data structure according to the 
 				  parameters.
 * @param[in]  	: POE_SAVE_PARAM *poe_param  The parameters of poe save
               	  POE_SAVE_DATA *poe_data    poe save data structure
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE data_create_plain_save(POE_SAVE_PARAM *poe_param, 
									   POE_SAVE_DATA *poe_data)
{
	UINT64 timestamp = 0;
	UINT8 *encode_msg = NULL;
	UINT32 content_len = 0;
	UINT8 hash[32];
	UINT8 hash_str[65];
	UINT32 hash_len = 0;

	/*1.Get the current timestamp*/
	timestamp = get_timestamp();
	
	if(poe_param->PoeType == POE_TYPE_TEXT_PLAIN)
	{
		/*2.The data is signed together with the time 	*/
		/*  stamp, and result will be encoded by Base64	*/
		encode_msg = data_signature(poe_param->Data, 
									poe_param->DataLen, 
									poe_param->SignMethod, 
									poe_param->AccessKey->PrivKey, 
									timestamp, 
									POE_METHOD_SAVE);
	}
	else if(poe_param->PoeType == POE_TYPE_FILE_PLAIN)
	{
		/*2.Calculates the hash value of the file content*/
		memset(hash, 0, sizeof(hash));
		hash_len = FileHash(poe_param->Data, poe_param->HashMethod, hash);
		if(hash_len == 0)
		{
			return ERR_HASH_FAIL;
		}

		/*3.The hash value is signed together with the  	*/
		/*  timestamp, and result will be encoded by Base64	*/
		memset(hash_str, 0, sizeof(hash_str));
		HexToStr((char *)hash_str, hash, hash_len);
		encode_msg = data_signature(hash_str, 
									strlen((char *)hash_str),  
									poe_param->SignMethod,
									poe_param->AccessKey->PrivKey, 
									timestamp, 
									POE_METHOD_SAVE);
	}
	else
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}
	if(encode_msg == NULL)
	{
		return ERR_SIGN_FAIL;
	}

	/*4.Generate a standard poe save data structure.*/
	content_len = poe_param->DataLen;
	poe_data->Content = malloc(content_len + 1);
	if(poe_data->Content == NULL)
	{
		return ERR_MALLOC_FAIL;
	}
	memset(poe_data->Content, 0, content_len + 1);
	memcpy(poe_data->Content, poe_param->Data, poe_param->DataLen);
	poe_data->TimeStamp = timestamp;
	poe_data->PoeType = poe_param->PoeType;
	memcpy(poe_data->Sign, encode_msg, strlen((char *)encode_msg));
	memcpy(poe_data->AccessKeyId, 
		   poe_param->AccessKey->AccessId, 
		   strlen((char *)poe_param->AccessKey->AccessId));
	
	/*5.free temporary space*/
	if(encode_msg)
	{
		free(encode_msg);
		encode_msg = NULL;
	}
	return ERR_SUCCESS;
}

/*****************************************************************************
 * @Function	: data_create_hash_save
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief		: This function implements hash PoE save. First hash the text or 
 				  file, and then sign the hash value with the timestamp, and 
 				  generate a standard data structure according to the parameters.
 * @param[in]	: POE_SAVE_PARAM *poe_param  The parameters of poe save
				  POE_SAVE_DATA *poe_data	 poe save data structure
 * @param[out]	: None
 * @return		: ERR_CODE
 * @relation	: 
 * @other		: 

*****************************************************************************/
static ERR_CODE data_create_hash_save(POE_SAVE_PARAM *poe_param, 
							   		  POE_SAVE_DATA *poe_data)
{
	UINT64 timestamp = 0;
	UINT8 hash[32];
	UINT8 hash_str[65];
	UINT8 *encode_msg = NULL;
	UINT32 hash_len = 0;
	UINT32 content_len = 0;

	/*1.Calculate text data hash*/
	memset(hash, 0, sizeof(hash));
	if(poe_param->PoeType == POE_TYPE_TEXT_HASH)
	{
		hash_len = DataHash(poe_param->Data, poe_param->DataLen, 
							poe_param->HashMethod, hash);
	}
	else if(poe_param->PoeType == POE_TYPE_FILE_HASH)
	{
		hash_len = FileHash(poe_param->Data, poe_param->HashMethod, hash);
	}
	else
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	if(hash_len == 0)
	{
		return ERR_HASH_FAIL;
	}
	memset(hash_str, 0, sizeof(hash_str));
	HexToStr((char *)hash_str, hash, hash_len);
	
	/*2.Get the current timestamp*/
	timestamp = get_timestamp();
	/*3.data signature*/
	encode_msg = data_signature(hash_str, 
								strlen((char *)hash_str),  
								poe_param->SignMethod,
								poe_param->AccessKey->PrivKey, 
								timestamp, 
								POE_METHOD_SAVE);
	if(encode_msg == NULL)
	{
		return ERR_SIGN_FAIL;
	}

	/*4.Generate a standard poe save data structure.*/
	content_len = strlen((char *)hash_str);
	poe_data->Content = malloc(content_len + 1);
	if(poe_data->Content == NULL)
	{
		return ERR_MALLOC_FAIL;
	}
	memset(poe_data->Content, 0, content_len + 1);
	memcpy(poe_data->Content, hash_str, content_len);
	poe_data->HashAlgorithm = poe_param->HashMethod;
	poe_data->TimeStamp = timestamp;
	memcpy(poe_data->Sign, encode_msg, strlen((char *)encode_msg));
	poe_data->PoeType = poe_param->PoeType;
	memcpy(poe_data->AccessKeyId, 
		   poe_param->AccessKey->AccessId, 
		   strlen((char *)poe_param->AccessKey->AccessId));
	
	/*5.free temporary space*/
	if(encode_msg)
	{
		free(encode_msg);
		encode_msg = NULL;
	}
	return ERR_SUCCESS;
}

/*****************************************************************************
* @Function    : data_create_verif_save
* @author	   : xqchen
* @date 	   : 2020-9-15
* @brief	   : This function implements verifiable PoE save. First hash the  
 				 text or file, and then sign the hash value with the timestamp,  
 				 and generate a standard data structure according to the 
 				 parameters.
* @param[in]   : POE_SAVE_PARAM *poe_param	The parameters of poe save
				 POE_SAVE_DATA *poe_data	poe save data structure
* @param[out]  : None
* @return	   : ERR_CODE
* @relation    : 
* @other	   : 

*****************************************************************************/
static ERR_CODE data_create_verif_save(POE_SAVE_PARAM *poe_param, 
									   POE_SAVE_DATA *poe_data)
{
	ERR_CODE ret = ERR_SUCCESS;
	UINT64 timestamp = 0;
	UINT8 hash[32];
	UINT8 hash_str[65];
	UINT8 *encode_msg = NULL;
	UINT32 hash_len = 0;
	UINT32 content_len = 0;
	UINT8 signed_msg[65];
	UINT8 *tmp = NULL;

	/*1.Calculate text data hash*/
	memset(hash, 0, sizeof(hash));
	if(poe_param->PoeType == POE_TYPE_TEXT_VERIFIABLE)
	{
		hash_len = DataHash(poe_param->Data, poe_param->DataLen, 
							poe_param->HashMethod, hash);
	}
	else if(poe_param->PoeType == POE_TYPE_FILE_VERIFIABLE)
	{
		hash_len = FileHash(poe_param->Data, poe_param->HashMethod, hash);
	}
	else
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	if(hash_len == 0)
	{
		return ERR_HASH_FAIL;
	}
	memset(hash_str, 0, sizeof(hash_str));
	HexToStr((char *)hash_str, hash, hash_len);

	/*2.Get the current timestamp and convert to string*/
	timestamp = get_timestamp();

	
	/*3.Calculate text data hash signature*/
	memset(signed_msg, 0, sizeof(signed_msg));
	ret = DataSignature(hash_str, 
						strlen((char *)hash_str), 
						poe_param->SignMethod, 
				  		poe_param->PriKey, 
				  		signed_msg);
	if(ret != ERR_SUCCESS)
	{
		return ERR_SIGN_FAIL;
	}

	/*4.the signature data base64 encode*/
	poe_data->Content = signature_data_base64_encode(signed_msg);
	if(poe_data->Content == NULL)
	{
		return ERR_BASE64_ENCODE_FAIL;
	}

	/*4.data content create*/
	content_len = strlen((char *)poe_data->Content);			
	
	/*5.Calculate package data signature*/
	free(tmp);
	encode_msg = data_signature(poe_data->Content, 
								content_len, 
								poe_param->SignMethod, 
								poe_param->AccessKey->PrivKey, 
								timestamp, 
								POE_METHOD_SAVE);
	if(encode_msg == NULL)
	{
		return ERR_SIGN_FAIL;
	}

	memcpy(poe_data->RawHash, hash, hash_len);
	poe_data->HashLen = hash_len;
	poe_data->HashAlgorithm = poe_param->HashMethod;
	poe_data->SignAlgorithm = poe_param->SignMethod;

	/*6.out data struct package*/
	poe_data->TimeStamp = timestamp;
	memcpy(poe_data->Sign, encode_msg, strlen((char *)encode_msg));
	poe_data->PoeType = poe_param->PoeType;
	memcpy(poe_data->AccessKeyId, 
		   poe_param->AccessKey->AccessId, 
		   strlen((char *)poe_param->AccessKey->AccessId));

	/*7.free temporary space*/
	if(encode_msg)
	{
		free(encode_msg);
		encode_msg = NULL;
	}

	return ERR_SUCCESS;
}

/*****************************************************************************
 * @Function	: PoeSaveDataDestroy
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief       : Free the space of poe save data, whitch was created by 
 				  PoeSaveDataCreate
 * @param[in]  	: POE_SAVE_DATA *poe_data  poe save data structure
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: PoeSaveDataCreate
 * @other  		: 

*****************************************************************************/
ERR_CODE PoeSaveDataDestroy(POE_SAVE_DATA *poe_data)
{
	ERR_CODE ret = ERR_SUCCESS;
	
	if(poe_data == NULL)
	{
		return ERR_SUCCESS;
	}

	if(poe_data->Content)
	{
		free(poe_data->Content);
		poe_data->Content = NULL;
	}

	if(poe_data->Description)
	{
		free(poe_data->Description);
		poe_data->Description = NULL;
	}

	return ret;
}

/*****************************************************************************
* @Function    : PoeSaveDataCreate
* @author	   : xqchen
* @date 	   : 2020-9-15
* @brief	   : Generate a standard poe save data structure according to the 
				 parameters. This function will process the data provided by  
				 the user according to the PoE type, such as hash calculation, 
				 encryption, signature, etc.
* @param[in]   : POE_SAVE_PARAM *poe_param	The parameters of poe save
				 POE_SAVE_DATA *poe_data	poe save data structure
* @param[out]  : None
* @return	   : ERR_CODE
* @relation    : 
* @other	   : 

*****************************************************************************/
ERR_CODE PoeSaveDataCreate(POE_SAVE_PARAM *poe_param, POE_SAVE_DATA *poe_data)
{
	ERR_CODE ret = ERR_SUCCESS;

	if(poe_param == NULL || poe_data == NULL)
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}
	
	switch(poe_param->PoeType)
	{
		case POE_TYPE_TEXT_PLAIN:
		case POE_TYPE_TEXT_CIPHER:
		case POE_TYPE_TEXT_CIPHER_SHARING:
		case POE_TYPE_FILE_PLAIN:
			ret = data_create_plain_save(poe_param, poe_data);
			break;
		case POE_TYPE_TEXT_HASH:
		case POE_TYPE_FILE_HASH:
			ret = data_create_hash_save(poe_param, poe_data);
			break;
		case POE_TYPE_TEXT_VERIFIABLE:
		case POE_TYPE_FILE_VERIFIABLE:
			ret = data_create_verif_save(poe_param, poe_data);
			break;
		default:
			ret = ERR_POE_TYPE_NOT_SUPPORT;
			break;
	}

	return ret;
}

/*****************************************************************************
* @Function    : PoeQueryDataCreate
* @author	   : xqchen
* @date 	   : 2020-9-15
* @brief	   : Generate a standard poe query data structure according to the 
				 parameters. This function will process the data provided by  
				 the user according to the PoE type, such as hash calculation, 
				 encryption, signature, etc.
* @param[in]   : POE_QUERY_PARAM *poe_param	The parameters of poe query
				 POE_QUERY_DATA  *poe_data	poe query data structure
* @param[out]  : None
* @return	   : ERR_CODE
* @relation    : 
* @other	   : 

*****************************************************************************/
ERR_CODE PoeQueryDataCreate(POE_QUERY_PARAM *poe_param, 
							POE_QUERY_DATA *poe_data)
{
	UINT64 timestamp = 0;
	UINT8 *encode_msg = NULL;
	
	if((poe_param == NULL) || (poe_data == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	/*1.Get the current timestamp and convert to string*/
	timestamp = get_timestamp();

	/*2.data signature*/
	encode_msg = data_signature(poe_param->Data, 
								poe_param->DataLen, 
								poe_param->SignMethod, 
								poe_param->AccessKey->PrivKey, 
								timestamp, 
								POE_METHOD_QUERY);
	if(encode_msg == NULL)
	{
		return ERR_SIGN_FAIL;
	}

	/*3.out data struct package*/
	poe_data->TimeStamp = timestamp;
	memcpy(poe_data->Sign, encode_msg, strlen((char *)encode_msg));
	poe_data->PoeType = poe_param->PoeType;
	memcpy(poe_data->AccessKeyId, 
		   poe_param->AccessKey->AccessId, 
		   strlen((char *)poe_param->AccessKey->AccessId));
	poe_data->HashLen = StrToHex(poe_data->PoeHash, 
								(char *)poe_param->Data, 
								poe_param->DataLen/2);
	//poe_data->HashLen = poe_param->DataLen/2;
		   
	/*4.free temporary space*/
	if(encode_msg)
	{
		free(encode_msg);
		encode_msg = NULL;
	}
	return ERR_SUCCESS;
}

