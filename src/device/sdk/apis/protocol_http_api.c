/***********************************************************************************
 * @file   			: protocol_http_api.c
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This file mainly implements the communication with the 
 					  server and obtains the response results.The parameters 
 					  provided by users are encapsulated in the URL or body 
 					  of HTTP protocol.If need to support other protocols (such  
 					  as mqtt, etc.), we can refer to this file to realize the 
 					  external interface:
                      PoeSave
                      PoeQuery
 * @Copyright   	: Copyright (c) 2008-2020  Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "cJSON.h"
#include "http_client.h"
#include "samurai_client.h"
#include "hexutils.h"
#include "json_define.h"
#include "samurai_url_define.h"


#define HASH_TYPE_MD5               "MD5"
#define HASH_TYPE_SHA256            "SHA-256"
#define HASH_TYPE_SHA3K             "SHA3K"
#define SIGN_TYPE_ECDSA            	"ECDSA"

static UINT8 *g_poetype_str[] = {	(UINT8 *)"TEXT_PLAIN", 
									(UINT8 *)"TEXT_CIPHER", 
									(UINT8 *)"TEXT_SHARING", 
									(UINT8 *)"HASH",
									(UINT8 *)"VERIFIABLE",
									(UINT8 *)"FILE",
									(UINT8 *)"HASH",
									(UINT8 *)"VERIFIABLE",
									(UINT8 *)"UNKNOWN",
									NULL
								 };

static UINT8 *g_hash_type_str[] = {
									 (UINT8 *)HASH_TYPE_MD5, 
									 (UINT8 *)HASH_TYPE_SHA256, 
									 (UINT8 *)HASH_TYPE_SHA3K, 
									 (UINT8 *)"UNKNOWN",
									 NULL
								  };

static UINT8 *g_sign_type_str[] =  {
									 (UINT8 *)SIGN_TYPE_ECDSA, 
									 (UINT8 *)"UNKNOWN",
									 NULL
								   };


/*****************************************************************************
 * @Function	: poetype_to_string
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Convert Poe type to string according to the enum value
 * @param[in]  	: POE_TYPE type  Poe enum value
 * @param[out]  : None
 * @return  	: Poe type string
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT8 *poetype_to_string(POE_TYPE type)
{
	return g_poetype_str[type];
}

/*****************************************************************************
 * @Function	: poetype_to_int
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Convert Poe type to enum value according to the string
 * @param[in]  	: UINT8 *type_str  Poe type string
 * @param[out]  : None
 * @return  	: POE_TYPE Poe type enum value
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static POE_TYPE poetype_to_int(UINT8 *type_str)
{
	UINT16 i = 0;

	while(g_poetype_str[i] != NULL)
	{
		if (strcmp((char *)g_poetype_str[i], (char *)type_str) == 0)
		{
			return i;
		}
		i++;
	};

	return POE_TYPE_UNKNOWN;
}

/*****************************************************************************
 * @Function	: hash_type_to_string
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Convert hash type to string according to the enum value
 * @param[in]  	: HASH_TYPE type  hash enum value
 * @param[out]  : None
 * @return  	: hash type string
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT8 *hash_type_to_string(HASH_TYPE type)
{
	return g_hash_type_str[type];
}

/*****************************************************************************
 * @Function	: hash_type_to_int
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Convert hash type to enum value according to the string
 * @param[in]  	: UINT8 *type_str  hash type string
 * @param[out]  : None
 * @return  	: HASH_TYPE hash type enum value
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static HASH_TYPE hash_type_to_int(UINT8 *type_str)
{
	UINT16 i = 0;

	while(g_hash_type_str[i] != NULL)
	{
		if (strcmp((char *)g_hash_type_str[i], (char *)type_str) == 0)
		{
			return i;
		}
		i++;
	};

	return HASH_TYPE_UNKNOWN;
}

/*****************************************************************************
 * @Function	: sign_type_to_string
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Convert sign type to string according to the enum value
 * @param[in]  	: SIGN_TYPE type  sign enum value
 * @param[out]  : None
 * @return  	: sign type string
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT8 *sign_type_to_string(SIGN_TYPE type)
{
	return g_sign_type_str[type];
}

/*****************************************************************************
 * @Function	: sign_type_to_int
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Convert sign type to enum value according to the string
 * @param[in]  	: UINT8 *type_str  sign type string
 * @param[out]  : None
 * @return  	: SIGN_TYPE sign type enum value
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static SIGN_TYPE sign_type_to_int(UINT8 *type_str)
{
	UINT16 i = 0;

	while(g_sign_type_str[i] != NULL)
	{
		if (strcmp((char *)g_sign_type_str[i], (char *)type_str) == 0)
		{
			return i;
		}
		i++;
	};

	return SIGN_TYPE_UNKNOWN;
}

/*****************************************************************************
 * @Function	: get_query_url
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Get the sub URL according to the Poe type and method
 * @param[in]  	: POE_TYPE type      Poe type
               	  POE_METHOD method  poe method get/save
 * @param[out]  : None
 * @return  	: UINT8
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT8 *get_query_url(POE_TYPE type, POE_METHOD method)
{
	if(method == POE_METHOD_SAVE)
	{
		switch (type)
		{
			case POE_TYPE_TEXT_PLAIN:
			case POE_TYPE_TEXT_CIPHER:
				return (UINT8 *)TEXT_ONCHAIN_URL;
			case POE_TYPE_TEXT_CIPHER_SHARING:
				return (UINT8 *)TEXT_SHARE_ONCHAIN_URL;
			case POE_TYPE_TEXT_HASH:
			case POE_TYPE_FILE_HASH:
				return (UINT8 *)HASH_ONCHAIN_URL;
			case POE_TYPE_TEXT_VERIFIABLE:
			case POE_TYPE_FILE_VERIFIABLE:
				return (UINT8 *)VERIFY_ONCHAIN_URL;
			case POE_TYPE_FILE_PLAIN:
				return (UINT8 *)FILE_ONCHAIN_URL;
			default:
				break;
		}
	}
	else if(method == POE_METHOD_QUERY)
	{
		switch (type)
		{
			case POE_TYPE_TEXT_PLAIN:
			case POE_TYPE_TEXT_CIPHER:
			case POE_TYPE_TEXT_CIPHER_SHARING:
				return (UINT8 *)TEXT_GETCHAIN_URL;
			case POE_TYPE_TEXT_HASH:
			case POE_TYPE_FILE_HASH:
				return (UINT8 *)HASH_GETCHAIN_URL;
			case POE_TYPE_TEXT_VERIFIABLE:
			case POE_TYPE_FILE_VERIFIABLE:
				return (UINT8 *)VERIFY_GETCHAIN_URL;
			case POE_TYPE_FILE_PLAIN:
				return (UINT8 *)FILE_GETCHAIN_URL;
			default:
				break;
		}
	}
	return NULL;
}

static ERR_CODE plain_respone_parse(cJSON *array, POE_QUERY_RESPONE *respone)
{
	cJSON *tmp = NULL;

	if((array == NULL) || (respone == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	tmp = cJSON_GetObjectItem(array, JSON_TEXT_CONTENT_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		respone->PoeContent = malloc(strlen(tmp->valuestring) + 1);
		if(respone->PoeContent == NULL)
		{
			return ERR_MALLOC_FAIL;
		}
		memset(respone->PoeContent, 0, strlen(tmp->valuestring) + 1);
		sprintf((char *)respone->PoeContent, "%s", tmp->valuestring);
		DEBUG_PRINT("%s:%s\r\n", 
					JSON_TEXT_CONTENT_STR, 
					(char *)respone->PoeContent);
	}

	return ERR_SUCCESS;
}

/*****************************************************************************
 * @Function	: verifiable_result_parse
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : parse the verifiable poe query result
 * @param[in]  	: cJSON *array                    		JSON item array
               	  VERIFIABLE_QUERY_RESULT *result_data  Query result data of 
               	  										verifiable poe
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE verifiable_respone_parse(cJSON *array, 
										 		POE_QUERY_RESPONE *respone)
{
	cJSON *tmp = NULL;

	if((array == NULL) || (respone == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	tmp = cJSON_GetObjectItem(array, JSON_ALG_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_ALG_STR, tmp->valuestring);
		respone->HashMethod = hash_type_to_int((UINT8 *)tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_SIGN_ALG_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_SIGN_ALG_STR, tmp->valuestring);
		respone->SignMethod = sign_type_to_int((UINT8 *)tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_HASH_SIGN_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_HASH_SIGN_STR, tmp->valuestring);
		respone->PoeContent = malloc(strlen(tmp->valuestring) + 1);
		if(respone->PoeContent == NULL)
		{
			return ERR_MALLOC_FAIL;
		}
		sprintf((char *)respone->PoeContent, "%s", tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_RAW_HASH);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		if(strlen(tmp->valuestring) > (HASH_DATA_LEN * 2))
		{
			return ERR_PARAMETER_CHECK_FAILED;
		}
		respone->RawHashLen = StrToHex(respone->RawHash, tmp->valuestring, 
				 					   strlen(tmp->valuestring)/2);
		DEBUG_PRINT("%s:%s\r\n", JSON_RAW_HASH, tmp->valuestring);
	}

	return ERR_SUCCESS;
}
								 
/*****************************************************************************
 * @Function	: file_result_parse
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : parse the file poe query result
 * @param[in]  	: cJSON *array                    JSON item array
               	  FILE_QUERY_RESULT *result_data  Query result data of file poe
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE file_respone_parse(cJSON *array, POE_QUERY_RESPONE *respone)
{
	cJSON *tmp = NULL;

	if((array == NULL) || (respone == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	tmp = cJSON_GetObjectItem(array, JSON_ALG_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_ALG_STR, tmp->valuestring);
		respone->HashMethod = hash_type_to_int((UINT8 *)tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_FILENAME_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_FILENAME_STR, tmp->valuestring);
		respone->PoeContent = malloc(strlen(tmp->valuestring) + 1);
		if(respone->PoeContent == NULL)
		{
			return ERR_MALLOC_FAIL;
		}
		sprintf((char *)respone->PoeContent, "%s", tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_FILEDESP_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_FILEDESP_STR, tmp->valuestring);
		respone->FileDesc = malloc(strlen(tmp->valuestring) + 1);
		if(respone->FileDesc == NULL)
		{
			return ERR_MALLOC_FAIL;
		}
		sprintf((char *)respone->FileDesc, "%s", tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_FILE_DOWNLOAD_PATH_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_FILE_DOWNLOAD_PATH_STR, tmp->valuestring);
		respone->FileOssDownloadPath = 
			malloc(strlen(tmp->valuestring) + 1);
		if(respone->FileOssDownloadPath == NULL)
		{
			return ERR_MALLOC_FAIL;
		}
		sprintf((char *)respone->FileOssDownloadPath, 
				"%s", tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_FILE_HASH);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		if(strlen(tmp->valuestring) > (HASH_DATA_LEN * 2))
		{
			return ERR_PARAMETER_CHECK_FAILED;
		}
		respone->RawHashLen = StrToHex(respone->RawHash, tmp->valuestring, 
				 					   strlen(tmp->valuestring)/2);
		//respone->RawHashLen = strlen(tmp->valuestring)/2;
		DEBUG_PRINT("%s:%s\r\n", JSON_FILE_HASH, tmp->valuestring);
	}
	return ERR_SUCCESS;
}
/*****************************************************************************
 * @Function	: hash_result_parse
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : parse the hash poe query result
 * @param[in]  	: cJSON *array                    JSON item array
               	  HASH_QUERY_RESULT *result_data  Query result data of hash poe
 * @param[out]  : None
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE hash_respone_parse(cJSON *array, POE_QUERY_RESPONE *respone)
{
	cJSON *tmp = NULL;

	if((array == NULL) || (respone == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	tmp = cJSON_GetObjectItem(array, JSON_ALG_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_ALG_STR, tmp->valuestring);
		respone->HashMethod = hash_type_to_int((UINT8 *)tmp->valuestring);
	}

	tmp = cJSON_GetObjectItem(array, JSON_HASH_CONTENT_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_HASH_CONTENT_STR, tmp->valuestring);
		respone->PoeContent = malloc(strlen(tmp->valuestring) + 1);
		if(respone->PoeContent == NULL)
		{
			return ERR_MALLOC_FAIL;
		}
		sprintf((char *)respone->PoeContent, "%s", tmp->valuestring);
	}

	return ERR_SUCCESS;
}



/*****************************************************************************
 * @Function	: http_body_create
 * @author		: xqchen
 * @date		: 2020-8-7
 * @brief       : Generate the body data of HTTP protocol in poe save.  
 * @param[in]  	: ACCESS_KEY *customer_key  User key structure
               	  POE_SAVE_DATA *poe_data   Data structure to be encapsulated 
               	  							in HTTP body. 
 * @param[out]  : UINT8 **body				The HTTP body
 						                    After used, the corresponding inter-
 						                    face needs to be called to release
 						                    space;
 				  UINT32 *body_len     		The data len of the HTTP body
 * @return  	: ERR_CODE
 * @relation  	: http_body_destroy
 * @other  		: 

*****************************************************************************/
static ERR_CODE http_body_create( 	POE_SAVE_DATA *poe_data, 
									UINT8 **body, 
									UINT32 *body_len )
{
	cJSON *root = NULL;
	UINT8 *json_str = NULL;
	UINT32 json_len = 0;
	UINT8 *poe_type = 0;
	UINT8 hash_str[65];
	UINT8 *hash_type = NULL;
	UINT8 *sign_type = NULL;

	root = cJSON_CreateObject();	
    if(root == NULL)
    {
        return ERR_MALLOC_FAIL;
    }
	
	/*file plain is */
	if(poe_data->PoeType == POE_TYPE_FILE_PLAIN)
	{
		return ERR_POE_TYPE_NOT_SUPPORT;
	}

	cJSON_AddItemToObject(root, JSON_TIMESTAP_STR, 	  
						  cJSON_CreateNumber(poe_data->TimeStamp));
    cJSON_AddItemToObject(root, JSON_SIGN_STR,		  
						  cJSON_CreateString((char *)poe_data->Sign));
	cJSON_AddItemToObject(root, JSON_ACCESSKEYID_STR, 
						  cJSON_CreateString((char *)poe_data->AccessKeyId));	
	cJSON_AddItemToObject(root, JSON_CONTEXT_STR,  	  
						  cJSON_CreateString((char *)poe_data->Content));

	switch(poe_data->PoeType)		
	{
		case POE_TYPE_TEXT_PLAIN:
		case POE_TYPE_TEXT_CIPHER:
		case POE_TYPE_TEXT_CIPHER_SHARING:
		{
			poe_type = poetype_to_string(poe_data->PoeType);
			cJSON_AddItemToObject(root, JSON_TEXTTYPE_STR, 
						  		  cJSON_CreateString((char *)poe_type));
			break;
		}
		case POE_TYPE_TEXT_HASH:
		case POE_TYPE_FILE_HASH:
		{
			hash_type = hash_type_to_string(poe_data->HashAlgorithm);
			cJSON_AddItemToObject(root, JSON_ALG_STR, 
								  cJSON_CreateString((char *)hash_type));
			break;
		}
		case POE_TYPE_TEXT_VERIFIABLE:
		case POE_TYPE_FILE_VERIFIABLE:
		{
			memset(hash_str, 0, sizeof(hash_str));
			HexToStr((char *)hash_str, poe_data->RawHash, poe_data->HashLen);
			hash_type = hash_type_to_string(poe_data->HashAlgorithm);
			sign_type = sign_type_to_string(poe_data->SignAlgorithm);
			cJSON_AddItemToObject(root, JSON_ALG_STR, 
								  cJSON_CreateString((char *)hash_type));
			cJSON_AddItemToObject(root, JSON_RAW_HASH,  	  
								  cJSON_CreateString((char *)hash_str));
			cJSON_AddItemToObject(root, JSON_SIGN_ALG_STR, 
								  cJSON_CreateString((char *)sign_type));
			break;
		}
		default:
			return ERR_POE_TYPE_NOT_SUPPORT;
	}
	
	json_str = (UINT8 *)cJSON_Print(root);	
    if(json_str == NULL)
    {
        cJSON_Delete(root);	
        return ERR_MALLOC_FAIL;
    }
	json_len = strlen((char *)json_str);  

	*body = json_str;
	*body_len = json_len;

	cJSON_Delete(root);	
	return ERR_SUCCESS;
}

