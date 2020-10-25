#include <stdio.h>      
#include <stdlib.h>     
#include <unistd.h>     
#include <string.h>

#include "exam_config.h"
#include "exam.h"
#include "algorithm_api.h"
#include "hexutils.h"
#include "register.h"


#define CONFIG_FILE	"config.ini"

exam_config config;

UINT8 *access_id  =	"5f22143cf1d84c9e907125fee5897f07";
UINT8 *access_key = "gnT3vEcdJ093jTjEMlf0fgMMzzznQdKxPtAF6laTojY=";


int main(int argc, char **argv)
{
	char cUserChoose[128];
	
	
	memset(&config, 0, sizeof(config));
	
	read_config(CONFIG_FILE, &config);

	StartExam();

    while ('q' != cUserChoose[0])
    {
        printf("\r\n");
		if(!exam_status_get())
		{
			printf("\r\n请输入：\r\n");
	        printf("       1, 考生报名过程模拟  \r\n");
			printf("       2, 智能笔注册过程模拟\r\n");
			printf("       3, 考前认证过程模拟\r\n");
			printf("       4, 考试开始模拟\r\n");
	        //printf("       5, 考试过程数据模拟\r\n");
			//printf("       6, 考试过程校验模拟\r\n");
	        printf("       5, 考试结束模拟\r\n");
			//printf("       8, 阅卷过程模拟\r\n");
	        printf("       q, 退出.\r\n");
	        printf("Input:");
		}

		memset(cUserChoose, 0, sizeof(cUserChoose));
		fgets(cUserChoose, sizeof(cUserChoose), stdin);
        switch (cUserChoose[0])
        {
	        case '1':
				ExamineeRegistration();
				printf("考试报名成功.");
	            break;
	        case '2':
				GpenRegister();
				printf("智能笔成功.");
	            break;
	        case '3':
				ExamineeGpenBind();
				printf("智能笔绑定成功.");
	            break;
			case '4':
				ExamStart();
	            break;
			#if 0
			case '5':
				Examing(access_id, access_key);
	            break;
			case '6':
				ExamCheck(access_id, access_key);
	            break;
			#endif
			case '5':
				ExamEnd(access_id, access_key);
	            break;
	        default:
	            break;
        }
    }

    return 0;
}


