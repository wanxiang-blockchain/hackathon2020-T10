/***********************************************************************************
 * @file   			: json_define.h
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This file defines the JSON string used in the HTTP
                     protocol
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __JSON_DEFINE_H__
#define __JSON_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "types.h"

#define JSON_CONTEXT_STR			"content"
#define JSON_TIMESTAP_STR			"timestamp"
#define JSON_SIGN_STR				"sign"
#define JSON_ACCESSKEYID_STR		"accessKeyId"
#define JSON_TEXTTYPE_STR			"textType"
#define JSON_POEHASH_STR			"poeHash"
#define JSON_RETURN_CODE_STR		"returnCode"
#define JSON_RETURN_DESC_STR		"returnDesc"
#define JSON_DATA_STR				"data"
#define JSON_POE_STATUS_STR			"poeStatus"
#define JSON_POE_TYPE_STR			"poeType"
#define JSON_TRANSACTION_HASH_STR	"transactionHash"
#define JSON_TRANSACTION_INDEX_STR	"transactionIndex"
#define JSON_BLOCK_NUM_STR			"blockNumber"
#define JSON_BLOCK_HASH_STR			"blockHash"
#define JSON_TEXT_CONTENT_STR		"textContent"
#define JSON_FILE_STR               "attachment"
#define JSON_FILENAME_STR			"fileName"
#define JSON_FILEDESP_STR           "fileDescription"
#define JSON_FILE_DOWNLOAD_PATH_STR "ossDownloadPath"
#define JSON_ALG_STR                "hashAlgorithm"
#define JSON_RAW_HASH               "rawHash"
#define JSON_SIGN_ALG_STR           "signatureAlgorithm"
#define JSON_HASH_CONTENT_STR		"hashContent"
#define JSON_HASH_SIGN_STR			"hashSignature"
#define JSON_FILE_HASH              "fileHash"

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