/*****************************************************************************
 * @Function	: http_body_destroy
 * @author		: xqchen
 * @date		: 2020-9-15
 * @brief       : free the spae of http body
 * @param[in]  	: UINT8 *body  http body
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static void http_body_destroy(UINT8 *body)
{
	if(body)
	{
		free(body);
		body = NULL;
	}
}

/*****************************************************************************
 * @Function	: http_url_create_save
 * @author		: xqchen
 * @date		: 2020-8-7
 * @brief       : Generate the URL address of HTTP put operation. The address
                  contains the parameters necessary for poe search. After
                  the return value of the interface is used, the correspond-
                  ing interface needs to be called to release space;
 * @param[in]  	: UINT8 *domain			   Domain of Wanxiang samurai platform
 				  POE_SAVE_TYPE save_type  poe type
 * @param[out]  : None
 * @return  	: success:the URL address
 				  fail:NULL
 * @relation  	: UrlDestroy
 * @other  		: 

*****************************************************************************/
static UINT8 *http_url_create_save(UINT8 *domain, POE_TYPE type)
{
	UINT8 *full_url = NULL;
	UINT32 len = 0;
	UINT8 *sub_url = NULL;

	sub_url = get_query_url(type, POE_METHOD_SAVE);

	/*6.Calculate the total length of the URL*/
	len = strlen((char *)domain) + strlen((char *)sub_url) + 1;
	
	/*7.Malloc space to store full url address, */
	/*the space should be freeed by UrlDestroy	*/
	full_url = (UINT8 *)malloc(len);
	if(full_url == NULL)
	{
		return NULL;
	}
	memset(full_url, 0, sizeof(len));

	/*8.assemble the URL address*/
	sprintf((char *)full_url, "%s%s", domain, sub_url);

	return full_url;
}

