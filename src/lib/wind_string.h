/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_string.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: ��д���ַ���������ͷ�ļ�
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2012.09.26
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2012.10.20
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/

#ifndef WIND_STRING_H_
#define WIND_STRING_H_


#include "wind_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char*  wind_strcpy(char *dest,const char *src);

extern char* wind_strncpy(char *dest,const char *src,u32_t count);

extern char * wind_strcat(char * dest, const char * src);

extern char* wind_strncat(char *dest, const char *src, u32_t count);

extern s32_t wind_strcmp(const char *dest,const char *src);

extern s32_t wind_strncmp(const char* cs,const char * ct,u32_t count);

extern char* wind_strchr(const char * s, int c);

extern char * wind_strrchr(const char * s, int c);

extern s16_t wind_strlen(const char *str);

extern u32_t wind_strnlen(const char * s, u32_t count);

extern u32_t wind_strspn(const char *s, const char *accept);

extern char * wind_strpbrk(const char * cs,const char * ct);

extern void * wind_memset(void * s,char c,u32_t count);

extern char * wind_bcopy(const char * src, char * dest, int count);

extern void * wind_memcpy(void * dest,const void *src,u32_t count);

extern void * wind_memmove(void * dest,const void *src,u32_t count);

extern int wind_memcmp(const void * cs,const void * ct,u32_t count);

extern void * wind_memscan(void * addr, int c, u32_t size);

extern char * wind_strstr(const char * s1,const char * s2);



#ifdef __cplusplus
}
#endif

#endif

