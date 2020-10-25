/***********************************************************************************
 * @file   			: types.h
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This document mainly defines the basic data type, data
                     structure, macro and data enumeration in the SDK
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <inttypes.h>

//#define DEBUG

/*Macro definition*/
#ifndef FALSE
#define FALSE  0
#endif

#ifndef TRUE
#define TRUE   1
#endif

#ifdef	DEBUG
#define DEBUG_PRINT(fmt, args...) printf(fmt, ##args);
#else
#define DEBUG_PRINT(fmt, args...)
#endif	/* DEBUG */


#ifndef UINT8
typedef unsigned char   	UINT8;
#endif

#ifndef UINT16
typedef unsigned short  	UINT16;
#endif

#ifndef UINT32
typedef unsigned int  		UINT32;
#endif

#ifndef UINT64
typedef uint64_t 			UINT64;
#endif


#ifndef INT8
typedef signed char 		INT8;
#endif

#ifndef INT16
typedef signed short  		INT16;
#endif

#ifndef INT32
typedef signed int  		INT32;
#endif

#ifndef INT64
typedef int64_t          	INT64;
#endif

/*user access key max length*/
#define ACCESS_KEY_LEN		64
/*user private key max length*/
#define PRIVATE_KEY_LEN		32
/*hash value max length*/
#define HASH_DATA_LEN		33

/*enum define*/
/*Error code define*/
typedef enum
{
	/*The following error code is the client local error code.				*/
	ERR_SUCCESS					= 0,	 /*success							*/
	ERR_FILE_OPEN_FAILED		= 1001,	 /*file open fail					*/
	ERR_FILE_READ_FAILED		= 1002,	 /*file read error					*/
	ERR_FILE_IS_NULL			= 1003,	 /*file size is 0					*/
	ERR_HASH_METHOD_NOT_SUPPORT	= 2001,	 /*Hash algorithm not supported		*/
	ERR_HASH_FAIL				= 2002,	 /*Hash calculation failed			*/
	ERR_SIGN_FAIL				= 2003,	 /*Signature calculation failed		*/
	ERR_SIGN_METHOD_NOT_SUPPORT	= 2004,	 /*sign algorithm not supported		*/
	ERR_MALLOC_FAIL				= 3001,  /*malloc fail, maybe not 			*/
										 /*enough heap memory				*/
	ERR_BASE64_ENCODE_FAIL		= 4001,	 /*base64 encode fail				*/
	ERR_BASE64_DECODE_FAIL		= 4002,	 /*base64 decode fail				*/
	ERR_DATA_ASSEMBLE_FAIL		= 5001,  /*data assemble fail				*/
	ERR_POE_TYPE_NOT_SUPPORT	= 6001,	 /*POE type not supported			*/
	ERR_KEY_GENERATE_FAIL		= 7001,

	/*The following error code is consistent with the server				*/
	ERR_REQUEST_SUCCESS			= 17000, /*Request success/response ok		*/
	ERR_PARAMETER_CHECK_FAILED 	= 12010, /*Parameter check failed, maybe	*/ 
										 /*parameter is null or content 	*/
										 /*length is too long				*/
	ERR_FILE_UPLOAD_FAILED		= 18010, /*File upload failed				*/
	ERR_FILE_DOWNLOAD_FAILED	= 18020, /*File download failed				*/
	ERR_REQUEST_TIMEOUT			= 20001, /*request timeout, The timestamp 	*/
										 /*is 2 minute longer				*/
	ERR_INCORRECT_ACCESSKEYID  	= 20003, /*incorrect accessKeyId, Identity 	*/
										 /*check failed						*/
	ERR_ACCESS_CLOSED			= 20005, /*access closed, The uplink  		*/
										 /*function has been suspended, 	*/
										 /*please contact customer service	*/
	ERR_VERIFY_SIGN_FAILED		= 20006, /*verify sign failed				*/
	ERR_CONTENT_STORAGE_FAILED 	= 20007, /*content storage failed, please 	*/
										 /*try again						*/
	ERR_POE_HASH_INCORRECT		= 20009, /*poe hash incorrect				*/
	ERR_FILE_SIZE_EXCEEDED		= 20010, /*file size exceeded, file size no */
										 /*larger than 30 MB				*/
	ERR_POE_TYPE_MISMATCH		= 20011, /*poe type mismatch, check your poe*/
										 /*type if match sdk method			*/
	ERR_POE_NOT_AVAILABLE		= 20012, /*poe not available, try later or 	*/
										 /*contact admin					*/
}ERR_CODE;