/*****************************************************************************
 * @Function	: http_body_parse_save
 * @author		: xqchen
 * @date		: 2020-8-7
 * @brief       : Parse the body data of the HTTP put operation response.
                  The format of the data is JSON
 * @param[in]  	: UINT8 *body         		The body data of the HTTP get
 									  		operation response
 * @param[out]  : POE_SAVE_RESPONE *respone The data structure used to store
 	              	  				  		parsing results
 * @return  	: success:0
 				  fail:ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE http_body_parse_save(UINT8 *body, POE_SAVE_RESPONE *respone)
{
	cJSON *root = NULL;
	cJSON *tmp = NULL;
	cJSON *array = NULL;

	if((body == NULL) || (respone == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	root = cJSON_Parse((char *)body);
	if(root == NULL)
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	/*1.Parse the return code and its description*/
	tmp = NULL;
	tmp = cJSON_GetObjectItem(root, JSON_RETURN_CODE_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		respone->SaveRetCode = tmp->valueint;
		DEBUG_PRINT("%s:%d\r\n", JSON_RETURN_CODE_STR, respone->SaveRetCode);
	}

	tmp = NULL;
	tmp = cJSON_GetObjectItem(root, JSON_RETURN_DESC_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		respone->SaveRetDesc = malloc(strlen(tmp->valuestring) + 1);
		if(respone->SaveRetDesc == NULL)
		{
			cJSON_Delete(root); 
			return ERR_MALLOC_FAIL;
		}
		sprintf((char *)respone->SaveRetDesc, "%s", tmp->valuestring);
		DEBUG_PRINT("%s:%s\r\n", JSON_RETURN_DESC_STR, respone->SaveRetDesc);
	}

	/*2.Ckeck the return code to determine whether the query is successful.*/
	if(respone->SaveRetCode != ERR_REQUEST_SUCCESS)
	{
		cJSON_Delete(root); 
		return respone->SaveRetCode;
	}

	array = cJSON_GetObjectItemCaseSensitive(root, JSON_DATA_STR);
    if(array == NULL)
    {
		cJSON_Delete(root);	
        return 0;
    }

	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_POEHASH_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		if(strlen(tmp->valuestring) > (HASH_DATA_LEN * 2))
		{
			cJSON_Delete(root); 
			return ERR_PARAMETER_CHECK_FAILED;
		}
		//respone->HashLen = strlen(tmp->valuestring)/2;
		respone->HashLen = StrToHex(respone->SavePoeHash, 
									tmp->valuestring, 
									strlen(tmp->valuestring)/2);
		//printHexData(JSON_POEHASH_STR, respone->SavePoeHash, respone->HashLen);
	}

	cJSON_Delete(root); 

	return ERR_SUCCESS;
}

