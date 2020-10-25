#include <string.h> 
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>

#include "config.h"


/*******************************************************************************
 * Function	    : section_key_string_check                        	           *
 * Input	    : section_key_string *search_struct-�ַ�����ָ��               *
 *		        : char *search_str-��У����ַ���				               *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����						               *
 * Comments	    : У���ַ���search_str���Ƿ����ַ�����search_struct��          *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
static int section_key_string_check (section_key_string *search_struct, char *search_str, int section_id)
{
    section_key_string *pnt = NULL;

    if(search_struct == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if(search_str == NULL)
    {
        return INI_PARAM_ERROR;
    }

    for(pnt = search_struct; pnt->section_key != 0; pnt++) 
    {
        if(strcmp(pnt->section_key_str, search_str) == 0) 
        {
            if(section_id != 0)
            {
                if(pnt->section_key != section_id)
                {
                    continue;
                }
            }
            return pnt->section_key;
        }
    }

    return INI_GENERAL_ERROR;
}

/*******************************************************************************
 * Function	    : delete_blank                        	                       *
 * Input	    : char *str-��������ַ���                                     *
 * Return	    : �������׵�һ���ǿ��ַ�����λ��					           *
 * Comments	    : ɾ���ַ���str����β�ո�TAB��                               *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
static int delete_blank(char *str)
{
	char *c = NULL;
	int n   = 0;
	int i   = 0;
    int ret = 0;

    if(str == NULL)
    {
        return INI_PARAM_ERROR;
    }
	
	c = str;
	n = strlen(str);
	
	for(i = 0; i < n; i++)
	{
		if ((c[i] != ' ') && (c[i] != '\t'))
		{
			break;	
		}
	}
	str = &c[i];
    ret = i;

	n = strlen(str);
	c = str;
	
	for(i = n-1; i >= 0; i--)
	{
		if ((c[i] != ' ') && (c[i] != '\t') && (c[i] != '\n') && (c[i] != '\r'))
		{
			break;
		}
	}
	
	c[i+1] = '\0';
	
	return ret;
}

/*******************************************************************************
 * Function	    : delete_comments                      	                       *
 * Input	    : char *str-��������ַ���                                     *
 * Return	    : �������׵�һ���ǿ��ַ�����λ��					           *
 * Comments	    : ɾ���ַ���str����β�ո�TAB��                               *
 *              : ȥ����";"��ʽע��                                            *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
static int delete_comments(char *str)
{
    int i   = 0;
    int ret = 0;

    if(str == NULL)
    {
        return INI_PARAM_ERROR;
    }
    
    while(str[i] != '\0')
    {
        if((str[i] == ';') || (str[i] == '\n'))
            str[i] = '\0';
        i++;
    }
    
    ret = delete_blank(str);
    if(ret < 0)
        return ret;
    
    i = 0;
    while(str[i+ret] != '\0')
    {
    	str[i] = str[i+ret];
    	i++;
    }
    str[i]='\0';
    
    return ret;
}


/*******************************************************************************
 * Function	    : is_section_str                                	           *
 * Input	    : section_key_string *section_struct-�����ַ�����ָ��          *
 *		        : char *str-��У����ַ���      				               *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����						               *
 * Comments	    : У���ַ���str���Ƿ��ڶ����ַ�����section_struct��            *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
static int is_section_str(char *str, section_key_string *section_struct)
{
    if(str == NULL)
    {
        return INI_PARAM_ERROR;
    }

    int ret     = 0;
    char *tmp   = NULL;

    if(str[0] != SECTION_LEFT_LABEL_CHAR)
    {
        return INI_NOT_SECTION_STR;
    }
    tmp = str + 1;
    tmp = strrchr(tmp, SECTION_RIGHT_LABEL_CHAR);
    if(tmp != NULL)
    {
        tmp = tmp + 1;
        if(strlen(tmp) != 0)
        {
            return INI_SECTION_STR_ERR;
        }
    }
    else
    {
        return INI_SECTION_STR_ERR;
    }
    if(section_struct != NULL)
    {
        ret = section_key_string_check(section_struct, str, 0);
    }
    return ret;
    
}


/*******************************************************************************
 * Function	    : is_key_str                                    	           *
 * Input	    : section_key_string *key_struct-�ؼ������ַ�����ָ��          *
 *		        : char *str-��У����ַ���      				               *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����						               *
 * Comments	    : У���ַ���str���Ƿ��ڹؼ��������ַ�����key_struct��          *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
static int is_key_str(char *str, section_key_string *key_struct, int section_id)
{
    if(str == NULL)
    {
        return INI_PARAM_ERROR;
    }

    int ret     = 0;
    
    if(key_struct != NULL)
    {
        ret = section_key_string_check(key_struct, str, section_id);
    }

    return ret;
    
}

/*******************************************************************************
 * Function	    : set_section_text                       			           *
 * Input        : config_file_buf *st_file_buf - Ҫ����������ļ��ṹָ��	   *
 *              : int section_num - �ö�����Ҫ���������	                   *
 *              : int size - �ö����ַ�������	                               *
 *      	    : const char *text - �ö����ַ�������			               *
 * Return	    : ����ɹ�����0��ʧ�ܷ���-1						               *
 * Comments	    : �������                  					               *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
static int set_section_text(config_file_buf *st_file_buf, int section_num, char *text, int size)
{
    if(st_file_buf == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if(text == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if((section_num < 0) || (section_num >= CONFIG_MAX_SECTION))
    {
        return INI_PARAM_ERROR;
    }

    if(size >= CONFIG_ROW_MAX_CHAR)
    {
        return INI_PARAM_ERROR;
    }
    
	memcpy(st_file_buf->section_buf[section_num].section_str, text, size);
    st_file_buf->section_buf[section_num].section_str[size+1] = '\0';
    st_file_buf->section_num = section_num;
	
	return INI_OK;
}


/*******************************************************************************
 * Function	    : delete_section_text                      			           *
 * Input        : config_file_buf *st_file_buf - Ҫ����������ļ��ṹָ��	   *
 *              : int section_num - �ö������ڵ�����    	                   *
 * Return	    : ����ɹ�����0��ʧ�ܷ���-1						               *
 * Comments	    : �������                  					               *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
static int delete_section_text(config_file_buf *st_file_buf, int section_num)
{
    if(st_file_buf == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if((section_num < 0) || (section_num >= CONFIG_MAX_SECTION))
    {
        return INI_PARAM_ERROR;
    }
    int total_rows  = 0;
    int i           = 0; 
    
    total_rows = st_file_buf->section_num;
    memset(&(st_file_buf->section_buf[section_num]), 0, sizeof(config_section_buf));
    if(section_num != total_rows)
    {
        for(i=section_num+1; i<=total_rows; i++)
        {
            memcpy(&(st_file_buf->section_buf[i-1]), &(st_file_buf->section_buf[i]), sizeof(config_section_buf));
            memset(&(st_file_buf->section_buf[i]), 0, sizeof(config_section_buf));
        }
    }
    st_file_buf->section_num = total_rows - 1;
	
	return INI_OK;
}


/*******************************************************************************
 * Function	    : config_key_search                          		           *
 * Input	    : config_section_buf *st_key_buf - �νṹָ��		           *
 *		        : const char *key - �����ҵļ�ֵ��  			               *
 * Return	    : ��ȡ�ɹ����ظü�ֵ������������ʧ�ܷ��ش�����	               *
 * Comments	    : ����ָ������key   					                       *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
static int config_key_search(config_section_buf *st_key_buf, const char *key)
{
	if(st_key_buf == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if(key == NULL)
	{
        return INI_PARAM_ERROR;
	}
	
	int i                       = 0;
    int	total_rows              = 0;
    int ret                     = 0;
    config_key_buf *tmp_key     = NULL;

    total_rows = st_key_buf->key_num;
    ret = total_rows + 1;
	for (i = 1; i <= total_rows; i++)
	{
        tmp_key = &(st_key_buf->key_buf[i]);
		if (strcmp(tmp_key->key_str, key) == 0)
		{
			ret = i;
			break;
		}
	}

	return ret;
}

/*******************************************************************************
 * Function	    : delete_key_text                          			           *
 * Input        : config_section_buf *section_buf - �ü�ֵ��Ҫ���صĶ�ָ��	   *
 *              : int key_num - ��Ҫɾ����ֵ���ڵ�����    	                   *
 *      	    : const char *text - �ü�ֵ�ַ�������			               *
 * Return	    : ����ɹ�����0��ʧ�ܷ���-1						               *
 * Comments	    : �����ֵ��                					               *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
static int delete_key_text(config_section_buf *section_buf, int key_num)
{
    
    if(section_buf == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if((key_num < 0) || (key_num >= CONFIG_MAX_LINE))
    {
        return INI_PARAM_ERROR;
    }

    int total_rows  = 0;
    int i           = 0; 
    
    total_rows = section_buf->key_num;
    memset(&(section_buf->key_buf[key_num]), 0, sizeof(config_key_buf));
    if(key_num != total_rows)
    {
        for(i=key_num+1; i<=total_rows; i++)
        {
            memcpy(&(section_buf->key_buf[i-1]), &(section_buf->key_buf[i]), sizeof(config_key_buf));
            memset(&(section_buf->key_buf[i]), 0, sizeof(config_key_buf));
        }
    }
    
    section_buf->key_num = total_rows - 1;	
	return INI_OK;
}


 
/*******************************************************************************
 * Function	    : config_section_search                     		           *
 * Input	    : config_file_buf *st_file_buf - �ļ��ṹָ��		           *
 *		        : const char *section - �����ҵĶ���			               *
 *		        : int n - ��ʼ���ҵ�����        			                   *
 * Return	    : ��ȡ�ɹ����ظö�������������ʧ�ܷ��ش�����	               *
 * Comments	    : ����ָ������section					                       *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
static int config_section_search(config_file_buf *st_file_buf, const char *section, int n)
{
	if(st_file_buf == NULL)
	{
      return INI_PARAM_ERROR;
	}

    if(section == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if((n < 0) || (n > CONFIG_MAX_SECTION))
    {
        return INI_PARAM_ERROR;
    }
	
	int i                           = 0;
    int	total_rows                  = 0;
    int ret                         = 0;
    config_section_buf *tmp_section = NULL;

    total_rows = st_file_buf->section_num;
    ret = total_rows + 1;
	for (i = n; i <= total_rows; i++)
	{
        tmp_section = &(st_file_buf->section_buf[i]);
		if (strcmp(tmp_section->section_str, section) == 0)
		{
			ret = i;
			break;
		}
	}
	
	return ret;
}

 
/*******************************************************************************
 * Function	    : set_key_text                             			           *
 * Input        : config_section_buf *section_buf - �ü�ֵ��Ҫ���صĶ�ָ��	   *
 *              : int key_num - �ü�ֵ��Ҫ���������    	                   *
 *              : int size - �ü�ֵ�ַ�������	                               *
 *      	    : const char *text - �ü�ֵ�ַ�������			               *
 * Return	    : ����ɹ�����0��ʧ�ܷ���-1						               *
 * Comments	    : �����ֵ��                					               *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
static int set_key_text(config_section_buf *section_buf, int key_num, char *text, int size)
{
    int ret = 0;
    
    if(section_buf == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if(text == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if((key_num < 0) || (key_num >= CONFIG_MAX_LINE))
    {
        return INI_PARAM_ERROR;
    }

    if(size >= CONFIG_KEY_MAX_CHAR)
    {
        return INI_PARAM_ERROR;
    }

    ret = config_key_search(section_buf, text);
    if(ret < 0)
    {
        return INI_GENERAL_ERROR;
    }
    else if(ret <= section_buf->key_num)
	{
		return INI_KEY_EXIST;
	}
    
	memcpy(section_buf->key_buf[key_num].key_str, text, size);
    section_buf->key_buf[key_num].key_str[size+1] = '\0';
    section_buf->key_num = key_num;
	
	return INI_OK;
}

 
/*******************************************************************************
 * Function	    : set_row_text                             			           *
 * Input        : config_key_buf *key_buf - �ַ�����Ҫ���صļ�ֵָ��    	   *
 *              : int key_num - �ַ�����Ҫ���صļ�ֵ������    	               *
 *              : int size - ���ַ�������	                                   *
 *      	    : const char *text - ���ַ�������			                   *
 * Return	    : ����ɹ�����0��ʧ�ܷ���-1						               *
 * Comments	    : �����ַ���ֵ              					               *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
static int set_row_text(config_key_buf *key_buf, int key_num, char *text, int size)
{
    if(key_buf == NULL)
    {
        return INI_PARAM_ERROR;
    }

    if(text == NULL)
    {
        return INI_PARAM_ERROR;
    }
    
    if((key_num < 0) || (key_num >= CONFIG_MAX_LINE))
    {
        return INI_PARAM_ERROR;
    }

    if(size >= CONFIG_ROW_MAX_CHAR)
    {
        return INI_PARAM_ERROR;
    }

    memset(key_buf->text, 0, strlen(key_buf->text));
	memcpy(key_buf->text, text, size);
    key_buf->text[strlen(key_buf->text)+1] = '\0';
	
	return INI_OK;
}

/*******************************************************************************
 * Function	    : load_file_into_config_buf                    		           *
 * Input	    : config_file_buf *st_file_buf - �ļ��ṹָ��		           *
 *		        : const char *file_name - �ļ���				               *
 *		        : section_key_string *section_struct - ҪУ��Ķ�����          *
 *		        : section_key_string *key_struct - ҪУ��ļ�ֵ����            *
 *		        : ����ҪУ�����������������NULL                               *
 * Output	    : char *err_str - ����ʱ�����������Ϣ      		           *
 * Return	    : ��ȡ�ɹ�����0��ʧ�ܷ��ش�����					               *
 * Comments	    : ���ļ����뵽������					                       *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
int utl_load_file_into_config_buf(config_file_buf *st_file_buf, const char *file_name, 
                              section_key_string *section_struct, section_key_string *key_struct)
{
	FILE *fp        = NULL;
    char *tmp       = NULL;
    char *ret_fgets = NULL;
    int size        = 0;
    int ret         = 0;
    int line_num    = 0;
    int section_num = 0;
    int key_num     = 0;
    int section_id  = 0;
    int have_data   = 0;
    char buffer[CONFIG_LINE_MAX_CHAR];
    char buffer1[CONFIG_LINE_MAX_CHAR];
	int reading_key_word = 0;
    
	if(st_file_buf == NULL)
    {
        return INI_PARAM_ERROR;
    }
    
	if(file_name == NULL)
    {
        return INI_PARAM_ERROR;
    }
	
	fp = fopen(file_name, "r");
	if (fp == NULL)
	{
		perror(file_name);
		return INI_PARAM_ERROR;
	}

    memset(buffer,0,sizeof(buffer));
    while ((ret_fgets = fgets (buffer, CONFIG_LINE_MAX_CHAR, fp)) != NULL)
	{
        line_num ++;
		if(strstr(buffer, "[end]"))
		{
			break;
		}
        ret = delete_comments(buffer);
        if(ret < 0)
        {
            fclose(fp);
            return ret;
        }
        size = strlen(buffer);
		buffer[size + 1] = '\0';
        //printf("line %d: %s\r\n", line_num, buffer);
        if(size > 0)
        {
			if(strcmp(buffer, SECTION_START) == 0)
			{
				reading_key_word = 1;
				continue;
			}
			if(strcmp(buffer, KEY_END) == 0)
			{
				reading_key_word = 0;
				continue;
			}
			if(reading_key_word)
			{
				continue;
			}
            have_data = 1;
            ret = is_section_str(buffer, section_struct);
            if(ret >= 0)
            {
                section_num++;
				if(section_num >= CONFIG_MAX_SECTION)
				{
					fclose(fp);
                    return INI_GENERAL_ERROR;
				}
                key_num = 0;
                section_id = ret;
                ret = set_section_text(st_file_buf, section_num, buffer, size);
                if(ret < 0)
                {
                    fclose(fp);
                    return ret;
                }
            }
            else if(ret == INI_NOT_SECTION_STR)
            {
                if(section_num == 0)
                {
                    fclose(fp);
                    return ret;
                }
                memset(buffer1, 0, sizeof(buffer1));
                memcpy(buffer1, buffer, strlen(buffer));
                tmp = strtok(buffer1, KEY_ASSIGNMENT_CHAR);
                if(tmp == NULL)
                {
                    fclose(fp);
                    return INI_KEY_STR_ERR;
                }
                
                ret = is_key_str(tmp, key_struct, section_id);
                if(ret < 0)
                {
                    fclose(fp);
                    return ret;
                }
                key_num++;
				if(key_num >= CONFIG_MAX_LINE)
				{
					fclose(fp);
                    return INI_GENERAL_ERROR;
				}
                ret = set_key_text(&(st_file_buf->section_buf[section_num]), key_num, tmp, strlen(tmp));
                if(ret < 0)
                {
                    fclose(fp);
                    return ret;
                }
                tmp = strtok(NULL, ";");
                if(tmp == NULL)
                {
                    continue;
                }
                ret = set_row_text(&(st_file_buf->section_buf[section_num].key_buf[key_num]), key_num, tmp, strlen(tmp));
                if(ret != 0)
                {
                    fclose(fp);
                    return ret;
                }
            }
            else
            {
                //fclose(fp);
                //return ret;
            }
        }
        
		memset(buffer,0,sizeof(buffer));
	}
    if((line_num == 0) || (have_data == 0))
    {
        fclose(fp);
        return INI_FILE_NULL;
    }
    fclose(fp);
	return INI_OK;
}


/*******************************************************************************
 * Function	    : get_config_item		                       	               *
 * Input	    : config_file_buf *st_file_buf - �ļ��ṹָ��                  *
 *		        : const char *section - �����ҵ�sectionָ��		               *
 *		        : const char *key - �����ҵ�keyָ��			                   *
 *		        : int n - ��ʼ���ҵĶ�����(һ��ʹ�ã���ֵ0������)              *
 * Output	    : char *value - ����Ҫ���ҵ�key�ַ���ֵ		                   *
 * Return	    : �ɹ����ظü�ֵ���ڶε�������ʧ�ܷ��ش�����                   *
 * Comments	    : ��ȡ���ļ���ָ��keyֵ				                           *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/ 
int utl_get_config_item(config_file_buf *st_file_buf, const char *section, 
	                       const char *key, char *value, int n)
{
	if(st_file_buf == NULL)
	{
        return INI_PARAM_ERROR;
	}
    
	if(section == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if(key == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if(value == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if((n < 0) || (n > CONFIG_MAX_SECTION))
    {
        return INI_PARAM_ERROR;
    }

	int key_row     = 0;
    int section_row = 0;
    char *tmp       = NULL;
    
	
	section_row = config_section_search(st_file_buf, section, n);
	if(section_row < 0)
	{
        return INI_GENERAL_ERROR;
	}
    else if(section_row > st_file_buf->section_num)
	{
		return INI_SECTION_NOT_EXIST;
	}

    key_row = config_key_search(&(st_file_buf->section_buf[section_row]), key);
    if(key_row < 0)
    {
        return INI_GENERAL_ERROR;
    }
    else if(key_row > st_file_buf->section_buf[section_row].key_num)
	{
		return INI_KEY_NOT_EXIST;
	}

    tmp = st_file_buf->section_buf[section_row].key_buf[key_row].text;
    memcpy(value, tmp, strlen(tmp));
    value[strlen(value)+1] = '\0';

	return section_row;
}


/*******************************************************************************
 * Function	    : set_config_item		                        	           *
 * Input	    : config_file_buf *st_file_buf - �ļ��ṹָ��                  *
 *		        : char *section - �����õ�sectionָ��   				       *
 *		        : char *key - �����õ�keyָ��   						       *
 *		        : char **value - �����õ�key����				               *
 *		        : section_key_string *section_struct - ҪУ��Ķ�����          *
 *		        : section_key_string *key_struct - ҪУ��ļ�ֵ����            *
 *		        : int n - ��ʼ���ҵĶ�����(һ��ʹ�ã���ֵ0������)              *
 * Return	    : �ɹ����ظü�ֵ���ڶε�������ʧ�ܷ��ش�����                   *
 * Comments	    : �������ļ���ָ��keyֵ						                   *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_set_config_item(config_file_buf *st_file_buf, char *section, char *key, char *value,
                    section_key_string *section_struct, section_key_string *key_struct, int n)
{
	if(st_file_buf == NULL)
	{
        return INI_PARAM_ERROR;
	}
    
	if(section == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if(key == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if(value == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if((n < 0) || (n > CONFIG_MAX_SECTION))
    {
        return INI_PARAM_ERROR;
    }

    int section_row = 0;
    int section_id  = 0;
    int key_row     = 0;
    int ret         = 0;

    section_id = is_section_str(section, section_struct);
    if(section_id < 0)
    {
        return section_id;
    }

    ret = is_key_str(key, key_struct, section_id);
    if(ret < 0)
    {
        return ret;
    }
    	
	section_row = config_section_search(st_file_buf, section, n);
	if (section_row < 0)
	{
		return section_row;
	}
    else if(section_row > st_file_buf->section_num)
    {
        section_row = st_file_buf->section_num + 1;
        ret = set_section_text(st_file_buf, section_row, section, strlen(section));
        if(ret != INI_OK)
        {
            return ret;
        }
    }

    key_row = config_key_search(&(st_file_buf->section_buf[section_row]), key);
    if (key_row < 0)
	{
		return key_row;
	}
    else if(key_row > st_file_buf->section_buf[section_row].key_num)
    {
        key_row = st_file_buf->section_buf[section_row].key_num + 1;
        ret = set_key_text(&(st_file_buf->section_buf[section_row]), key_row, key, strlen(key));
        if(ret != INI_OK)
        {
            return ret;
        }
    }

    ret = set_row_text(&(st_file_buf->section_buf[section_row].key_buf[key_row]), key_row, value, strlen(value));
    if(ret != INI_OK)
    {
        return ret;
    }
			
	return section_row;
}



/*******************************************************************************
 * Function	    : delete_config_item	                        	           *
 * Input	    : config_file_buf *st_file_buf - �ļ��ṹָ��                  *
 *		        : char *section - ��ɾ����sectionָ��   				       *
 *		        : char *key - ��ɾ����keyָ��   						       *
 *		        : section_key_string *section_struct - ҪУ��Ķ�����          *
 *		        : section_key_string *key_struct - ҪУ��ļ�ֵ����            *
 *		        : int n - ��ʼ���ҵĶ�����(һ��ʹ�ã���ֵ0������)              *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����                                    *
 * Comments	    : �������ļ���section����keyֵ����key=NULL,��ɾ������section�� *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_delete_config_item(config_file_buf *st_file_buf, char *section, char *key, 
                    section_key_string *section_struct, section_key_string *key_struct, int n)
{
	if(st_file_buf == NULL)
	{
        return INI_PARAM_ERROR;
	}
    
	if(section == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if((n < 0) || (n > CONFIG_MAX_SECTION))
    {
        return INI_PARAM_ERROR;
    }

    int section_row = 0;
    int section_id  = 0;
    int key_row     = 0;
    int ret         = 0;

    section_id = is_section_str(section, section_struct);
    if(section_id < 0)
    {
        return section_id;
    }

    section_row = config_section_search(st_file_buf, section, n);
	if (section_row < 0)
	{
		return section_row;
	}
    else if(section_row > st_file_buf->section_num)
    {
        return INI_OK;
    }
    
    if(key == NULL)
    {
        ret = delete_section_text(st_file_buf, section_row);
        return ret;
    }
    
    ret = is_key_str(key, key_struct, section_id);
    if(ret < 0)
    {
        return ret;
    }
    
    key_row = config_key_search(&(st_file_buf->section_buf[section_row]), key);
    if (key_row < 0)
	{
		return key_row;
	}
    else if(key_row > st_file_buf->section_buf[section_row].key_num)
    {
        return INI_OK;
    }
    else
    {
        ret = delete_key_text(&(st_file_buf->section_buf[section_row]), key_row);
        return ret;
    }
}


/*******************************************************************************
 * Function	    : save_config_buf_to_file	                        	       *
 * Input	    : const config_file_buf *st_file_buf - �ļ��ṹ��ָ��          *
 *		        : const char *file_name - Ҫ������ļ���		               *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����					                   *
 * Comments	    : �����ڴ������ݵ�ָ���ļ���						           *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_save_config_buf_to_file(const config_file_buf *st_file_buf, const char *file_name)
{
	if(st_file_buf == NULL)
	{
        return INI_PARAM_ERROR;
	}

    if(file_name == NULL)
	{
        return INI_PARAM_ERROR;
	}

	int section_rows    = 0;
    int key_rows        = 0;
	int i               = 0;
    int j               = 0;
    FILE *fp            = NULL;
    config_section_buf  *tmp_section = NULL;
    config_key_buf      *tmp_key     = NULL;
    
	fp = fopen(file_name, "w");
	if (fp == NULL)
	{
		return INI_GENERAL_ERROR;
	}

    section_rows = st_file_buf->section_num;
	for (i = 1; i <= section_rows; i++)
	{
        tmp_section = (config_section_buf  *)&(st_file_buf->section_buf[i]);
		fputs(tmp_section->section_str, fp);
		fputc('\n', fp);
        key_rows = tmp_section->key_num;
        for (j = 1; j <= key_rows; j++)
        {
            tmp_key = &(tmp_section->key_buf[j]);
            fputs(tmp_key->key_str, fp);
		    fputc('=', fp);
            fputs(tmp_key->text, fp);
            fputc('\n', fp);
        }
        fputc('\n', fp);
	}
	fputs("[end]\n", fp);
    fclose(fp);
	return INI_OK;
}

/*******************************************************************************
 * Function	    : utl_get_config_section_num		                       	               *
 * Input	         :  st_file_buf           �ļ��ṹָ��                       *
                        : section               �����ҵ�sectionָ��
 * Output	    : section_num - ���ز��ҵĶ����ļ��ṹ���еĸ���           *
 * Return	    : �ɹ����ضεĸ���
                          :  ʧ�ܷ��ش�����                                                                                         *
 *******************************************************************************/ 