/*hash method define, now only support MD5 and SHA-256*/
typedef enum
{
	HASH_MD5,
	HASH_SHA_256,
	HASH_SHA3K,
	HASH_TYPE_UNKNOWN
}HASH_TYPE;

/*sign method define, now only support ECDSA*/
typedef enum
{
	SIGN_ECDSA,
	SIGN_TYPE_UNKNOWN
}SIGN_TYPE;

/*encryption method define, now only support ECDH*/
typedef enum
{
	ENC_TYPE_ECDSA,
	ENC_TYPE_UNKNOWN
}ENC_TYPE;

/*poe status define*/
typedef enum
{
	POE_STATUS_PROCESSING,
	POE_STATUS_DONE,
	POE_STATUS_FAILED
}POE_STATUS;

/*poe type define*/
typedef enum
{
	POE_TYPE_TEXT_PLAIN,			/*plain text on chain			*/
	POE_TYPE_TEXT_CIPHER,			/*encrypted text on chain		*/
	POE_TYPE_TEXT_CIPHER_SHARING,	/*share encrypted text on Chain	*/
	POE_TYPE_TEXT_HASH,				/*text hash on chain			*/
	POE_TYPE_TEXT_VERIFIABLE,		/*text hash signature on chain	*/
	POE_TYPE_FILE_PLAIN,			/*plain file on chain			*/
	POE_TYPE_FILE_HASH,				/*file hash on chain			*/
	POE_TYPE_FILE_VERIFIABLE,		/*file hash signature on chain	*/
	POE_TYPE_UNKNOWN				/*unknown poe type				*/
}POE_TYPE;


/*samurai platform access key structure*/
/*which provided by Wanxiang blockchain*/
typedef struct _ACCESS_KEY
{
	UINT8	AccessId[ACCESS_KEY_LEN];	/*user's access ID*/
	UINT8	PrivKey[PRIVATE_KEY_LEN];	/*user's private key*/
}ACCESS_KEY ,*P_ACCESS_KEY; 

/*poe save result data structure*/
typedef struct _POE_SAVE_RESPONE
{
	ERR_CODE	SaveRetCode;				/*Return code of Poe save	*/
	UINT8		SavePoeHash[HASH_DATA_LEN];	/*The Hash value of Poe save*/
											/*we can use this hash value*/ 
											/*for Poe query				*/
	UINT32		HashLen;
	UINT8		*SaveRetDesc;				/*Return code description	*/
}POE_SAVE_RESPONE ,*P_POE_SAVE_RESPONE; 

/*poe query result structure*/
typedef struct _POE_QUERY_RESPONE
{
	ERR_CODE	QueryRetCode;						/*Return code of Poe query*/		
	POE_STATUS	PoeStatus;							/*Poe save process status */
	POE_TYPE	PoeType;							/*Poe save type			  */
	UINT8		PoeTransactionHash[HASH_DATA_LEN];	/*transaction hash		  */
	UINT32		PoeTransactionHashLen;
	UINT32  	PoeTransactionIndex;				/*transaction index 	  */
	UINT8		PoeBlockHash[HASH_DATA_LEN];		/*block hash			  */
	UINT32		PoeBlockHashLen;
	UINT32  	PoeBlockNumber;						/*block number			  */
	UINT64		PoeTimeStamp;						/*timestamp				  */
	HASH_TYPE 	HashMethod;							/*hash type:refer to the  */
													/*hash type definition	  */
													/*enum HASH_TYPE		  */
	SIGN_TYPE	SignMethod;							/*sign type:refer to the  */
													/*sign type definition 	  */
													/*enum SIGN_TYPE  	  	  */
	UINT8		RawHash[HASH_DATA_LEN];				/*hash value of data/file */ 
	UINT32		RawHashLen;							/*the hash value length	  */
	UINT8		*QueryRetDesc;						/*Return code description */
	UINT8		*PoeContent;						/*The poe save content,has*/ 
													/*diff value according to */
													/*poeType:				  */
													/*poe plain text: data	  */
													/*poe plain file:file name*/
													/*poe hash:hash value str */
													/*poe verif:sign value str*/
	UINT8		*FileDesc;							/*file description		  */ 
	UINT8		*FileOssDownloadPath;				/*file DownloadPath		  */ 
}POE_QUERY_RESPONE ,*P_POE_QUERY_RESPONE; 


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