/*****************************************************************************
 * @Function	: http_body_parse_query
 * @author		: xqchen
 * @date		: 2020-8-7
 * @brief       : Parse the body data of the HTTP get operation response.
                  The format of the data is JSON
 * @param[in]  	: UINT8 *body         			The body data of the HTTP get
 									  			operation response
 * @param[out]  : POE_QUERY_RESPONE *respone	The data structure used to store
 	              	  				  			parsing results
 * @return  	: success:0
 				  fail:ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE http_body_parse_query(UINT8 *body, POE_QUERY_RESPONE *respone)
{
	cJSON *root = NULL;
	cJSON *tmp = NULL;
	cJSON *array = NULL;
	ERR_CODE ret = ERR_SUCCESS;

	if((body == NULL) || (respone == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	root = cJSON_Parse((char *)body);
	if(root == NULL)
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	/*1.Parse the return code and its description*/
	tmp = NULL;
	tmp = cJSON_GetObjectItem(root, JSON_RETURN_CODE_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		respone->QueryRetCode = tmp->valueint;
		DEBUG_PRINT("%s:%d\r\n", JSON_RETURN_CODE_STR, respone->QueryRetCode);
	}

	tmp = NULL;
	tmp = cJSON_GetObjectItem(root, JSON_RETURN_DESC_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		respone->QueryRetDesc = malloc(strlen(tmp->valuestring) + 1);
		if(respone->QueryRetDesc == NULL)
		{
			cJSON_Delete(root); 
			return ERR_MALLOC_FAIL;
		}
		sprintf((char *)respone->QueryRetDesc, "%s", tmp->valuestring);
		DEBUG_PRINT("%s:%s\r\n", JSON_RETURN_DESC_STR, respone->QueryRetDesc);
	}

	/*2.Ckeck the return code to determine whether the query is successful.*/
	if(respone->QueryRetCode != ERR_REQUEST_SUCCESS)
	{
		cJSON_Delete(root); 
		return respone->QueryRetCode;
	}

	array = cJSON_GetObjectItemCaseSensitive(root, JSON_DATA_STR);
	if(array == NULL)
	{
		cJSON_Delete(root); 
		return 0;
	}

	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_POE_STATUS_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_POE_STATUS_STR, tmp->valuestring);
		if (strncmp(tmp->valuestring, "processing", 
					strlen(tmp->valuestring)) == 0)
		{
			respone->PoeStatus = POE_STATUS_PROCESSING;
		}
		else if (strncmp(tmp->valuestring, "done", 
				 strlen(tmp->valuestring)) == 0)
		{
			respone->PoeStatus = POE_STATUS_DONE;
		}
		else if (strncmp(tmp->valuestring, "failed", 
				 strlen(tmp->valuestring)) == 0)
		{
			respone->PoeStatus = POE_STATUS_FAILED;
		}
	}

	/*If the poe status is processing, other fields do not need to be prased*/
	if(respone->PoeStatus != POE_STATUS_DONE)
	{
		cJSON_Delete(root); 
		return ERR_SUCCESS;
	}

	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_TRANSACTION_HASH_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		if(strlen(tmp->valuestring) > (HASH_DATA_LEN * 2))
		{
			cJSON_Delete(root); 
			return ERR_PARAMETER_CHECK_FAILED;
		}
		respone->PoeTransactionHashLen = StrToHex(respone->PoeTransactionHash, 
												  tmp->valuestring, 
				 								  strlen(tmp->valuestring)/2);
		//respone->PoeTransactionHashLen = strlen(tmp->valuestring)/2;
		DEBUG_PRINT("%s:%s\r\n", JSON_TRANSACTION_INDEX_STR, tmp->valuestring);
		#if 0
		printHexData(JSON_TRANSACTION_HASH_STR, 
					 respone->PoeTransactionHash, 
					 strlen(tmp->valuestring)/2);
		#endif
	}

	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_TRANSACTION_INDEX_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%d\r\n", JSON_TRANSACTION_INDEX_STR, tmp->valueint);
		respone->PoeTransactionIndex = tmp->valueint;
	}

	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_BLOCK_HASH_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		if(strlen(tmp->valuestring) > (HASH_DATA_LEN * 2))
		{
			cJSON_Delete(root); 
			return ERR_PARAMETER_CHECK_FAILED;
		}
		respone->PoeBlockHashLen = StrToHex(respone->PoeBlockHash, 
											tmp->valuestring, 
				 							strlen(tmp->valuestring)/2);
		//respone->PoeBlockHashLen = strlen(tmp->valuestring)/2;
		#if 0
		printHexData(JSON_BLOCK_HASH_STR, respone->PoeBlockHash, 
					 strlen(tmp->valuestring)/2);
		#endif
	}

	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_BLOCK_NUM_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%d\r\n", JSON_BLOCK_NUM_STR, tmp->valueint);
		respone->PoeBlockNumber = tmp->valueint;
	}
	
	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_TIMESTAP_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		respone->PoeTimeStamp = tmp->valuedouble;
		DEBUG_PRINT("%s:%" PRId64 "\r\n", JSON_TIMESTAP_STR, 
					respone->PoeTimeStamp);
	}

	/*3.Parse the Poe type and malloc space according to the type*/
	tmp = NULL;
	tmp = cJSON_GetObjectItem(array, JSON_POE_TYPE_STR);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		DEBUG_PRINT("%s:%s\r\n", JSON_POE_TYPE_STR, tmp->valuestring);
		respone->PoeType = poetype_to_int((UINT8 *)tmp->valuestring);
	}

	switch(respone->PoeType)
	{
		case POE_TYPE_TEXT_PLAIN:
		case POE_TYPE_TEXT_CIPHER:
		case POE_TYPE_TEXT_CIPHER_SHARING:
			ret = plain_respone_parse(array, respone);
			break;
		case POE_TYPE_TEXT_VERIFIABLE:
		case POE_TYPE_FILE_VERIFIABLE:
			ret = verifiable_respone_parse(array, respone);
			break;
		case POE_TYPE_FILE_PLAIN:
			ret = file_respone_parse(array, respone);
			break;
		case POE_TYPE_TEXT_HASH:
		case POE_TYPE_FILE_HASH:
			ret = hash_respone_parse(array, respone);
			break;
		default:
			ret = ERR_POE_TYPE_NOT_SUPPORT;
			break;
	}

	cJSON_Delete(root); 
	return ret;
}

