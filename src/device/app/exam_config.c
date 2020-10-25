#include <string.h> 
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>

#include "types.h"
#include "config.h"
#include "exam_config.h"
#include "hexutils.h"

//#define CONFIG_DEBUG_ON	

#ifdef	CONFIG_DEBUG_ON
#define CONFIG_DEBUG(fmt, args...) printf(fmt, ##args);
#else
#define CONFIG_DEBUG(fmt, args...)
#endif	/* DEBUG */


section_key_string key_string[] = 
{
	{IS_SECTION_EXAM_STR, KEY_REGISTER_URL}, 
	{IS_SECTION_EXAM_STR, KEY_PLATFORM_BROKER_ADDR},
    {IS_SECTION_EXAM_STR, KEY_PLATFORM_BROKER_PORT}, 
	{IS_SECTION_EXAM_STR, KEY_PLATFORM_BROKER_USERNAME},
	{IS_SECTION_EXAM_STR, KEY_PLATFORM_BROKER_PWD},

	{IS_SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_ADDR},
    {IS_SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_PORT}, 
	{IS_SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_TOKEN},
	{IS_SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_URL},

	{IS_SECTION_STU_STR, KEY_STU_NAME}, 
	{IS_SECTION_STU_STR, KEY_STU_ID},  	
	{IS_SECTION_STU_STR, KEY_STU_PIC}, 
	{IS_SECTION_STU_STR, KEY_EXAM_ADDR}, 
	{IS_SECTION_STU_STR, KEY_EXAM_NAME}, 
	{IS_SECTION_STU_STR, KEY_EXAM_TIME}, 
	{IS_SECTION_STU_STR, KEY_EXAM_START_TIME}, 
	{IS_SECTION_STU_STR, KEY_EXAM_END_TIME}, 
	{IS_SECTION_STU_STR, KEY_GPEN_ID}, 
	{IS_SECTION_STU_STR, KEY_GPEN_PRIKEY}, 

	{IS_SECTION_GPEN_STR, KEY_GPEN_ID}, 
	{IS_SECTION_GPEN_STR, KEY_GPEN_PRIKEY}, 
	{IS_SECTION_GPEN_STR, KEY_GPEN_PUBKEY}, 

    {0,""},    
};

//段分割关键字定义
section_key_string section_string[] = 
{
    {IS_SECTION_EXAM_STR,    	SECTION_EXAM_STR},  
	{IS_SECTION_STU_STR,    	SECTION_STU_STR},  
	{IS_SECTION_GPEN_STR,    	SECTION_GPEN_STR},  
    {0,""},   
};

int read_config(char *config_file, exam_config *config)
{
	config_file_buf config_buf;
	int ret = 0;
	char value[512];

	memset(&config_buf, 0, sizeof(config_file_buf));
	ret = utl_load_file_into_config_buf(&config_buf, config_file, section_string, key_string);
	if(ret)
    {
		CONFIG_DEBUG("open config file %s fail:%d\r\n", config_file, ret);
        return ret;
    }

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_REGISTER_URL, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)config->register_url, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_REGISTER_URL, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_BROKER_ADDR, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)config->broker_addr, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_BROKER_ADDR, value);
		}
	}
	
	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_BROKER_PORT, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			config->broker_port = atoi(value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_BROKER_PORT, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_BROKER_USERNAME, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)config->broker_username, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_BROKER_USERNAME, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_BROKER_PWD, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)config->broker_pwd, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_BROKER_PWD, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_ADDR, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)config->file_put_addr, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_FILE_PUT_ADDR, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_TOKEN, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)config->file_put_token, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_FILE_PUT_TOKEN, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_URL, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)config->file_put_url, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_FILE_PUT_URL, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_EXAM_STR, KEY_PLATFORM_FILE_PUT_PORT, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			config->file_put_port = atoi(value);
			CONFIG_DEBUG("%s: %s\n", KEY_PLATFORM_FILE_PUT_PORT, value);
		}
	}

	//config_print(&config_buf);
	return 0;
}

int read_stu_info(char *config_file, stu_config *stu_info)
{
	config_file_buf config_buf;
	int ret = 0;
	char value[512];

	memset(&config_buf, 0, sizeof(config_file_buf));
	ret = utl_load_file_into_config_buf(&config_buf, config_file, section_string, key_string);
	if(ret)
    {
		CONFIG_DEBUG("open config file %s fail:%d\r\n", config_file, ret);
        return ret;
    }

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_STU_NAME, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->stu_name, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_STU_NAME, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_STU_ID, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->stu_id, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_STU_ID, value);
		}
	}
	
	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_STU_PIC, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->stu_pic, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_STU_PIC, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_ADDR, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->exam_addr, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_EXAM_ADDR, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_NAME, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->exam_name, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_EXAM_NAME, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_TIME, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->exam_time, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_EXAM_TIME, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_START_TIME, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->exam_start_time, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_EXAM_START_TIME, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_END_TIME, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->exam_end_time, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_EXAM_END_TIME, value);
		}
	}
	
	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_GPEN_ID, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->GpenId, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_GPEN_ID, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_STU_STR, KEY_GPEN_PRIKEY, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)stu_info->DevPriKey, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_GPEN_PRIKEY, value);
		}
	}

	//config_print(&config_buf);
	return 0;
}


