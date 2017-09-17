/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_queue.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os����Ϣ��صĴ���
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2012.09.26
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2014.11.2
** �衡��: �޸��˶��еĻ��ⷽ��������ϵͳ�ڲ�������ʱ�ֲ��������߲�����
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/

#include "wind_type.h"
#include "wind_queue.h"
#include "wind_os_hwif.h"
#include "wind_assert.h"
#include "wind_err.h"

static w_uint32_t wind_queue_lock(queue_s *q)
{
    if(q->lock_type == LOCK_TYPE_NONE)
        return 0;
    else if(q->lock_type == LOCK_TYPE_GLOBAL) 
        wind_close_interrupt();
#if WIND_LOCK_SUPPORT > 0
    else if(q->lock_type == LOCK_TYPE_AREA)
        wind_lock_close(q->lock);
#endif
    return 0;
}

static void wind_queue_unlock(queue_s *q)
{
    if(q->lock_type == LOCK_TYPE_NONE)
        return;
    else if(q->lock_type == LOCK_TYPE_GLOBAL) 
        wind_open_interrupt();
#if WIND_LOCK_SUPPORT > 0
    else if(q->lock_type == LOCK_TYPE_AREA)
        wind_lock_open(q->lock);
#endif
}


w_err_t wind_queue_create(void *mem,
                          w_uint32_t size,
                          w_uint16_t data_wid,
                          lock_type_e lock_type
                          )
{
    queue_s *q;
    WIND_ASSERT_RETURN(mem != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(size > sizeof(queue_s),ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(data_wid > 0,ERR_INVALID_PARAM);

    q = (queue_s *)mem;
    
    //����Ҫʹ�þֲ���ʱ�������������֧�֣���ʹ��ȫ����
    if(lock_type == LOCK_TYPE_GLOBAL)
    {
#if WIND_LOCK_SUPPORT > 0
        q->lock_type = (w_int32_t)lock_type;
        q->lock = wind_lock_create("queue");
        WIND_ASSERT_RETURN(q->lock != NULL,ERR_NULL_POINTER);
#else
        q->lock_type = LOCK_TYPE_GLOBAL;
#endif
    }

    q->rd = q->buf;
    q->wr = q->buf;
    q->data_wid = data_wid;
    q->count = 0;
    
    // ������п��Դ洢��������Ŀ 
    q->capacity = (size - (w_uint32_t)(((queue_s *)0)->buf)) / q->data_wid;
    
    // �������ݻ���Ľ�����ַ
    q->end = q->buf + q->capacity * q->data_wid;               
    q->magic = WIND_QUEUE_MAGIC;
    return ERR_OK;
}



w_int32_t wind_queue_read(void *queue,void *buf,w_uint32_t len)
{
    w_uint32_t i;
    queue_s *q;
    w_uint8_t *buff;
    w_uint32_t lenth;
    
    WIND_ASSERT_RETURN(buf != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(queue != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(len % q->data_wid == 0,ERR_INVALID_PARAM);
    
    q = (queue_s *)queue;
    WIND_ASSERT_RETURN(q->magic == WIND_QUEUE_MAGIC,ERR_INVALID_PARAM);
    buff = buf;
    
    wind_queue_lock(q);
    lenth = q->count * q->data_wid;
    lenth = lenth > len?len:lenth;
    for(i = 0;i < lenth;i ++)
    {
        buff[i] = q->rd[0]; /* ���ݳ���     */
        q->rd++;  /* ��������ָ�� */
        if(q->rd >= q->end)
            q->rd = 0;
    }
    q->count -= lenth / q->data_wid;                                     /* ���ݼ���      */
    wind_queue_unlock(q);
    return lenth;
}




w_int32_t wind_queue_write(void *queue,void *buf,w_uint32_t len)
{
    w_uint32_t i;
    queue_s *q;
    w_uint8_t *buff;
    w_uint32_t lenth;
    
    WIND_ASSERT_RETURN(buf != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(queue != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(len % q->data_wid == 0,ERR_INVALID_PARAM);

    q = (queue_s *)queue;
    WIND_ASSERT_RETURN(q->magic == WIND_QUEUE_MAGIC,ERR_INVALID_PARAM);
    buff = buf;

    wind_queue_lock(q);
    lenth = (q->capacity - q->count) * q->data_wid;
    lenth = lenth > len?len:lenth;
                                                             
    for(i = 0;i < q->data_wid;i ++)
    {
        q->wr[0] = buff[i];                                        
        q->wr++; /* �������ָ��*/
        if(q->wr >= q->end)
            q->wr = 0;                                         
    }
    q->count += lenth / q->data_wid;   
    wind_queue_unlock(q);

    return lenth;
}


w_int32_t wind_queue_datalen(void *queue)
{
    w_int32_t temp;
    queue_s *q;
    WIND_ASSERT_RETURN(queue != NULL,ERR_NULL_POINTER);
    q = (queue_s *)queue;
    WIND_ASSERT_RETURN(q->magic == WIND_QUEUE_MAGIC,ERR_INVALID_PARAM);
    temp = 0;                                                   
    wind_queue_lock(q);
    temp = q->count;
    wind_queue_unlock(q);
    return temp;
}




w_int32_t wind_queue_capacity(void *queue)
{
    w_int32_t temp;
    queue_s *q;
    WIND_ASSERT_RETURN(queue != NULL,ERR_NULL_POINTER);
    q = (queue_s *)queue;
    WIND_ASSERT_RETURN(q->magic == WIND_QUEUE_MAGIC,ERR_INVALID_PARAM);
    temp = 0;                                                   /* ������Ч����0 */
    wind_queue_lock(q);
    temp = ((queue_s *)queue)->count;
    wind_queue_unlock(q);
    return temp;
}



w_err_t wind_queue_flush(void *queue)
{
    queue_s *q;
    WIND_ASSERT_RETURN(queue != NULL,ERR_NULL_POINTER);

    q = (queue_s *)queue;
    WIND_ASSERT_RETURN(q->magic == WIND_QUEUE_MAGIC,ERR_INVALID_PARAM);
    wind_queue_lock(q);
    q->rd = q->buf;
    q->wr = q->buf;
    q->count = 0;                                           /* ������ĿΪ0 */
    wind_queue_unlock(q);
    return ERR_OK;
}

w_err_t wind_queue_destory(void *queue)
{
    queue_s *q;
    w_err_t err;
    WIND_ASSERT_RETURN(queue != NULL,ERR_NULL_POINTER);

    q = (queue_s *)queue;
    WIND_ASSERT_RETURN(q->magic == WIND_QUEUE_MAGIC,ERR_INVALID_PARAM);
    wind_queue_lock(q);
    q->magic = 0;
    wind_queue_unlock(q);
    if(q->lock_type == LOCK_TYPE_GLOBAL)
    {
#if WIND_LOCK_SUPPORT > 0
        err = wind_lock_free(q->lock);
        WIND_ASSERT_RETURN(err == ERR_OK,err);
#endif
    }
    return ERR_OK;
}





