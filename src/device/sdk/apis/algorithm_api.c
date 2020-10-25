/***********************************************************************************
 * @file   			: algorithm_api.c
 * @author   		: xqchen
 * @date   			: 2020-9-15
 * @Description   	: This file provides the underlying algorithm interface.
                      Including the following algorithms: hash, signature encry-
                      ption, key pair generation, public key recovery, Base64 
                      encoding and decoding.
 * @Copyright   	: Copyright (c) 2008-2020  Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#include "types.h"
#include "md5.h"
#include "hasher.h"
#include "base64.h"
#include "secp256k1_api.h"
#include "samurai_client.h"

/*****************************************************************************
 * @Function	: file_size
 * @author		: xqchen
 * @date		: 2020-8-10
 * @brief       : get the file size
 * @param[in]  	: UINT8 *file  file name
 * @param[out]  : None
 * @return  	: the file size
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static UINT32 file_size(UINT8 *file) 
{
	INT32 fd = 0;
	INT32 filelen = 0;

	fd = open((char *)file, O_RDONLY);
	if (fd < 0) 
	{	 
		return ERR_FILE_OPEN_FAILED;
	}	

	filelen = lseek(fd, 0L, SEEK_END);
	lseek(fd,0L,SEEK_SET); 
	close(fd);

	return filelen;
}

/*****************************************************************************
 * @Function	: file_read
 * @author		: xqchen
 * @date		: 2020-8-10
 * @brief       : Read data the of the file
 * @param[in]  	: UINT8 *file       the file to read 
               	  UINT32 read_size  read size
 * @param[out]  : UINT8 *filebuff   data read out
 * @return  	: success:0
 				  fail:ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
static ERR_CODE file_read(UINT8 *file, UINT32 read_size, UINT8 *filebuff)
{
	INT32 fd = 0;
	INT32 ret = 0;


	
	if((read_size == 0) || (filebuff == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	fd = open((char *)file, O_RDONLY);
	if (fd < 0) 
	{	 
		return ERR_FILE_OPEN_FAILED;
	}	

	ret = read(fd, filebuff, read_size);
	if(ret < 0)
	{
		return ERR_FILE_READ_FAILED; 
	}
	close(fd);

	return ERR_SUCCESS;
}



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
UINT32 DataHash( UINT8 *data, UINT32 len, HASH_TYPE hash_method, UINT8 *hash )
{
	UINT32 ret = 0;
	
    if((data == NULL) || (hash == NULL))
	{
		return 0;
	}

	switch(hash_method)
	{
		case HASH_MD5:
			Md5Calculate(data, len, hash);
			ret = MD5_HASH_SIZE;
			break;
		case HASH_SHA_256:
			//sha256_Raw(data, len, hash);
			hasher_Raw(HASHER_SHA2, data, len, hash);
			ret = HASHER_DIGEST_LENGTH;
			break;
		case HASH_SHA3K:
			//sha256_Raw(data, len, hash);
			hasher_Raw(HASHER_SHA3K, data, len, hash);
			ret = HASHER_DIGEST_LENGTH;
			break;
		default:
			ret = 0;
			break;
	}

	return ret;
}

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
UINT32 FileHash(UINT8 *file, HASH_TYPE hash_method, UINT8 *hash)
{
	UINT32 size;
	ERR_CODE ret = ERR_SUCCESS;
	UINT8 *filebuff = NULL;
	
    if((file == NULL) || (hash == NULL))
	{
		return 0;
	}

	/*1. get the file size*/
	size = file_size(file);
	if(size == 0)
	{
		DEBUG_PRINT("file:%s is NULL\r\n", file);
		return 0;
	}

	/*2.malloc space to store the file data*/
	filebuff = (UINT8 *)malloc(size);
	if(filebuff == NULL)
	{
		return 0;
	}
	memset(filebuff, 0, sizeof(size));

	/*3.read the file*/
	ret = file_read(file, size, filebuff);
	if(ret != ERR_SUCCESS)
	{
		DEBUG_PRINT("read file:%s fail\r\n", file);
		free(filebuff);
		filebuff = NULL;
		return 0;
	}

	/*4.Calculate the hash value*/
	ret = DataHash(filebuff, size, hash_method, hash);
	if(ret == 0)
	{
		DEBUG_PRINT("hash file:%s fail\r\n", file);
		free(filebuff);
		filebuff = NULL;
		return 0;
	}

	/*5.free the malloc space*/
	free(filebuff);
	filebuff = NULL;
	
	return ret;
}


