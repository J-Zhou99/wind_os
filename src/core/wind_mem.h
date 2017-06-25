/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_mem.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: �ڴ������صĴ���ͷ�ļ�
**��        ע: Ƕ��ʽϵͳ���ڴ�ռ�������������ޣ����Ҷ����ʱЧ����Ҫ����˽���������ر��Ҫ��
**              ��������Ҫ�ö�̬����ķ�ʽ��ͬʱ��ʹ����ɺ�Ҫ�����ͷţ����������϶����Ƭ����
**              Ӱ��ϵͳ������
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
#ifndef WIND_MEM_H__
#define WIND_MEM_H__



#include "wind_config.h"
#include "wind_types.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
#if WIND_MEM_SUPPORT > 0


typedef struct __memhaed_s
{
    struct __memhaed_s *pre;//ǰһ���������headָ��λ��
    struct __memhaed_s *next;//��һ���������headָ���λ��
    u32_t lenth;//��ǰ�������ĳ���
} memhead_s,*pmemhead_s;

typedef struct __meminfo_s
{
    u32_t base;//���ɿռ�Ļ���ַ
    u32_t lenth;//���ɿռ���ܳ���    
    u32_t offset;//��ǰ�Ŀռ�ƫ����
    //u32_t used;//�Ѿ�����Ŀռ���ܺ�
    pmemhead_s plast;//����һ�η����ָ���λ��
    pmemhead_s pfree;
}meminfo_s,*pmeminfo_s;

#define MEM_ALIGN_4 1 //�����ڴ�ռ���4�ֽڶ���

err_t wind_mem_init(u8_t num,u32_t base,u32_t lenth);

//err_t wind_memset(void *p,u32_t value,u32_t len);

//err_t wind_memcpy(void *dest,const void *src,u32_t len);


//�������������ǹ����ڴ����ĺ������������ڴ����Ĵ��뻹û�н���
//��ˣ��ڴ����ֻ����Ϊ������һ�ַ���

void *wind_malloc(u32_t size);

void **wind_calloc(u32_t block,u32_t size);

void ***wind_talloc(u32_t num,u32_t block,u32_t size);

void wind_free(void *p);
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //WIND_MEM_SUPPORT
#endif //WIND_MEM_H__

