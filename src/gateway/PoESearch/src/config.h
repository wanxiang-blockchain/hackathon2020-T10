#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
 * �궨��                   								                   *
 *******************************************************************************/
#define 	SECTION_LEFT_LABEL_CHAR        	'['	//�ļ����������ǩ
#define 	SECTION_RIGHT_LABEL_CHAR       	']'	//�ļ��������ұ�ǩ
#define 	KEY_ASSIGNMENT_CHAR            	"="	//��ֵ����
#define 	NOTES_CHAR                     	';'	//�ļ�ע�ͷ���
#define	    CONFIG_LINE_MAX_CHAR			128	//һ������ַ���
#define	    CONFIG_ROW_MAX_CHAR			    95	//��ֵ����ַ���:128-32-1
#define	    CONFIG_SECTION_MAX_CHAR			16	//��������ַ���
#define	    CONFIG_KEY_MAX_CHAR			    32	//�ؼ���������ַ���
#define	    CONFIG_MAX_LINE			        32	//��������ֵ��
#define	    CONFIG_MAX_SECTION			    32	//������

#define SECTION_START						"[section_start]"
#define SECTION_END							"[section_end]"
#define KEY_START							"[key_start]"
#define KEY_END								"[key_end]"


/*error code*/
#define INI_OK                              0
#define INI_GENERAL_ERROR                   -1
#define INI_PARAM_ERROR                     -2
#define INI_NOT_SECTION_STR                 -3
#define INI_SECTION_STR_ERR                 -4
#define INI_KEY_EXIST                       -5
#define INI_KEY_STR_ERR                     -6
#define INI_FILE_NULL                       -7
#define INI_SECTION_NOT_EXIST               -8
#define INI_KEY_NOT_EXIST                   -9


/*******************************************************************************
 * ���ݽṹ	                								                   *
 *******************************************************************************/

typedef struct _config_key_buf
{
    char key_str[CONFIG_KEY_MAX_CHAR];                  //�ؼ���key�����ַ���
    char text[CONFIG_ROW_MAX_CHAR];                     //�ؼ���key�����ַ���
}config_key_buf;

typedef struct _config_section_buf
{
    int      key_num;                                   //��ǰsection��������key����
    char section_str[CONFIG_SECTION_MAX_CHAR];          //��ǰsection�����ַ���
    config_key_buf key_buf[CONFIG_MAX_LINE];            //��ǰsection�������ص�key, key_buf�±���Чֵ��1��ʼʹ��
}config_section_buf;

typedef struct _config_file_buf
{
    int section_num;                                    //��ǰ�ļ���������section����
    config_section_buf section_buf[CONFIG_MAX_SECTION]; //��ǰ�ļ������ص�section, section_buf�±���Чֵ��1��ʼʹ��
}config_file_buf;

typedef struct _section_key_string
{
    int section_key;                                     //�α��:������ֵ�ã���Ϊ�ü�ֵ���ڶα��                                           
    char section_key_str[CONFIG_KEY_MAX_CHAR];           //���ַ������ֵ�ַ���           
}section_key_string;

