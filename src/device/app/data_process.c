/***********************************************************************************
 * @file   			: exam_simulate.c
 * @author   		: xqchen
 * @date   			: 2020-10-24
 * @Description   	: 本文件主要实现模拟考试场景数据处理、区块链存证等
 * @Copyright   	: Copyright (c) 2008-2020   Shanghai Wanxiang Blockchain Inc.
 * @other   		: 
 * @note History 	: 
***********************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <sys/time.h>

#include "types.h"
#include "data_process.h"
#include "exam_config.h"
#include "cJSON.h"
#include "http_client.h"
#include "hexutils.h"
#include "mqtt_api.h"
#include "application_api.h"
#include "algorithm_api.h"




extern exam_config config;

#if 0
UINT32 tlv_add_data_to_package(data_package *package, DATA_TYPE_E type, UINT32 value_len, UINT8* value)
{
	TLV_package *root = NULL;

	if((package == NULL) || (value == NULL))
	{
		return 0;
	}

	root = (TLV_package *)package->data;

	add_tlv(root, type, value_len, value);

	package->data_len = root->header.len;
	return package->data_len;
}

UINT8 *tlv_get_data_from_package(data_package *package, DATA_TYPE_E type, UINT32 *len)
{
	TLV_package *root = NULL;

	if((package == NULL) || (len == NULL))
	{
		return NULL;
	}
	root = (TLV_package *)package->data;
	
	return get_tlv(root, type, len);
}
#endif


/*****************************************************************************
 * @Function	: json_add_data_to_package
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现向JSON数据包中增加一条数据内容
 * @param[in]  	: data_package *package  待操作的JSON数据包
               	  DATA_TYPE_E type       待增加的数据类型
               	  UINT32 value_len       待增加的数据长度
               	  UINT8* value           待增加的数据内容
 * @param[out]  : None
 * @return  	: 修改后的数据包长
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 json_add_data_to_package(data_package *package, DATA_TYPE_E type, UINT32 value_len, UINT8* value)
{
	cJSON *root = NULL;
	UINT8 *json_str = NULL;

	if((package == NULL) || (value == NULL))
	{
		return 0;
	}

	if(package->data_len == 0)
	{
		root = cJSON_CreateObject();	
	    if(root == NULL)
	    {
	        return 0;
	    }
	}
	else
	{
		root = cJSON_Parse((char *)package->data);
		if(root == NULL)
		{
			return 0;
		}
	}

	switch (type)
	{
		case PKG_DATA_TYPE_GPEN_ID:
			cJSON_AddItemToObject(root, JSON_EXAM_GPEN_ID,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_GPEN_PUBKEY:
			cJSON_AddItemToObject(root, JSON_EXAM_GPEN_PUBKEY,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_STU_NAME:
			cJSON_AddItemToObject(root, JSON_EXAM_STU_NAME,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_STU_ID:
			cJSON_AddItemToObject(root, JSON_EXAM_STU_ID,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_TOPIC:
			cJSON_AddItemToObject(root, JSON_EXAM_TOPIC,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_ANSWER:
			cJSON_AddItemToObject(root, JSON_EXAM_ANSWER,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_SIGN:	
			cJSON_AddItemToObject(root, JSON_EXAM_SIGN,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_POE_HASH:
			cJSON_AddItemToObject(root, JSON_EXAM_POE_HASH,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_RESULT:	
			cJSON_AddItemToObject(root, JSON_EXAM_RESULT,		  
						  cJSON_CreateString((char *)value));
			break;

		case PKG_DATA_TYPE_EXAM_ADDR:
			cJSON_AddItemToObject(root, JSON_EXAM_ADDR,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_EXAM_SUBJECT:
			cJSON_AddItemToObject(root, JSON_EXAM_SUBJECT,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_EXAM_START_TIME:	
			cJSON_AddItemToObject(root, JSON_EXAM_START_TIME,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_EXAM_CURR_TIME:
			cJSON_AddItemToObject(root, JSON_EXAM_CURR_TIME,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_EXAM_END_TIME:	
			cJSON_AddItemToObject(root, JSON_EXAM_END_TIME,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_STU_PIC:
			cJSON_AddItemToObject(root, JSON_EXAM_STU_PIC,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_EXAM_NAME:
			cJSON_AddItemToObject(root, JSON_EXAM_NAME,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_EXAM_TIME:
			cJSON_AddItemToObject(root, JSON_EXAM_TIME,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_PIC_ID:
			cJSON_AddItemToObject(root, JSON_EXAM_PIC_ID,		  
						  cJSON_CreateString((char *)value));
			break;
		case PKG_DATA_TYPE_PIC_NAME:
			cJSON_AddItemToObject(root, JSON_EXAM_PIC_NAME,		  
						  cJSON_CreateString((char *)value));
			break;
	}

	json_str = (UINT8 *)cJSON_PrintUnformatted(root);	
    if(json_str == NULL)
    {
        cJSON_Delete(root);	
        return 0;
    }

	memset(package->data, 0, sizeof(package->data));
	package->data_len = strlen((char *)json_str); 
	memcpy(package->data, json_str, package->data_len);

	cJSON_Delete(root);	
	free(json_str);

	return package->data_len;
}

/*****************************************************************************
 * @Function	: json_get_data_from_package
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现从JSON数据包中取出一条数据内容
                  
 * @param[in]  	: data_package *package  待处理的JSON数据包
               	  DATA_TYPE_E type       待获取的数据类型
 * @param[out]  : UINT32 *len            待获取的数据长度
 * @return  	: 待获取的数据内容
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT8 *json_get_data_from_package(data_package *package, DATA_TYPE_E type, UINT32 *len)
{
	cJSON *root = NULL;
	cJSON *tmp = NULL;
	UINT8 *value = NULL;

	if((package == NULL) || (len == NULL))
	{
		return 0;
	}

	root = cJSON_Parse((char *)package->data);
	if(root == NULL)
	{
		return NULL;
	}

	switch (type)
	{
		case PKG_DATA_TYPE_GPEN_ID:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_GPEN_ID);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_GPEN_PUBKEY:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_GPEN_PUBKEY);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_STU_NAME:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_STU_NAME);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_STU_ID:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_STU_ID);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_TOPIC:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_TOPIC);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_ANSWER:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_ANSWER);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_SIGN:	
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_SIGN);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_POE_HASH:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_POE_HASH);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_RESULT:	
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_RESULT);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;

		case PKG_DATA_TYPE_EXAM_ADDR:	
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_ADDR);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;

		case PKG_DATA_TYPE_EXAM_SUBJECT:	
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_SUBJECT);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;

		case PKG_DATA_TYPE_EXAM_START_TIME:	
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_START_TIME);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;

		case PKG_DATA_TYPE_EXAM_CURR_TIME:	
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_CURR_TIME);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_EXAM_END_TIME:	
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_END_TIME);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;

		case PKG_DATA_TYPE_STU_PIC:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_STU_PIC);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_EXAM_NAME:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_NAME);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_EXAM_TIME:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_TIME);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;

		case PKG_DATA_TYPE_PIC_ID:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_PIC_ID);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
		case PKG_DATA_TYPE_PIC_NAME:
			tmp = NULL;
			tmp = cJSON_GetObjectItem(root, JSON_EXAM_PIC_NAME);
			if((tmp != NULL) && !(cJSON_IsNull(tmp)))
			{
				*len = strlen(tmp->valuestring);
				value = malloc(*len + 1);
				if(value == NULL)
				{
					cJSON_Delete(root); 
					return NULL;
				}
				sprintf((char *)value, "%s", tmp->valuestring);
			}
			break;
	}

	cJSON_Delete(root);	

	return value;
}

/*****************************************************************************
 * @Function	: GetPicPrepare
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现摄像头相关提示
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void GetPicPrepare()
{
	char cUserChoose[1024];
	
	printf("请正视摄像头，系统将采集您的照片\n");
	printf("准备好之后，请按任意键继续\n");
	memset(cUserChoose, 0, sizeof(cUserChoose));
	fgets(cUserChoose, sizeof(cUserChoose), stdin);
}

/*****************************************************************************
 * @Function	: GetPic
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现调取摄像头拍照功能
 * @param[in]  	: None
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void GetPic(UINT32 with_info, UINT8 *picname)
{
	UINT8 cmd[128];
	memset(cmd, 0, sizeof(cmd));

	if(with_info)
	{
		GetPicPrepare();
	}
	
	sprintf(cmd, "fswebcam -S 5 %s 2>null", picname);
	system(cmd);
	if(with_info)
	{
		printf("图片采集成功.\r\n");
	}
}

/*****************************************************************************
 * @Function	: GetPicId
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现从图片服务器返回的消息体中提-
                  取图片ID及文件名等信息
 * @param[in]  	: UINT8 *body     图片服务器返回的消息体
               	  UINT8 *picname  图片文件名
 * @param[out]  : None
 * @return  	: INT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
INT32 GetPicId(UINT8 *body, UINT8 *picname)
{
	cJSON *root = NULL;
	cJSON *tmp = NULL;
	cJSON *event_Item = NULL;
	ERR_CODE ret = ERR_SUCCESS;
	INT32 pic_id = -1;

	if(body == NULL)
	{
		return -1;
	}

	root = cJSON_Parse((char *)body);
	if(root == NULL)
	{
		return -1;
	}

	/*1.Parse the return code and its description*/
	cJSON_ArrayForEach(event_Item, root)
	{
		tmp = NULL;
		tmp = cJSON_GetObjectItem(event_Item, "id");
		if((tmp != NULL) && !(cJSON_IsNull(tmp)))
		{
			pic_id = tmp->valueint;
		}

		tmp = NULL;
		tmp = cJSON_GetObjectItem(event_Item, "url");
		if((tmp != NULL) && !(cJSON_IsNull(tmp)))
		{
			sprintf(picname, "%s", tmp->valuestring);
			//device->deviceType = tmp->valueint;
			//printf("%s:%s\r\n", "url", picname);
		}
	}
	

	return pic_id;
}



