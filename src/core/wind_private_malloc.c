/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**��   ��   ��: wind_private_malloc.h / wind_private_malloc.c
**��   ��   ��: �ܽ���
**����޸�����: 2014/10/24 0:00:00
**��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2014/10/24 0:00:00
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: �̵߳�˽��ջ�Ķ������صĲ���
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/


/*********************************************ͷ�ļ�����***********************************************/
#include "wind_config.h"
#include "wind_types.h"
#include "wind_err.h"
#include "wind_heap.h"
#include "wind_thread.h"
#include "wind_private_malloc.h"
#include "wind_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_PRIVATE_HEAP_SUPPORT > 0 && WIND_PRIVATE_HEAP_SUPPORT > 0

/********************************************�ڲ���������**********************************************/



/********************************************�ڲ���������*********************************************/



/********************************************ȫ�ֱ�������**********************************************/



/********************************************ȫ�ֺ�������**********************************************/
err_t wind_privateheap_create(u32_t size)
{
    err_t err;
    void *start;
    pthread_s pthread;
    pmemheap_s mhp;
    pthread = wind_get_cur_proc();
    WIND_ASSERT_RETURN((pthread != NULL),ERR_NULL_POINTER);
    mhp = (pmemheap_s)wind_heap_alloc_default(size);
    WIND_ASSERT_RETURN(mhp != NULL,ERR_NULL_POINTER);
    //mhp = (pmemheap_s)
    pthread->private_heap = mhp;
    start = (void*)((u32_t)mhp + sizeof(memheap_s));
    err = wind_heap_init(mhp,"pri heap",start,size);
    if(err != ERR_OK)
    {
        wind_heap_free(mhp);
        pthread->private_heap = NULL;
        return ERR_COMMAN;
    }
    return ERR_OK;
}

void *wind_privateheap_alloc(u32_t size)
{
    void *p;
    err_t err;
    pthread_s pthread;
    pthread = wind_get_cur_proc();
    WIND_ASSERT_RETURN(pthread != NULL,NULL);
    WIND_ASSERT_RETURN(pthread->private_heap != NULL,NULL);
    if(pthread->private_heap == NULL)
    {
        err = wind_privateheap_create(PRIVATE_HEAP_SIZE);
        WIND_ASSERT_RETURN(err == ERR_OK,NULL);
    }
    p = wind_heap_alloc(pthread->private_heap,size);
    return p;
}

err_t wind_privateheap_free(void *ptr)
{
    err_t err;
    WIND_ASSERT_RETURN(ptr != NULL,ERR_NULL_POINTER);
    err = wind_heap_free((void *) ptr);
    return err;    
}


#endif
#ifdef __cplusplus
}
#endif