/*******************************************************************************
 * Function	    : utl_load_file_into_config_buf                		           *
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
                              section_key_string *section_struct, section_key_string *key_struct);

/*******************************************************************************
 * Function	    : get_config_item		                       	               *
 * Input	    : const config_file_buf *st_file_buf - �ļ��ṹ��ָ��          *
 *		        : const char *section - �����ҵ�sectionָ��		               *
 *		        : const char *key - �����ҵ�keyָ��			                   *
 *		        : int n - ��ʼ���ҵ�����        			                   *
 * Output	    : char *value - ����Ҫ���ҵ�key�ַ���ֵ		                   *
 * Return	    : �ɹ����ظü�ֵ���ڶε�������ʧ�ܷ��ش�����                   *
 * Comments	    : ��ȡ���ļ���ָ��keyֵ				                           *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/ 
int utl_get_config_item(config_file_buf *st_file_buf, const char *section, 
	                       const char *key, char *value, int n);

/*******************************************************************************
 * Function	    : set_config_item		                        	           *
 * Input	    : const config_file_buf *st_file_buf - �ļ��ṹ��ָ��          *
 *		        : char *section - �����õ�sectionָ��   				       *
 *		        : char *key - �����õ�keyָ��   						       *
 *		        : char **value - �����õ�key����				               *
 *		        : section_key_string *section_struct - ҪУ��Ķ�����          *
 *		        : section_key_string *key_struct - ҪУ��ļ�ֵ����            *
 *		        : int n - ��ʼ���ҵ�����        			                   *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����						               *
 * Comments	    : �������ļ���ָ��keyֵ						                   *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_set_config_item(config_file_buf *st_file_buf, char *section, char *key, char *value,
                    section_key_string *section_struct, section_key_string *key_struct, int n);

/*******************************************************************************
 * Function	    : save_config_buf_to_file	                        	       *
 * Input	    : const config_file_buf *st_file_buf - �ļ��ṹ��ָ��          *
 *		        : const char *file_name - Ҫ������ļ���		               *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����					                   *
 * Comments	    : �����ڴ������ݵ�ָ���ļ���						           *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_save_config_buf_to_file(const config_file_buf *st_file_buf, const char *file_name);

/*******************************************************************************
 * Function	    : delete_config_item	                        	           *
 * Input	    : config_file_buf *st_file_buf - �ļ��ṹָ��                  *
 *		        : char *section - ��ɾ����sectionָ��   				       *
 *		        : char *key - ��ɾ����keyָ��   						       *
 *		        : section_key_string *section_struct - ҪУ��Ķ�����          *
 *		        : section_key_string *key_struct - ҪУ��ļ�ֵ����            *
 *		        : int n - ��ʼ���ҵ�����        			                   *
 * Return	    : �ɹ�����0��ʧ�ܷ��ش�����                                    *
 * Comments	    : �������ļ���section����keyֵ����key=NULL,��ɾ������section�� *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_delete_config_item(config_file_buf *st_file_buf, char *section, char *key, 
                    section_key_string *section_struct, section_key_string *key_struct, int n);


#define COMM_CONFIG_STR                 	"[comm_config]"
#define KEY_LOGLEVEL                		"log_level"
#define KEY_DIR                				"Directory"
#define KEY_DOC            					"Documents"
#define KEY_IPADDR                			"ipaddr"
#define KEY_PORT                			"port"

#define KEY_UPON_CHAIN						"uponchain"
#define KEY_CHAIN_TOKEN						"chain_token"
#define KEY_CHAIN_SETURL					"chain_seturl"
#define KEY_CHAIN_GETURL					"chain_geturl"


#define KEY_HB_SERVER_STATION				"hb_server"				
#define KEY_HB_TOKEN_REQ_URL				"hb_token_url"		
#define KEY_HB_DEVICES_LIST_REQ_URL			"hb_dev_list_url"
#define KEY_HB_DEVICES_GPS_REQ_URL			"hb_dev_gps_url"		
#define KEY_HB_DEVICES_EVENTS_REQ_URL		"hb_events_url"	
#define KEY_HB_APPID						"hb_appid"
#define KEY_HB_APPSECRET					"hb_appsecret"


					
#define IS_SECTION_START                1
#define IS_COMM_CONFIG_STR          	2
#define IS_SECTION_END                  6

typedef struct _config_param_t
{
    char dir[128];
    char doc[128];
    char ipaddr[32];
    char port[16];
	int upon_chain;
    char chain_token[32];
	char chain_seturl[128];
	char chain_geturl[128];
	char hb_server[128];
	char hb_token_url[64];
	char hb_dev_list_url[64];
	char hb_dev_gps_url[64];
	char hb_events_url[64];
	char hb_appid[64];
	char hb_appsecret[64];
} config_param_t;


extern int read_config(char *config_file, config_param_t *config);
extern config_param_t config_param;

#ifdef __cplusplus
};
#endif

#endif