/*****************************************************************************
 * @Function	: http_url_create_query
 * @author		: xqchen
 * @date		: 2020-8-7
 * @brief       : Generate the URL address of HTTP get operation. The address
                  contains the parameters necessary for poe search. After
                  the return value of the interface is used, the correspond-
                  ing interface needs to be called to release space;
 * @param[in]  	: UINT8 *domain				Domain of Wanxiang samurai platform
 				  POE_QUERY_DATA *poe_data  Data structure to be encapsulated 
               	  							in HTTP url
 * @param[out]  : UINT8 **url:the URL address
 * @return  	: success:0
 				  fail:ERR_CODE
 * @relation  	: http_url_destroy
 * @other  		: 

*****************************************************************************/
static ERR_CODE http_url_create_query(UINT8 *domain, 
							   POE_QUERY_DATA *poe_data, 
							   UINT8 **url)
{
	UINT8 timestamp_str[32];
	UINT32 len = 0;
	UINT8 *sub_url = NULL;
	UINT8 *full_url = NULL;
	UINT8 *poe_type = NULL;
	UINT8 hash_str[65];

	/*2.Convert the timestamp to string*/
	memset(timestamp_str, 0u, sizeof(timestamp_str));
	sprintf((char *)timestamp_str, "%" PRId64 "", poe_data->TimeStamp);
	memset(hash_str, 0, sizeof(hash_str));
	HexToStr((char *)hash_str, poe_data->PoeHash, poe_data->HashLen);
	poe_type = poetype_to_string(poe_data->PoeType);
	/*3.Calculate the total length of the URL*/
	sub_url = get_query_url(poe_data->PoeType, POE_METHOD_QUERY);
	len = strlen((char *)domain)				+ 
		  strlen((char *)sub_url)				+ 1 +	/*1 for ?	   */
		  strlen((char *)timestamp_str)			+ 
		  strlen((char *)JSON_TIMESTAP_STR)		+ 2 +	/*2 for = and &*/
		  strlen((char *)poe_data->Sign)		+ 
		  strlen((char *)JSON_SIGN_STR)			+ 2 +	/*2 for = and &*/
		  strlen((char *)poe_data->AccessKeyId)	+ 
		  strlen((char *)JSON_ACCESSKEYID_STR)	+ 2 +	/*2 for = and &*/
		  strlen((char *)poe_type)				+ 
		  strlen((char *)JSON_TEXTTYPE_STR)		+ 2 +	/*2 for = and &*/
		  strlen((char *)hash_str)				+ 
		  strlen((char *)JSON_POEHASH_STR)		+ 1;	/*1 for =	   */

	/*4.Malloc space to store full url address, */
	/*the space should be freeed by UrlDestroy	*/
	full_url = (UINT8 *)malloc(len + 1);
	if(full_url == NULL)
	{
		return ERR_MALLOC_FAIL;
	}
	memset(full_url, 0, sizeof(len + 1));

	/*5.assemble the URL address*/
	sprintf((char *)full_url, "%s%s?%s=%s&%s=%s&%s=%s&%s=%s&%s=%s", 
					domain, sub_url, 
					JSON_TIMESTAP_STR, 		timestamp_str,
				   	JSON_SIGN_STR, 			poe_data->Sign, 
				   	JSON_ACCESSKEYID_STR, 	poe_data->AccessKeyId, 
				   	JSON_TEXTTYPE_STR, 		poe_type,
				   	JSON_POEHASH_STR, 		hash_str);
	*url = full_url;
	return ERR_SUCCESS;
}