/*****************************************************************************
 * @Function	: DataSignature
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
 * @return  	: the signature data
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE DataSignature(UINT8 *data, UINT32 data_len, SIGN_TYPE sign_method, 
					 UINT8 *pri_key, UINT8 signed_msg[65])
{
	ERR_CODE ret = ERR_SUCCESS;
	UINT8 recid = 0;
	
    if((data == NULL) || (pri_key == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	switch(sign_method)
	{
		case SIGN_ECDSA:
			/*3.ecdsa secp256k1 data signature*/
 			ret = secp256k1_sign(pri_key, data, data_len, 
								 signed_msg, &recid);
			if(ret != ERR_SUCCESS)
			{
				return ERR_SIGN_FAIL;
			}
			signed_msg[64] = recid;
			break;
		default:
			ret = ERR_SIGN_METHOD_NOT_SUPPORT;
			break;
	}

	return ret;
}



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
					    UINT8 *enc_key, UINT8 *cipher)
{
	ERR_CODE ret = ERR_SUCCESS;

	if((data == NULL) || (enc_key == NULL) || (cipher == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	switch(enc_method)
	{
		case ENC_TYPE_ECDSA:
			ret = ERR_SUCCESS;
			break;
		default:
			ret = ERR_HASH_METHOD_NOT_SUPPORT;
			break;
	}

	return ret;
}

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
					  ENC_TYPE enc_method, UINT8 *share_key)
{
	ERR_CODE ret = ERR_SUCCESS;

	if((r_pub_key == NULL) || (s_pri_key == NULL) || (share_key == NULL))
	{
		return ERR_PARAMETER_CHECK_FAILED;
	}

	switch(enc_method)
	{
		case ENC_TYPE_ECDSA:
			secp256k1_ecdh(s_pri_key, r_pub_key, share_key);
			break;
		default:
			ret = ERR_HASH_METHOD_NOT_SUPPORT;
			break;
	}

	return ret;
}

/*****************************************************************************
 * @Function	: PublicKeyRecover
 * @author		: xqchen
 * @date		: 2020-8-27
 * @brief       : Recover an ECDSA public key from a signature
 * @param[in]  	: UINT8 *sig      data signture
               	  UINT8 *msg      original data
               	  UINT32 msg_len  data length
 * @param[out]  : UINT8 *pub_key  a 65-byte array computed from the signature
 * @return  	: ERR_CODE
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
ERR_CODE PublicKeyRecover(ENC_TYPE enc_method, UINT8 *sig, UINT8 *msg, 
						  UINT32 msg_len, UINT8 *pub_key)
{
	ERR_CODE ret = ERR_SUCCESS;
	
	switch(enc_method)
	{
		case ENC_TYPE_ECDSA:
			secp256k1_recover(sig, msg, msg_len, sig[64], pub_key);
			break;
		default:
			ret = ERR_HASH_METHOD_NOT_SUPPORT;
			break;
	}
	
	return ret;
}


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
ERR_CODE KeyPairCreate(ENC_TYPE enc_method, UINT8 pri_key[32], UINT8 pub_key[65])
{
    ERR_CODE ret = ERR_SUCCESS;
	
	switch(enc_method)
	{
		case ENC_TYPE_ECDSA:
			ecdsa_keypair_create(pri_key, pub_key);
			break;
		default:
			ret = ERR_HASH_METHOD_NOT_SUPPORT;
			break;
	}
	
	return ret;
}


/*****************************************************************************
 * @Function	: DataBase64Decode
 * @author		: Preh
 * @date		: 2020-8-10
 * @brief		: decode private key coding by base64
 * @param[in]   : UINT8 *data 			data encoded by base64
 				  UINT32 data_len		data length				
 * @return		: success:	decoded data, the space must be freed manually. 
 				  fail:		NULL
 * @relation	: 
 * @other 	  	: 

*****************************************************************************/
UINT8 *DataBase64Decode(UINT8 *data, UINT32 data_len)
{
  UINT32 ret = 0;
  UINT32 out_len = 0;
  UINT8 *out = NULL;

  if(data == NULL)
  {
	  return NULL;
  }
  /*1.Calculate the private key length after decoding */
  out_len = BASE64_DECODE_OUT_SIZE(data_len);
  out = (UINT8 *)malloc(out_len);
  if(out == NULL)
  {
	  return NULL;
  }

  /*2.decode the private key*/
  ret = base64_decode((char *)data, data_len, out);
  if(ret != (out_len - 1))
  {
	  DEBUG_PRINT("Data:%s base64 decode fail, ret:%d\r\n", data, ret);
	  return NULL;
  }

  return out;
}

 /*****************************************************************************
  * @Function	 : DataBase64Encode
  * @author 	 : Preh
  * @date		 : 2020-8-10
  * @brief		 : encoding data by base64
  * @param[in]	 : UINT8* data	  	data to encode
				   UINT16 data_len  data length 
  * @param[out]  : None
  * @return 	 : encoded data by base64
  * @relation	 : 
  * @other		 : 
 
 *****************************************************************************/
 UINT8 *DataBase64Encode(UINT8* data, UINT32 data_len)
 {
   INT16 out_len;
   UINT8* encode_data = NULL;
   INT16 ret = 0; 
 
 
   /*1.Calculate the data length after encoding */
   out_len = BASE64_ENCODE_OUT_SIZE(data_len);
 
   
   /*2.malloc space to store the after encoding*/
   encode_data = (UINT8 *)malloc(out_len);
   if(encode_data == NULL)
   {
	   return NULL;
   }
   memset(encode_data, 0U, out_len);
 
   /*3.encoding data by Base64*/
   ret = base64_encode(data, data_len, (char *)encode_data);
   if(ret != (out_len - 1))
   {
	   DEBUG_PRINT("Data base64 encode fail, ret:%d\r\n", ret);
	   return NULL;
   }
   DEBUG_PRINT("encode_data:%s\r\n", encode_data);
 
   return encode_data;
 }
 


