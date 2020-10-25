#include <string.h>
#include <stdlib.h>

#include "cJSON.h"
#include "application_api.h"
#include "goahead.h"


#define JSON_POE_HASH	"PoeHash"

int RequestPaser(UINT8 *body, UINT8 *PoeHash)
{
	cJSON *root = NULL;
	cJSON *tmp = NULL;

	if((body == NULL) || (PoeHash == NULL))
	{
		return -1;
	}

	root = cJSON_Parse((char *)body);
	if(root == NULL)
	{
		return -1;
	}

	/*1.Parse the return code and its description*/
	tmp = NULL;
	tmp = cJSON_GetObjectItem(root, JSON_POE_HASH);
	if((tmp != NULL) && !(cJSON_IsNull(tmp)))
	{
		sprintf((char *)PoeHash, "%s", tmp->valuestring);
	}

	return 0;
}

void PoEQueryRecv(Webs *wp)
{	
	UINT8 PoeHash[64];
	UINT8 *result = NULL;
	UINT8 *access_id  =	"5f22143cf1d84c9e907125fee5897f07";
	UINT8 *access_key = "gnT3vEcdJ093jTjEMlf0fgMMzzznQdKxPtAF6laTojY=";
	
	memset(PoeHash, 0, sizeof(PoeHash));

	printf("wp->input.servp:%s\r\n", wp->input.servp);
	RequestPaser(wp->input.servp, PoeHash);

	websSetStatus(wp, 200);
    websWriteHeaders(wp, -1, 0);
    websWriteEndHeaders(wp);
	if(strlen(PoeHash))
	{
		result = TextHashPoeQueryTest(access_id, access_key, PoeHash);
		printf("result:%s\r\n", result);
		websWrite(wp,result);
	}
	else
	{
		websWrite(wp,"fail");
	}

    websFlush(wp, 0);
	websDone(wp);

	if(result)
	{
		free(result);
		result = NULL;
	}
}