int save_stu_info(char *config_file, stu_config *config)
{
	config_file_buf config_buf;
	int ret = 0;
	char value[512];

	memset(&config_buf, 0, sizeof(config_file_buf));


	if(strlen((char *)config->stu_name))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_STU_NAME, 
								  config->stu_name, section_string, key_string, 0);
		if(ret < 0)
		{
			return ret;
		}
	}

	if(strlen((char *)config->stu_id))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_STU_ID, 
								  config->stu_id, section_string, key_string, ret);
		if(ret < 0)
		{
			return ret;
		}
	}

	if(strlen((char *)config->stu_pic))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_STU_PIC, 
								  config->stu_pic, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	if(strlen((char *)config->exam_addr))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_ADDR, 
								  config->exam_addr, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	if(strlen((char *)config->exam_name))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_NAME, 
								  config->exam_name, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	if(strlen((char *)config->exam_time))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_TIME, 
								  config->exam_time, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	if(strlen((char *)config->exam_start_time))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_START_TIME, 
								  config->exam_start_time, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}


	if(strlen((char *)config->exam_end_time))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_EXAM_END_TIME, 
								  config->exam_end_time, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	if(strlen((char *)config->GpenId))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_GPEN_ID, 
								  config->GpenId, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	if(strlen((char *)config->DevPriKey))
	{
		ret = utl_set_config_item(&config_buf, SECTION_STU_STR, KEY_GPEN_PRIKEY, 
								  config->DevPriKey, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	if(strlen((char *)config->stu_name))
	{
		ret = utl_save_config_buf_to_file(&config_buf, config_file);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	//config_print(&config_buf);
	return 0;
}

int read_gpen_info(char *config_file, gpen_config *gpen_info)
{
	config_file_buf config_buf;
	int ret = 0;
	char value[512];

	memset(&config_buf, 0, sizeof(config_file_buf));
	ret = utl_load_file_into_config_buf(&config_buf, config_file, section_string, key_string);
	if(ret)
    {
		CONFIG_DEBUG("open config file %s fail:%d\r\n", config_file, ret);
        return ret;
    }

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_GPEN_STR, KEY_GPEN_ID, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)gpen_info->GpenId, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_GPEN_ID, value);
		}
	}

	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_GPEN_STR, KEY_GPEN_PRIKEY, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)gpen_info->DevPriKey, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_GPEN_PRIKEY, value);
		}
	}
	
	memset(value, 0, sizeof(value));
	ret = utl_get_config_item(&config_buf, SECTION_GPEN_STR, KEY_GPEN_PUBKEY, value, 0);
	if(ret > 0)
	{
		if(strlen(value) > 0)
		{
			sprintf((char *)gpen_info->DevPubKey, "%s", value);
			CONFIG_DEBUG("%s: %s\n", KEY_GPEN_PUBKEY, value);
		}
	}
	return 0;
}

int save_gpen_info(char *config_file, gpen_config *config)
{
	config_file_buf config_buf;
	int ret = 0;
	char value[512];

	memset(&config_buf, 0, sizeof(config_file_buf));


	if(strlen((char *)config->GpenId))
	{
		ret = utl_set_config_item(&config_buf, SECTION_GPEN_STR, KEY_GPEN_ID, 
								  config->GpenId, section_string, key_string, 0);
		if(ret < 0)
		{
			return ret;
		}
	}

	if(strlen((char *)config->DevPriKey))
	{
		ret = utl_set_config_item(&config_buf, SECTION_GPEN_STR, KEY_GPEN_PRIKEY, 
								  config->DevPriKey, section_string, key_string, ret);
		if(ret < 0)
		{
			return ret;
		}
	}

	if(strlen((char *)config->DevPubKey))
	{
		ret = utl_set_config_item(&config_buf, SECTION_GPEN_STR, KEY_GPEN_PUBKEY, 
								  config->DevPubKey, section_string, key_string, ret);
		if(ret < 0)
	    {
	        return ret;
	    }
	}

	ret = utl_save_config_buf_to_file(&config_buf, config_file);
	if(ret < 0)
    {
        return ret;
    }

	//config_print(&config_buf);
	return 0;
}