/*****************************************************************************
 * @Function	: GetStuFile
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 获取考生信息文件名
 * @param[in]  	: UINT8 *filename  考生信息文件名
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void GetStuFile(UINT8 *filename)
{
	char cUserChoose[128];
	UINT8 tmp[128];

	#if 0
	printf("请输入考生信息：\r\n");
	memset(cUserChoose, 0, sizeof(cUserChoose));
	fgets(cUserChoose, sizeof(cUserChoose), stdin);

	memset(tmp, 0, sizeof(tmp));
	memcpy(tmp, cUserChoose, strlen(cUserChoose)-1);
	sprintf(filename, "data/stu/%s.ini", tmp);
	#else
	sprintf(filename, "data/stu/%s.ini", "lisi");
	#endif
	
}

/*****************************************************************************
 * @Function	: GetGpenId
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现获取智能笔的相关信息
 * @param[in]  	: UINT8 *GpenId     智能笔ID
               	  UINT8 *gpen_file  智能笔配置文件
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void GetGpenId(UINT8 *GpenId, UINT8 *gpen_file)
{
	char cUserChoose[128];

	#if 0
	printf("请输入智能笔编号：\r\n");
	memset(cUserChoose, 0, sizeof(cUserChoose));
	fgets(cUserChoose, sizeof(cUserChoose), stdin);

	memcpy(GpenId, cUserChoose, strlen(cUserChoose)-1);
	sprintf(gpen_file, "data/gpen/%s.ini", GpenId);
	#else
	char *tmp = "Gpen123";
	memcpy(GpenId, tmp, strlen(tmp));
	sprintf(gpen_file, "data/gpen/%s.ini", tmp);
	
	#endif
}



/*****************************************************************************
 * @Function	: GetTimeString
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现获取系统当前时间
 * @param[in]  	: UINT8 *str  时间字符串
 * @param[out]  : None
 * @return  	: void
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
void GetTimeString(UINT8 *str)
{
	time_t now;
	time(&now);
	struct tm *local;
	local = localtime(&now);

	sprintf(str,"%04d-%02d-%02d_%02d:%02d:%02d", local->tm_year+1900, 
														local->tm_mon+1,
														local->tm_mday, 
														local->tm_hour, 
														local->tm_min, 
														local->tm_sec);
}

/*****************************************************************************
 * @Function	: AddDataToPackage
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现向数据包中增加一个字段的数据
 * @param[in]  	: data_package *package  数据包
               	  DATA_TYPE_E type       数据类型
               	  UINT32 value_len       数据长度
               	  UINT8* value           待增加的数据
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 AddDataToPackage(data_package *package, DATA_TYPE_E type, UINT32 value_len, UINT8* value)
{
	UINT32 ret = 0;

	ret = json_add_data_to_package(package, type, value_len,  value);

	return ret;
}

/*****************************************************************************
 * @Function	: GetDataFromPackage
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现从数据包中获取一个字段的数据
 * @param[in]  	: data_package *package  数据包
               	  DATA_TYPE_E type       数据类型
               	  UINT32 *len            数据长度
 * @param[out]  : None
 * @return  	: UINT8
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT8 *GetDataFromPackage(data_package *package, DATA_TYPE_E type, UINT32 *len)
{
	return json_get_data_from_package(package, type, len);
}

/*****************************************************************************
 * @Function	: FilePutToPlatform
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现向服务端发送一张图片，并返回-
                  改图片在服务端存储的ID及文件名
 * @param[in]  	: UINT8 *filename  文件名
               	  UINT8 *picname   图片名
 * @param[out]  : None
 * @return  	: INT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
INT32 FilePutToPlatform(UINT8 *filename, UINT8 *picname)
{
	HTTP_POST_FORM form;
	http_respone post_return;
	ERR_CODE ret = ERR_SUCCESS;
	UINT8 url[128];
	INT32 pic_id;

	memset(url, 0, sizeof(url));
	sprintf(url, "http://%s:%d/%s?token=%s", config.file_put_addr, 
											 config.file_put_port,
											 config.file_put_url,
											 config.file_put_token);
	memset(&form, 0, sizeof(form));

	/*fill in file name */
	form.content_type = HTTP_CONTENT_OCTET_STREAM;
	sprintf((char *)form.content_name, "%s", "files");
	form.content = (UINT8 *)malloc(strlen((char *)filename) + 1);
	if(form.content == NULL)
	{
		return -1;
	}
	memset(form.content, 0, strlen((char *)filename) + 1);
	sprintf((char *)form.content, "%s", filename);

	memset(&post_return, 0, sizeof(post_return));
	ret = http_post_file(url, &form, 1, (void *)&post_return);
	if(ret != ERR_SUCCESS)
	{
		if(form.content)
		{
			free(form.content);
			form.content = NULL;
		}
		free(form.content);
		if(post_return.data)
		{
			free(post_return.data);
			post_return.data = NULL;
		}
		return -1;
	}

	pic_id = GetPicId(post_return.data, picname);
	if(pic_id <= 0)
	{
		if(form.content)
		{
			free(form.content);
			form.content = NULL;
		}

		if(post_return.data)
		{
			free(post_return.data);
			post_return.data = NULL;
		}
		return -1;
	}

	if(form.content)
	{
		free(form.content);
		form.content = NULL;
	}

	if(post_return.data)
	{
		free(post_return.data);
		post_return.data = NULL;
	}

	//printf("图片保存业务平台成功.\r\n");
	return pic_id;
}


