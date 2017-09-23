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
#include "wind_type.h"
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
    w_uint32_t magic;//�ڴ�ر��ɹ������ı�־
    void *head;//�ڴ�ص�ͷ��λ��
    const char *name;
    w_uint32_t len;//�ڴ�ص�ʵ�ʿ��ÿռ��С
    w_uint32_t itemsize;//ÿ����Ĵ�С
    w_uint32_t num;//�ֳɵ��ڴ�������
    w_uint32_t used;//�Ѿ�ʹ�õ��ڴ�������
    ppool_s free;//���п��ָ��
    ppool_s last;//���һ�����п��ָ��
}pool_head_s,*ppool_head_s;

#define WIND_MPOOL_ALIGN(x) ((((x)+3)/4)*4)
//�����ڴ�صķ���
#define WIND_MPOOL(pool,num,size) w_uint8_t pool[sizeof(pool_head_s) + num * (WIND_MPOOL_ALIGN(size) + sizeof(w_uint32_t))]

w_err_t wind_pool_print(w_int8_t *name,void *pool);

w_err_t wind_pool_create(const char *name,void *mem,w_uint32_t msize,w_uint32_t itemsize);

void *wind_pool_alloc(void *mem);

w_err_t wind_pool_free(void *mem,void *block);


#ifdef __cplusplus
}
#endif


#endif