/*****************************************************************************
* @Function    : http_url_destroy
* @author	   : xqchen
* @date 	   : 2020-8-7
* @brief	   : Release the corresponding space of URL address;
* @param[in]   : UINT8 *url  The URL address
* @param[out]  : None
* @return	   : success:0
				 fail:ERR_CODE
* @relation    : 
* @other	   : 

*****************************************************************************/
static ERR_CODE http_url_destroy(UINT8 *url)
{
	if(url)
	{
		free(url);
		url = NULL;
	}

	return ERR_SUCCESS;
}

/*****************************************************************************
 * @Function	: http_post_form_create
 * @author		: xqchen
 * @date		: 2020-8-17
 * @brief       : Generate the HTTP form list, the caller needs to encapsul-
                  ate it into the HTTP protocol
 * @param[in]  	: POE_SAVE_DATA *poe_data	Data structure to be encapsulated 
               	  							in HTTP form
               	  UINT32 form_num          	form number
 * @param[out]  : HTTP_POST_FORM *form     	form list
 * @return  	: HTTP_POST_FORM
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE http_post_form_create(POE_SAVE_DATA *poe_data, 
 							   		  HTTP_POST_FORM *form,
 							   		  UINT32 form_num)
{
	UINT8 timestamp_str[32];
	HTTP_POST_FORM *tmp = NULL;
	UINT32 i = 0;


	/*fill in file name */
	tmp = &form[i++];
	tmp->content_type = HTTP_CONTENT_OCTET_STREAM;
	sprintf((char *)tmp->content_name, "%s", JSON_FILE_STR);
	tmp->content = (UINT8 *)malloc(strlen((char *)poe_data->Content) + 1);
	if(tmp->content == NULL)
	{
		return ERR_MALLOC_FAIL;
	}
	memset(tmp->content, 0, strlen((char *)poe_data->Content) + 1);
	sprintf((char *)tmp->content, "%s", poe_data->Content);

	/*fill in timestamp */
	tmp = &form[i++];
	memset(timestamp_str, 0u, sizeof(timestamp_str));
	sprintf((char *)timestamp_str, "%" PRId64 "", poe_data->TimeStamp);
	tmp->content_type = HTTP_CONTENT_TEXT_PLAIN;
	sprintf((char *)tmp->content_name, "%s", JSON_TIMESTAP_STR);
	tmp->content = (UINT8 *)malloc(strlen((char *)timestamp_str) + 1);
	if(tmp->content == NULL)
	{
		return ERR_MALLOC_FAIL;
	}
	memset(tmp->content, 0, strlen((char *)timestamp_str) + 1);
	sprintf((char *)tmp->content, "%s", timestamp_str);

	/*fill in access key */
	tmp = &form[i++];
	tmp->content_type = HTTP_CONTENT_TEXT_PLAIN;
	sprintf((char *)tmp->content_name, "%s", JSON_ACCESSKEYID_STR);
	tmp->content = malloc(strlen((char *)poe_data->AccessKeyId) + 1);
	if(tmp->content == NULL)
	{
		return ERR_MALLOC_FAIL;
	}
	memset(tmp->content, 0, strlen((char *)poe_data->AccessKeyId) + 1);
	sprintf((char *)tmp->content, "%s", poe_data->AccessKeyId);
	

	/*fill in sign data */
	tmp = &form[i++];
	tmp->content_type = HTTP_CONTENT_TEXT_PLAIN;
	sprintf((char *)tmp->content_name, "%s", JSON_SIGN_STR);
	tmp->content = malloc(strlen((char *)poe_data->Sign) + 1);
	if(tmp->content == NULL)
	{
		return ERR_MALLOC_FAIL;
	}
	memset(tmp->content, 0, strlen((char *)poe_data->Sign) + 1);
	sprintf((char *)tmp->content, "%s", poe_data->Sign);
	
	return ERR_SUCCESS;
}