/*****************************************************************************
 * @Function	: PkgPoESave
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现数据包区块链存证
 * @param[in]  	: data_package *package  数据包
               	  UINT8 *access_id       存证系统接入ID
               	  UINT8 *access_key      存证系统存证秘钥
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 PkgPoESave(data_package *package, UINT8 *access_id, UINT8 *access_key)
{
	POE_SAVE_RESPONE save_resp;
	UINT8 tmp_str[132];
	UINT32 ret = ERR_SUCCESS;

	/*1.将整个数据包，哈希存证*/
	memset(&save_resp, 0, sizeof(save_resp));
	//printf("package:%s\r\n", package->data);
	TextHashPoeSave(access_id, access_key, (UINT8 *)package->data, package->data_len, &save_resp);
	//printf("SaveRetCode:%d\r\n", save_resp.SaveRetCode);
	//printf("SaveRetDesc:%s\r\n", save_resp.SaveRetDesc);
	//printHexData("poe hash:", save_resp.SavePoeHash, save_resp.HashLen);

	/*2.将哈希存证凭证，添加到数据包中*/
	memset(tmp_str, 0, sizeof(tmp_str));
	HexToStr((char *)tmp_str, save_resp.SavePoeHash, save_resp.HashLen);
	AddDataToPackage(package, PKG_DATA_TYPE_POE_HASH, strlen(tmp_str), tmp_str);

	printf("数据区块链存证成功, 存证凭证：%s.\r\n", tmp_str);
	/*3.释放相关资源*/
	PoeSaveResponeDestroy(&save_resp);
	
	return 0;
}

