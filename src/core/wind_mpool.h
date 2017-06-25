/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_mpool.h / wind_mpool.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os���ڴ�ع�����صĹ��ܺ���
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2013.11.27
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2013.11.27
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/

#ifndef WIND_MPOOL_H__
#define WIND_MPOOL_H__

#include "wind_config.h"
#include "wind_types.h"
#include "wind_debug.h"

#ifdef __cplusplus
extern "C" {
#endif
#define WIND_MPOOL_DEBUG(...) //WIND_STD_OUT(__VA_ARGS__)
#define WIND_MPOOL_MAGIC 0x5d9c843e
#define WIND_MPOOL_BLOCK_MAGIC 0x52d6e3a9
//һ���������ڴ�����ṹ��
typedef struct __pool_s
{
    struct __pool_s *next;
}pool_s,*ppool_s;

//�ڴ�ص�ͷ����Ϣ�ṹ��
typedef struct __mpoolHead_s
{
    u32_t magic;//�ڴ�ر��ɹ������ı�־
    void *head;//�ڴ�ص�ͷ��λ��
    const char *name;
    u32_t len;//�ڴ�ص�ʵ�ʿ��ÿռ��С
    u32_t itemsize;//ÿ����Ĵ�С
    u32_t num;//�ֳɵ��ڴ�������
    u32_t used;//�Ѿ�ʹ�õ��ڴ�������
    ppool_s free;//���п��ָ��
    ppool_s last;//���һ�����п��ָ��
}mpoolHead_s,*pmpoolHead_s;

#define WIND_MPOOL_ALIGN(x) ((((x)+3)/4)*4)
//�����ڴ�صķ���
#define WIND_MPOOL(pool,num,size) u8_t pool[sizeof(mpoolHead_s) + num * (WIND_MPOOL_ALIGN(size) + sizeof(u32_t))]

err_t wind_mpool_show(s8_t *name,void *pool);

err_t wind_mpool_create(const char *name,void *mem,u32_t msize,u32_t itemsize);

void *wind_mpool_alloc(void *mem);

err_t wind_mpool_free(void *mem,void *block);


#ifdef __cplusplus
}
#endif


#endif

