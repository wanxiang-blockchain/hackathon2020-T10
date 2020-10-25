#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "types.h"


typedef struct http_respone_t
{
	char *data;// 文件数据保存在内存中的指�? 用完后手动free 
	int data_len;
}http_respone;

/*HTTP content type.				*/
/*Because the uploading of files	*/ 
/*needs to be realized by post form,*/ 
/*this structure is defined here.	*/
typedef enum
{
	HTTP_CONTENT_TEXT_PLAIN,
	HTTP_CONTENT_OCTET_STREAM,
}HTTP_CONTENT_TYPE;


/*HTTP post form structure. 		*/
/*Because the uploading of files	*/ 
/*needs to be realized by post form,*/ 
/*this structure is defined here.	*/
typedef struct _HTTP_POST_FORM
{
	HTTP_CONTENT_TYPE content_type;
	UINT8 *content;
	UINT8 content_name[64];
	UINT8 content_encoding[64];
}HTTP_POST_FORM, *P_HTTP_POST_FORM;


int http_post_file(char *url, HTTP_POST_FORM *form, UINT32 form_num, void *data_out);
int http_post(char *url, char *data_in, int len, void *data_out);
int http_get(char *url, void *data_out);
int http_init(void);

#endif

