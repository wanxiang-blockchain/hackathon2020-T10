/***********************************************************************************
 * @file   			: samurai_client.h
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This header file mainly defines the data structure,
                      macro and external interface of samurai client layer.
                     
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __SAMURAI_CLIENT_H__
#define __SAMURAI_CLIENT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "types.h"

/*Signature data max length based on Base64 encoding*/
#define SIGN_DATA_LEN	149

/*POE operation method*/
typedef enum
{
	POE_METHOD_QUERY,
	POE_METHOD_SAVE,
}POE_METHOD;

/*A standard poe save data structure*/
typedef struct _POE_SAVE_DATA
{
	POE_TYPE	PoeType;					/*poe type:refer to the poe type  */
											/*definition enum POE_TYPE  	  */
	HASH_TYPE 	HashAlgorithm;				/*hash type:refer to the hash type*/
											/*definition enum HASH_TYPE  	  */
	SIGN_TYPE	SignAlgorithm;				/*sign type:refer to the sign type*/
											/*definition enum SIGN_TYPE  	  */
	UINT64 		TimeStamp;					/*time stamp					  */
	UINT8  		Sign[SIGN_DATA_LEN];		/*Signature data based on		  */ 
											/*Base64 encoding				  */
	UINT8  		AccessKeyId[ACCESS_KEY_LEN];/*user's access ID				  */
	UINT8		*Content;					/*The poe save content, has diffe-*/ 
											/*rent value according to poeType:*/
											/*poe plain text:plain data		  */
											/*poe plain file:file name		  */
											/*poe hash:the hash value string  */
											/*poe verif:the sign value string */
	UINT8		*Description;				/*file description, just for file */ 
	UINT8		RawHash[HASH_DATA_LEN];		/*the hash value of data/file	  */ 
											/*just for poe verif save		  */
	UINT32		HashLen;					/*the hash value length			  */
}POE_SAVE_DATA ,*P_POE_SAVE_DATA; 

/*The parameters of poe save*/
typedef struct _POE_SAVE_PARAM
{
	ACCESS_KEY	*AccessKey;	/*User access key structure						  */
	POE_TYPE	PoeType;	/*poe type:refer to the poe type  			  	  */
							/*definition enum POE_TYPE  	  			  	  */
	UINT8 		*Data; 		/*The data to save, if file is the file name. 	  */
	UINT32 		DataLen;	/*data length								  	  */
	HASH_TYPE 	HashMethod;	/*hash type:refer to the hash type			  	  */
							/*definition enum HASH_TYPE  	  			  	  */
	SIGN_TYPE 	SignMethod;	/*sign type:refer to the sign type			  	  */
							/*definition enum SIGN_TYPE  	  			  	  */
	ENC_TYPE 	EncMethod;	/*encryption type:refer to the encryption type	  */
							/*definition enum ENC_TYPE  	  			  	  */
	UINT8 		PriKey[PRIVATE_KEY_LEN];/*User private key for data signture  */
										/*or encryption						  */
}POE_SAVE_PARAM ,*P_POE_SAVE_PARAM; 

/*The parameters of poe query*/
typedef struct _POE_QUERY_PARAM
{
	ACCESS_KEY	*AccessKey;		/*User key structure						  */
	UINT8		*Data;			/*The data to save, if file is the file name. */
	UINT32 		DataLen;		/*data length								  */
	POE_TYPE	PoeType;		/*poe type:refer to the poe type  			  */
								/*definition enum POE_TYPE  	  			  */
	SIGN_TYPE 	SignMethod;		/*sign type:refer to the sign type			  */
								/*definition enum SIGN_TYPE  	  			  */
}POE_QUERY_PARAM ,*P_POE_QUERY_PARAM;


/*A standard poe query data structure*/
typedef struct _POE_QUERY_DATA
{
	POE_TYPE	PoeType;					/*poe type:refer to the poe type  */
											/*definition enum POE_TYPE  	  */
	UINT64 		TimeStamp;					/*time stamp					  */
	UINT8  		Sign[SIGN_DATA_LEN];		/*Signature data based on		  */ 
											/*Base64 encoding				  */
	UINT8  		AccessKeyId[ACCESS_KEY_LEN];/*user's access ID				  */
	UINT8		PoeHash[HASH_DATA_LEN];		/*The poe hash to be queried	  */
	UINT32		HashLen;					/*the hash value length			  */
}POE_QUERY_DATA ,*P_POE_QUERY_DATA; 


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
ERR_CODE PoeSaveDataDestroy(POE_SAVE_DATA *poe_data);

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
ERR_CODE PoeSaveDataCreate(POE_SAVE_PARAM *poe_param, POE_SAVE_DATA *poe_data);

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
							POE_QUERY_DATA *poe_data);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif



