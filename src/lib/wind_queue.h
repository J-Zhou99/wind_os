/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_queue.h
**创   建   人: 周江村
**最后修改日期: 2012.09.26
**描        述: wind os的消息相关的代码
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 2012.09.26
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 周江村
** 日　期: 2014.11.2
** 描　述: 修改了队列的互斥方法，允许系统在操作队列时局部锁定或者不锁定
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef __WIND_QUEUE_H_
#define __WIND_QUEUE_H_



#include "wind_config.h"
#include "wind_types.h"
#include "wind_lock.h"
#include "wind_err.h"

#ifdef __cplusplus
extern "C"{
#endif

#if WIND_QUEUE_SUPPORT
#define WIND_QUEUE_MAGIC 0x25a5c629
//#pragma pack(1)
typedef struct __queue_s
{
    w_uint32_t magic;
    w_uint8_t *rd;// 指向数据输出位置         
    w_uint8_t *wr; // 指向数据输入位置        
    w_uint8_t *end;// 指向buf的结束位置       
    w_uint32_t count;// 队列中数据个数          
    w_uint32_t capacity;// 队列中允许存储的数据个数 
    w_uint32_t  data_wid; // 元素的数据宽度
    w_int32_t lock_type;
#if WIND_LOCK_SUPPORT > 0
    plock_s lock;
#endif
    w_uint8_t buf[4]; // 存储数据的空间          
} queue_s,*pqueue_s;



w_err_t wind_queue_create(void *mem,
                          w_uint32_t size,
                          w_uint16_t data_wid,
                          lock_type_e lock_type
                          );


w_int32_t wind_queue_read(void *queue,void *buf,w_uint32_t len);

w_int32_t wind_queue_write(void *queue,void *buf,w_uint32_t len);

w_uint32_t wind_queue_datalen(void *queue);

w_uint32_t wind_queue_capacity(void *queue);

w_err_t wind_queue_flush(void *queue);

w_err_t wind_queue_destory(void *queue);

#endif//#if WIND_QUEUE_SUPPORT

#ifdef __cplusplus
}
#endif//#ifdef __cplusplus

#endif  //WIND_QUEUE_H_

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

