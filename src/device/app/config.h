#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
 * 宏定义                   								                   *
 *******************************************************************************/
#define 	SECTION_LEFT_LABEL_CHAR        	'['	//文件段类型左标签
#define 	SECTION_RIGHT_LABEL_CHAR       	']'	//文件段类型右标签
#define 	KEY_ASSIGNMENT_CHAR            	"="	//赋值符号
#define 	NOTES_CHAR                     	';'	//文件注释符号
#define	    CONFIG_LINE_MAX_CHAR			512	//一行最大字符数
#define	    CONFIG_SECTION_MAX_CHAR			16	//段名最大字符数
#define	    CONFIG_KEY_MAX_CHAR			    32	//关键字名最大字符数
#define	    CONFIG_MAX_LINE			        16	//各段最大键值数
#define	    CONFIG_MAX_SECTION			    2	//最大段数
#define	    CONFIG_ROW_MAX_CHAR			    (CONFIG_LINE_MAX_CHAR - CONFIG_KEY_MAX_CHAR - 1)	//键值最大字符数:256-32-1


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
 * 数据结构	                								                   *
 *******************************************************************************/

typedef struct _config_key_buf
{
    char key_str[CONFIG_KEY_MAX_CHAR];                  //关键字key本身字符串
    char text[CONFIG_ROW_MAX_CHAR];                     //关键字key内容字符串
}config_key_buf;

typedef struct _config_section_buf
{
    int      key_num;                                   //当前section所包含的key个数
    char section_str[CONFIG_SECTION_MAX_CHAR];          //当前section本身字符串
    config_key_buf key_buf[CONFIG_MAX_LINE];            //当前section下所挂载的key, key_buf下标有效值从1开始使用
}config_section_buf;

typedef struct _config_file_buf
{
    int section_num;                                    //当前文件所包含的section个数
    config_section_buf section_buf[CONFIG_MAX_SECTION]; //当前文件所挂载的section, section_buf下标有效值从1开始使用
}config_file_buf;

typedef struct _section_key_string
{
    int section_key;                                     //段编号:若做键值用，则为该键值所在段编号                                           
    char section_key_str[CONFIG_KEY_MAX_CHAR];           //段字符串或键值字符串           
}section_key_string;

/*******************************************************************************
 * Function	    : utl_load_file_into_config_buf                		           *
 * Input	    : config_file_buf *st_file_buf - 文件结构指针		           *
 *		        : const char *file_name - 文件名				               *
 *		        : section_key_string *section_struct - 要校验的段名集          *
 *		        : section_key_string *key_struct - 要校验的键值名集            *
 *		        : 不需要校验后面两个参数输入NULL                               *
 * Output	    : char *err_str - 出错时，输出错误信息      		           *
 * Return	    : 读取成功返回0，失败返回错误码					               *
 * Comments	    : 将文件读入到缓存中					                       *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/
int utl_load_file_into_config_buf(config_file_buf *st_file_buf, const char *file_name, 
                              section_key_string *section_struct, section_key_string *key_struct);

/*******************************************************************************
 * Function	    : get_config_item		                       	               *
 * Input	    : const config_file_buf *st_file_buf - 文件结构体指针          *
 *		        : const char *section - 待查找的section指针		               *
 *		        : const char *key - 待查找的key指针			                   *
 *		        : int n - 开始查找的行数        			                   *
 * Output	    : char *value - 带回要查找的key字符串值		                   *
 * Return	    : 成功返回该键值所在段的行数，失败返回错误码                   *
 * Comments	    : 在取得文件中指定key值				                           *
 * First Make	: 2011-11-14 xqchen							                   *
 *******************************************************************************/ 
int utl_get_config_item(config_file_buf *st_file_buf, const char *section, 
	                       const char *key, char *value, int n);

/*******************************************************************************
 * Function	    : set_config_item		                        	           *
 * Input	    : const config_file_buf *st_file_buf - 文件结构体指针          *
 *		        : char *section - 待设置的section指针   				       *
 *		        : char *key - 待设置的key指针   						       *
 *		        : char **value - 待设置的key内容				               *
 *		        : section_key_string *section_struct - 要校验的段名集          *
 *		        : section_key_string *key_struct - 要校验的键值名集            *
 *		        : int n - 开始查找的行数        			                   *
 * Return	    : 成功返回0，失败返回错误码						               *
 * Comments	    : 在设置文件中指定key值						                   *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_set_config_item(config_file_buf *st_file_buf, char *section, char *key, char *value,
                    section_key_string *section_struct, section_key_string *key_struct, int n);

/*******************************************************************************
 * Function	    : save_config_buf_to_file	                        	       *
 * Input	    : const config_file_buf *st_file_buf - 文件结构体指针          *
 *		        : const char *file_name - 要保存的文件名		               *
 * Return	    : 成功返回0，失败返回错误码					                   *
 * Comments	    : 保存内存中内容到指定文件中						           *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_save_config_buf_to_file(const config_file_buf *st_file_buf, const char *file_name);

/*******************************************************************************
 * Function	    : delete_config_item	                        	           *
 * Input	    : config_file_buf *st_file_buf - 文件结构指针                  *
 *		        : char *section - 待删除的section指针   				       *
 *		        : char *key - 待删除的key指针   						       *
 *		        : section_key_string *section_struct - 要校验的段名集          *
 *		        : section_key_string *key_struct - 要校验的键值名集            *
 *		        : int n - 开始查找的行数        			                   *
 * Return	    : 成功返回0，失败返回错误码                                    *
 * Comments	    : 在设置文件中section段下key值，若key=NULL,则删除整个section段 *
 * First Make	: 2011-11-14 xqchen								               *
 *******************************************************************************/ 
int utl_delete_config_item(config_file_buf *st_file_buf, char *section, char *key, 
                    section_key_string *section_struct, section_key_string *key_struct, int n);


#ifdef __cplusplus
};
#endif

#endif