int utl_get_config_section_num(config_file_buf *st_file_buf, const char *section)
{
    if(NULL==st_file_buf || NULL==section)
	{
        return INI_PARAM_ERROR;
	}
    
	int i                           = 0;
    int	total_rows                  = 0;
    int sub                         = 0;
    config_section_buf *tmp_section = NULL;

    total_rows = st_file_buf->section_num;
	for (i = 0; i <= total_rows; i++)
	{
        tmp_section = &(st_file_buf->section_buf[i]);
		if (strcmp(tmp_section->section_str, section) == 0)
			sub++;
	}

    return sub;
    
}

void config_print(config_file_buf *config_buf)
{
	int section_rows    = 0;
    int key_rows        = 0;
	int i               = 0;
    int j               = 0;
    config_section_buf  *tmp_section = NULL;
    config_key_buf      *tmp_key     = NULL;

	section_rows = config_buf->section_num;
	for (i = 1; i <= section_rows; i++)
	{
        tmp_section = (config_section_buf  *)&(config_buf->section_buf[i]);
		printf("%s\n", tmp_section->section_str);
		
        key_rows = tmp_section->key_num;
        for (j = 1; j <= key_rows; j++)
        {
            tmp_key = &(tmp_section->key_buf[j]);
			printf("%s=%s\n", tmp_key->key_str, tmp_key->text);
        }
	}
}

#if 0
int main(int argc, char **argv)
{
	//int i = 1;
	//memset(section_string, 0, sizeof(section_string));
	//memset(key_string, 0, sizeof(key_string));

	wallet_config *config;

	config = malloc(sizeof(wallet_config));
	if(config == NULL)
	{
		return -1;
	}
	memset(config, 0, sizeof(wallet_config));

	config->server = malloc(sizeof(wallet_server));
	if(config->server == NULL)
	{
		free(config);
		return -1;
	}
	memset(config->server, 0, sizeof(wallet_server));

	config->endpoint = malloc(sizeof(wallet_endpoint));
	if(config->endpoint == NULL)
	{
		free(config);
		free(config->server);
		return -1;
	}
	memset(config->endpoint, 0, sizeof(wallet_endpoint));
	
	read_config("config.ini", config);

	#if 0
	while(section_string[i].section_key)
	{
		printf("section_string[%d]:%s\r\n", i, section_string[i].section_key_str);
		i++;
	}
	i = 1;
	while(key_string[i].section_key)
	{
		printf("key_string[%d]:%s, section key:%d\r\n", i, key_string[i].section_key_str, key_string[i].section_key);
		i++;
	}
	#endif
	
	return 0;
}
#endif

