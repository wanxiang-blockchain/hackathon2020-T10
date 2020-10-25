/*******************************************************************************
 * @file   			: samurai_url_define.h
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This document mainly defines the domain name of the
                     samurai system and the URL address of all poe operation
 * @Copyright   	: Copyright (c) 2008-2020  Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
*******************************************************************************/
#ifndef __SAMURAI_URL_DEFINE_H__
#define __SAMURAI_URL_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define TEXT_ONCHAIN_URL			"/api/poe/text/savePoe"
#define TEXT_GETCHAIN_URL			"/api/poe/text/getPoe"
#define TEXT_SHARE_ONCHAIN_URL		"/api/poe/text/sharing/savePoe"
#define FILE_ONCHAIN_URL    		"/api/poe/file/savePoe"
#define FILE_GETCHAIN_URL			"/api/poe/file/getPoe"
#define HASH_ONCHAIN_URL    		"/api/poe/hash/savePoe"
#define HASH_GETCHAIN_URL			"/api/poe/hash/getPoe"
#define VERIFY_ONCHAIN_URL  		"/api/poe/verifiable/savePoe"
#define VERIFY_GETCHAIN_URL 		"/api/poe/verifiable/getPoe"

//#define SERVER_DOMAIN 				"https://testsamurai.i.wxblockchain.com"
#define SERVER_DOMAIN 				"https://testpoe.i.wxblockchain.com/frontgateway"



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