/*****************************************************************************
 * @Function	: ExamDataToPlatform
 * @author		: xqchen
 * @date		: 2020-10-24
 * @brief       : 本函数实现向服务端发送一个数据包
 * @param[in]  	: data_package *package  数据包
               	  PKG_TYPE_E pkg_type    数据包类型
 * @param[out]  : None
 * @return  	: UINT32
 * @relation  	: 
 * @other  		: 

*****************************************************************************/
UINT32 ExamDataToPlatform(data_package *package, PKG_TYPE_E pkg_type)
{
	MQTT_PARAM para;
	UINT8 topic[32];
	UINT8 *tmp = NULL;
	UINT32 len = 0;

	memset(topic, 0, sizeof(topic));
	memset(&para, 0, sizeof(MQTT_PARAM));

	switch (pkg_type)
	{
		case PKG_TYPE_STU_REG:
			tmp = GetDataFromPackage(package, PKG_DATA_TYPE_STU_ID, &len);
			if(tmp)
			{
				//printf("tmp:%s\r\n", tmp);
				sprintf(topic, "/register/%s/stu", tmp);
				para.clientid 	= tmp;
			}
			break;
		case PKG_TYPE_GPEN_REG:
			tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
			if(tmp)
			{
				//printf("tmp:%s\r\n", tmp);
				sprintf(topic, "/register/%s/gpen", tmp);
				para.clientid 	= tmp;
			}
			break;
		case PKG_TYPE_GPEN_STU_REG:
			tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
			if(tmp)
			{
				//printf("tmp:%s\r\n", tmp);
				sprintf(topic, "/exam/%s/register", tmp);
				para.clientid 	= tmp;
			}
			break;
		case PKG_TYPE_EXAM_START:
			tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
			if(tmp)
			{
				//printf("tmp:%s\r\n", tmp);
				sprintf(topic, "/exam/%s/start", tmp);
				para.clientid 	= tmp;
			}
			break;
		case PKG_TYPE_EXAM_ANSWER:
			tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
			if(tmp)
			{
				//printf("tmp:%s\r\n", tmp);
				sprintf(topic, "/exam/%s/answer", tmp);
				para.clientid 	= tmp;
			}
			break;
		case PKG_TYPE_EXAM_CHECK:
			tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
			if(tmp)
			{
				//printf("tmp:%s\r\n", tmp);
				sprintf(topic, "/exam/%s/check", tmp);
				para.clientid 	= tmp;
			}
			break;
		case PKG_TYPE_END:
			tmp = GetDataFromPackage(package, PKG_DATA_TYPE_GPEN_ID, &len);
			if(tmp)
			{
				//printf("tmp:%s\r\n", tmp);
				sprintf(topic, "/exam/%s/end", tmp);
				para.clientid 	= tmp;
			}
			break;
	}
	
	para.host 		= config.broker_addr;
	para.port 		= config.broker_port;
	para.username 	= config.broker_username;
	para.password 	= config.broker_pwd;
	para.qos = 0;

	//printf("topic:%s\r\n", topic);
	//printf("data:%s\r\n", package->data);

	MqttPublish(topic, package->data, package->data_len, &para);

	printf("数据保存业务平台成功.\r\n");

	return 0;
}