/*****************************************************************************
 * @Function  : http_post_form_destroy
 * @author	  : xqchen
 * @date      : 2020-8-7
 * @brief 	  : Release the corresponding space of the post form of the HTTP;
 * @param[in] : HTTP_POST_FORM *form  The post form of the HTTP
 *			    UINT32 form_num		  The post form number
 * @param[out]: None
 * @return	  : void
 * @relation  : 
 * @other 	  : 

*****************************************************************************/
static void http_post_form_destroy(HTTP_POST_FORM *form, UINT32 form_num)
{
	UINT32 i = 0;
	HTTP_POST_FORM *tmp = NULL;
	
	if(form == NULL)
	{
		return;
	}

	for(i = 0; i < form_num; i++)
	{
		tmp = &form[i];
		if(tmp && tmp->content)
		{
			free(tmp->content);
			tmp->content = NULL;
		}
	}	
}

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
ERR_CODE PoeSave(POE_SAVE_PARAM *poe_param, POE_SAVE_RESPONE *respone)
{
	POE_SAVE_DATA poe_data;
	http_respone post_return;
	UINT8 *url = NULL;
	UINT8 *body = NULL;
	UINT32 body_len = 0;
	ERR_CODE ret = ERR_SUCCESS;
	UINT32 poe_type;
	HTTP_POST_FORM form[4];
	UINT32 form_num = 4;

	/*1.Generate a standard poe save data 		*/
	/*  structure according to the parameters	*/
	memset(&poe_data, 0, sizeof(poe_data));
	ret = PoeSaveDataCreate(poe_param, &poe_data);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}
	
	/*2.get url address*/
	poe_type = poe_data.PoeType;
	url = http_url_create_save((UINT8 *)SERVER_DOMAIN, poe_type);
	DEBUG_PRINT("url:%s\r\n", url);

	http_init();

	if(poe_type == POE_TYPE_FILE_PLAIN)
	{
		/*3.HTTP protocol body data create*/
		memset(&form, 0, sizeof(form));
		
		ret = http_post_form_create(&poe_data, form, form_num);
		if(ret != ERR_SUCCESS)
		{
			PoeSaveDataDestroy(&poe_data);
			return ret;
		}

		/*4.HTTP data post*/
		memset(&post_return, 0, sizeof(post_return));
		ret = http_post_file((char *)url, form, form_num, (void *)&post_return);
		if(ret != ERR_SUCCESS)
		{
			http_post_form_destroy(form, form_num);
			PoeSaveDataDestroy(&poe_data);
			return ret;
		}
		http_post_form_destroy(form, form_num);
	}
	else
	{
		/*3.HTTP protocol body data create*/
		ret = http_body_create(&poe_data, &body, &body_len);
		if(ret != ERR_SUCCESS || body == NULL)
		{
			PoeSaveDataDestroy(&poe_data);
			return ret;
		}
		
		DEBUG_PRINT("body:%s\r\n", body);
		
		/*4.HTTP data post*/
		memset(&post_return, 0, sizeof(post_return));
		ret = http_post((char *)url, 
						(char *)body, 
						body_len, 
						(void *)&post_return);
		if(ret != ERR_SUCCESS)
		{
			http_body_destroy(body);
			PoeSaveDataDestroy(&poe_data);
			return ret;
		}
		http_body_destroy(body);
	}
	
	/*5 http response parse */
	http_body_parse_save((UINT8 *)post_return.data, respone);
	
	/*6 free temporary space*/
	http_url_destroy(url);
	PoeSaveDataDestroy(&poe_data);
	if(post_return.data)
	{
		free(post_return.data);
		post_return.data = NULL;
	}
	
	return ret;
}

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
ERR_CODE PoeQuery(POE_QUERY_PARAM *poe_param, POE_QUERY_RESPONE *respone)
{
	ERR_CODE ret = ERR_SUCCESS;
	UINT8 *url = NULL;
	http_respone post_return;
	POE_QUERY_DATA poe_data;

	/*1.Generate a standard poe query data 		*/
	/*  structure according to the parameters	*/
	memset(&poe_data, 0, sizeof(poe_data));
	ret = PoeQueryDataCreate(poe_param, &poe_data);
	if(ret != ERR_SUCCESS)
	{
		return ret;
	}

	/*2.get url address*/
	ret = http_url_create_query((UINT8 *)SERVER_DOMAIN, &poe_data, &url);
	if(ret != ERR_SUCCESS || url == NULL)
	{
		return ret;
	}

	DEBUG_PRINT("URL:%s\r\n", url);
	http_init();
	
	/*3.HTTP data get*/
	memset(&post_return, 0, sizeof(post_return));
	http_get((char *)url, (void *)&post_return);

	/*4.HTTP respone parse*/
	http_body_parse_query((UINT8 *)post_return.data, respone);

	/*5.free temporary space*/
	http_url_destroy(url);
	if(post_return.data)
	{
		free(post_return.data);
		post_return.data = NULL;
	}
	return ret;
}

UINT8 *PoeQueryTest(POE_QUERY_PARAM *poe_param)
{
	ERR_CODE ret = ERR_SUCCESS;
	UINT8 *url = NULL;
	http_respone post_return;
	POE_QUERY_DATA poe_data;

	/*1.Generate a standard poe query data 		*/
	/*  structure according to the parameters	*/
	memset(&poe_data, 0, sizeof(poe_data));
	ret = PoeQueryDataCreate(poe_param, &poe_data);
	if(ret != ERR_SUCCESS)
	{
		return NULL;
	}

	/*2.get url address*/
	ret = http_url_create_query((UINT8 *)SERVER_DOMAIN, &poe_data, &url);
	if(ret != ERR_SUCCESS || url == NULL)
	{
		return NULL;
	}

	DEBUG_PRINT("URL:%s\r\n", url);
	http_init();
	
	/*3.HTTP data get*/
	memset(&post_return, 0, sizeof(post_return));
	http_get((char *)url, (void *)&post_return);

	http_url_destroy(url);
	return (UINT8 *)post_return.data;
}


