#ifndef __EXAM_CONFIG_H__
#define __EXAM_CONFIG_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"


#define SECTION_EXAM_STR				"[exam]"
#define KEY_REGISTER_URL               	"register_url"
#define KEY_PLATFORM_BROKER_ADDR		"broker_addr"
#define KEY_PLATFORM_BROKER_PORT        "broker_port"
#define KEY_PLATFORM_BROKER_USERNAME    "broker_username"
#define KEY_PLATFORM_BROKER_PWD         "broker_pwd"
#define KEY_PLATFORM_FILE_PUT_ADDR		"file_put_addr"
#define KEY_PLATFORM_FILE_PUT_PORT		"file_put_port"
#define KEY_PLATFORM_FILE_PUT_TOKEN		"file_put_token"
#define KEY_PLATFORM_FILE_PUT_URL		"file_put_url"



#define SECTION_STU_STR					"[stu]"
#define KEY_STU_NAME 					"StuName"
#define KEY_STU_ID 						"StuIDCardNo"
#define KEY_GPEN_ID 					"GpenID"
#define KEY_GPEN_PRIKEY					"DevPriKey"
#define KEY_STU_PIC 					"StuPic"
#define KEY_EXAM_ADDR 					"ExamAddr"
#define KEY_EXAM_NAME 					"ExamName"
#define KEY_EXAM_TIME 					"ExamTime"
#define KEY_EXAM_START_TIME 			"ExamStartTime"
#define KEY_EXAM_END_TIME 				"ExamEndTime"

#define SECTION_GPEN_STR				"[gpen]"
#define KEY_GPEN_PUBKEY					"DevPubKey"

		
#define IS_SECTION_START                1
#define IS_SECTION_EXAM_STR       		2
#define IS_SECTION_STU_STR       		3
#define IS_SECTION_GPEN_STR       		4

#define IS_SECTION_END                  6

typedef struct exam_config_t
{
	UINT8 register_url[128];
	UINT8 broker_addr[32];
	UINT32 broker_port;
	UINT8 broker_username[32];
	UINT8 broker_pwd[32];
	UINT8 file_put_addr[32];
	UINT32 file_put_port;
	UINT8 file_put_token[32];
	UINT8 file_put_url[128];
}exam_config;

typedef struct stu_config_t
{
	UINT8 DevPriKey [65];
	UINT8 GpenId[32];
	UINT8 stu_name[64];
	UINT8 stu_id[20];
	UINT8 stu_pic[64];
	UINT8 exam_addr[64];
	UINT8 exam_name[64];
	UINT8 exam_time[64];
	UINT8 exam_start_time[64];
	UINT8 exam_end_time[64];
}stu_config;

typedef struct gpen_config_t
{
	UINT8 DevPriKey [65];
	UINT8 GpenId[32];
	UINT8 DevPubKey [132];
}gpen_config;


extern exam_config config;

int read_config(char *config_file, exam_config *config);
int read_stu_info(char *config_file, stu_config *stu_info);
int save_stu_info(char *config_file, stu_config *config);
int save_gpen_info(char *config_file, gpen_config *config);
int read_gpen_info(char *config_file, gpen_config *gpen_info);


#ifdef __cplusplus
};
#endif

#endif

