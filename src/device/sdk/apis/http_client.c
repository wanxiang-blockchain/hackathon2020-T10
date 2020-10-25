#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "types.h" 

#include "curl/curl.h"
#include "curl/easy.h"
#include "http_client.h" 

size_t curl_write_data_cb(void *buffer, size_t size, size_t nmemb, void *stream)
{
	http_respone *tmp = NULL;
	int len = size * nmemb;
	
	
	if (stream)
	{
		tmp = (http_respone *)stream;
		
		
		tmp->data = realloc(tmp->data, tmp->data_len + len + 1);	
		if (!tmp->data)
		{
			printf("%s[%d]: realloc failed!!\n", __FUNCTION__, __LINE__);
			return 0;
		}
		memcpy(tmp->data + tmp->data_len, buffer, len);
		tmp->data_len += len;
		tmp->data[tmp->data_len] = '\0';
	}
	
	return len;
}
 
int http_post(char *url, char *data_in, int len, void *data_out)
{
	http_respone *respone = (http_respone *)data_out;
	CURL *curl_handle = NULL;
	CURLcode curl_res;
	int ret = 0;

	
	curl_handle = curl_easy_init();
	if (!curl_handle)
	{
		printf("%s[%d]: curl easy init failed\n", __FUNCTION__, __LINE__);
		return ret;
	}
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
	//curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);	
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, len);
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data_in);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 30);
	curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10L);

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_data_cb);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, respone);
	curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0L);

	struct curl_slist *pList = NULL;
	pList = curl_slist_append(pList,"Content-Type: application/json;charset=utf-8");

	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, pList);
	curl_res = curl_easy_perform(curl_handle);
	if(curl_res != CURLE_OK)
	{
		printf("curl_easy_perform error, err_msg:[%d]\n", curl_res);
		ret = -1;
	}

	if(curl_handle)
	{
		curl_easy_cleanup(curl_handle);
	}

	if(pList)
	{
		curl_slist_free_all(pList);
	}

	#if 1
	if (respone->data)		
	{
		//printf("data_len:%d\r\n", respone->data_len);
		DEBUG_PRINT("respone data:\r\n%s\r\n", respone->data);
	}
	#endif
	return 0;
}

int http_post_file(char *url, HTTP_POST_FORM *form, UINT32 form_num, void *data_out)
{
	http_respone *respone = (http_respone *)data_out;
	CURL *curl_handle = NULL;
	CURLcode curl_res;
	int ret = 0;
	//CURLFORMcode formCode;
	uint8_t timeout = 15U;
	UINT32 i = 0;
	
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;

	//curl_global_init(CURL_GLOBAL_ALL);
	 /* Fill in the file upload field */ 
	for(i = 0; i < form_num; i++)
	{
		if(form[i].content_type == HTTP_CONTENT_OCTET_STREAM)
		{
			curl_formadd(&formpost,
 		                 &lastptr,
 		                 CURLFORM_COPYNAME, form[i].content_name,
 		                 CURLFORM_FILE, form[i].content,
 		                 CURLFORM_END);
		}
		else
		{
			curl_formadd(&formpost,
   		                 &lastptr,
   		                 CURLFORM_COPYNAME, form[i].content_name,
   		                 CURLFORM_COPYCONTENTS, form[i].content,
   		                 CURLFORM_END);
		}
	}

	curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "submit",
                 CURLFORM_COPYCONTENTS, "send",
                 CURLFORM_END);

	curl_handle = curl_easy_init();
	if (!curl_handle)
	{
		printf("%s[%d]: curl easy init failed\n", __FUNCTION__, __LINE__);
		return ret;
	}

	
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_data_cb);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, respone);
	curl_easy_setopt(curl_handle, CURLOPT_HEADER, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, timeout);
	
	headerlist = curl_slist_append(headerlist,"Content-Type: application/octet-stream");
	if(curl_handle) 
	{
	    /* what URL that receives this POST */ 
	    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	    //curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headerlist);

	    curl_easy_setopt(curl_handle, CURLOPT_HTTPPOST, formpost);
	 
	    /* Perform the request, res will get the return code */ 
	    curl_res = curl_easy_perform(curl_handle);
	    /* Check for errors */ 
	    if(curl_res != CURLE_OK)
	      fprintf(stderr, "curl_easy_perform() failed: %s\n",
	              curl_easy_strerror(curl_res));
	 
	    /* always cleanup */ 
	    curl_easy_cleanup(curl_handle);
	 
	    /* then cleanup the formpost chain */ 
	    curl_formfree(formpost);
	    /* free slist */ 
	    curl_slist_free_all(headerlist);
 	}


	#if 1
	if (respone->data)		// 若要对返回的内容进行处理, 可在此处�?
	{
		//printf("data_len:%d\r\n", respone->data_len);
		DEBUG_PRINT("respone data:\r\n%s\r\n", respone->data);
	}
	#endif
	return 0;
}


int http_get(char *url, void *data_out)
{
	//创建curl对象 
	http_respone *respone = (http_respone *)data_out;
	CURL *curl_handle = NULL;
	CURLcode curl_res;
	int ret = 0;
	
	//curl初始�?
	curl_handle = curl_easy_init(); 
	if (!curl_handle)
	{
		printf("%s[%d]: curl easy init failed\n", __FUNCTION__, __LINE__);
		return -1;
	}

	curl_easy_setopt(curl_handle, CURLOPT_HEADER,0);	

	curl_easy_setopt(curl_handle, CURLOPT_URL,url);	


	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION,1);	

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, curl_write_data_cb); 	

	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, respone);	

	curl_easy_setopt(curl_handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);	

	curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10L); 	

	curl_easy_setopt(curl_handle,CURLOPT_TIMEOUT, 30);

	curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1);		

	
	curl_res = curl_easy_perform(curl_handle); 
	if(curl_res != CURLE_OK)
	{
		printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_res));
		ret  = -1;
	}

	#if 1
	if (respone->data)		
	{
		DEBUG_PRINT("respone data:\r\n%s\r\n", respone->data);
	}
	#endif

	curl_easy_cleanup(curl_handle);

	return ret;
}

int http_init(void)
{
	CURLcode curl_res;
	
	curl_res = curl_global_init(CURL_GLOBAL_ALL);

	//  printf("param is: %s\n", param);
	if(curl_res != CURLE_OK)
	{
		printf("CURL ERROR : %s", curl_easy_strerror(curl_res));
		return -1;
	}

	return 0;
}

