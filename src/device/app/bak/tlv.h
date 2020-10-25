#ifndef TLV_H
#define TLV_H
#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"

#define MESSAGE_MAX_LENGTH		1000


#define __packed __attribute__ ((packed))


/*报文头部结构体*/
typedef struct package_header_t
{
	UINT16 len;				/*整个数据包的大小，包含头部和数据部分*/
	UINT16 type;			/*报文类型*/
}__packed package_header;

typedef struct tlv_package_struct{
	package_header header;
	UINT8 data[MESSAGE_MAX_LENGTH-sizeof(package_header)];
}__packed TLV_package;


UINT32 add_tlv(TLV_package *package, DATA_TYPE_E type, UINT32 value_len, UINT8* value);
UINT8 *get_tlv(TLV_package *package, DATA_TYPE_E type, UINT32 *len);
void free_tlv(TLV_package *package);

#ifdef	__cplusplus
}
#endif

#endif


