/***********************************************************************************
 * @file   			: algorithm_api.h
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This header file mainly defines the data structure,
                      macro and external interface of algorithm layer.
                     
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#ifndef __ALGORITHM_API_H__
#define __ALGORITHM_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*****************************************************************************
 * @Function	: DataHash
 * @author		: xqchen
 * @date		: 2020-8-10
 * @brief       : Calculate the hash value of text data, using the specified
                  hash algorithm;
 * @param[in]  	: UINT8 *data  				Text data to be hashed
 				  UINT32 len  				Text data length
               	  HASH_TYPE hash_method  	hash algorithm
 * @param[out]  : UINT8 *hash  Hash value
 * @return		: success:length of hash value
 				  fail:0
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 DataHash( UINT8 *data, UINT32 len, HASH_TYPE hash_method, UINT8 *hash );

/*****************************************************************************
 * @Function	: FileHash
 * @author		: xqchen
 * @date		: 2020-8-10
 * @brief       : Calculate the hash value of the file, using the specified
                  hash algorithm;
 * @param[in]  	: UINT8 *file              The file to be hashed
               	  HASH_TYPE hash_method  hash algorithm
 * @param[out]  : UINT8 *hash              hash value
 * @return  	: success:0
 				  fail:ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 FileHash(UINT8 *file, HASH_TYPE hash_method, UINT8 *hash);


/*****************************************************************************
 * @Function	: DataSignature
 * @author		: Preh
 * @date		: 2020-8-12
 * @brief       : Calculate the signature value of the data, using the 
 				  specified signature algorithm
 * @param[in]  	: UINT8 *data            data to be signed
               	  UINT32 data_len        data length
               	  SIGN_TYPE sign_method  signature algorithm
               	  UINT8 *pri_key         signature private key
               	  UINT64 timestamp       the timestamp when function is executed
 * @param[out]  : None
 * @return  	: the signature data
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE DataSignature(UINT8 *data, UINT32 data_len, SIGN_TYPE sign_method, 
					   UINT8 *pri_key, UINT8 signed_msg[65]);


/*****************************************************************************
 * @Function	: DataEncryption
 * @author 	 	: xqchen
 * @date		: 2020-8-10
 * @brief		: Encrypt the data, using the specified encryption algorithm
 * @param[in]	: UINT8 *data			  Data to be encrypted
 			   	  ENC_TYPE enc_method  encryption algorithm
 			   	  UINT8 *enc_key		  encryption key
 * @param[out]  : UINT8 *cipher		  cipher data
 * @return 	 	: success:0
 			   	  fail:ERR_CODE
 * @relation	: 
 * @other		: 

*****************************************************************************/
ERR_CODE DataEncryption(UINT8 *data, UINT32 data_len, ENC_TYPE enc_method, 
					    UINT8 *enc_key, UINT8 *cipher);

/*****************************************************************************
 * @Function	: ShareKeyCalc
 * @author		: xqchen
 * @date		: 2020-8-10
 * @brief		: Calculate the shared key, using the private key of the
 				  data sending and the public key of the data receiver
 * @param[in]   : UINT8 *r_pub_key	   the public key of the data receiver
 				  UINT8 *s_pri_key	   the private key of the data sending
 				  ENC_TYPE enc_method  encryption algorithm
 * @param[out]  : UINT8 *share_key	   the shared key
 * @return	  	: success:0
 				  fail:ERR_CODE
 * @relation	: 
 * @other		: 

*****************************************************************************/
ERR_CODE ShareKeyCalc(UINT8 *r_pub_key, UINT8 *s_pri_key, 
					  ENC_TYPE enc_method, UINT8 *share_key);

/*****************************************************************************
 * @Function	: PublicKeyRecover
 * @author		: xqchen
 * @date		: 2020-8-27
 * @brief       : Recover an ECDSA public key from a signature
 * @param[in]  	: ENC_TYPE enc_method  encryption algorithm
 				  UINT8 *sig      data signture
               	  UINT8 *msg      original data
               	  UINT32 msg_len  data length
 * @param[out]  : UINT8 *pub_key  a 65-byte array computed from the signature
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE PublicKeyRecover(ENC_TYPE enc_method, UINT8 *sig, UINT8 *msg, 
						  UINT32 msg_len, UINT8 *pub_key);

/*****************************************************************************
* @Function	  : DataBase64Decode
* @author	  : Preh
* @date		  : 2020-8-10
* @brief 	  : decode private key coding by base64
* @param[in]  : UINT8 *data 		  data encoded by base64
				UINT32 data_len 	  length of the data base64 by base64			  
* @return	  : success:  decoded data, the space must be freed manually. 
				fail:	  NULL
* @relation	  : 
* @other 	  : 

*****************************************************************************/
UINT8 *DataBase64Decode(UINT8 *data, UINT32 data_len);

/*****************************************************************************
 * @Function	: DataBase64Encode
 * @author		: Preh
 * @date		: 2020-8-10
 * @brief		: encoding data by base64
 * @param[in]	: UINT8* data	   data to encode
				  UINT16 data_len  data length 
 * @param[out]	: None
 * @return		: encoded data by base64
 * @relation	: 
 * @other		: 

*****************************************************************************/
UINT8 *DataBase64Encode(UINT8* data, UINT32 data_len);


/*****************************************************************************
 * @Function	: KeyPairCreate
 * @author		: xqchen
 * @date		: 2020-9-14
 * @brief       : Key pair generation function
 				  The function automatically generates a random number and uses 
 				  the number to generate a pair of public and private keys.
 * @param[in]  	: ENC_TYPE enc_method  Key pair generation algorithm
 * @param[out]  : UINT8 pri_key[32]    32 bytes private key
               	  UINT8 pub_key[65]    65 bytes public key
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE KeyPairCreate(ENC_TYPE enc_method, UINT8 pri_key[32], UINT8 pub_key[65]);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


