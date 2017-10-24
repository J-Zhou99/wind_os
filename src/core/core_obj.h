/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: core_obj.h / core_obj.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os���ں���صı����ļ���
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
#ifndef CORE_OBJ_H__
#define CORE_OBJ_H__

#include "wind_config.h"
#include "wind_type.h"
#include "wind_thread.h"
#include "dlist.h"
#include "wind_pipe.h"
#include "wind_msgbox.h"
#include "wind_sem.h"
#include "wind_lock.h"
#include "wind_timer.h"
#include "wind_stati.h"
#include "wind_mpool.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct __core_pool_s
{
    WIND_MPOOL(thread,WIND_THREAD_MAX_NUM,sizeof(thread_s));
#if (WIND_PIPE_SUPPORT && WIND_QUEUE_SUPPORT)
    WIND_MPOOL(pipe,WIND_PIPE_MAX_NUM,sizeof(pipe_s));
#endif
#if WIND_MESSAGE_SUPPORT
    WIND_MPOOL(msg,WIND_MESSAGE_MAX_NUM,sizeof(msg_s));
    WIND_MPOOL(mbox,WIND_MBOX_MAX_NUM,sizeof(mbox_s));
#endif
#if WIND_SEM_SUPPORT
    WIND_MPOOL(sem,WIND_SEM_MAX_NUM,sizeof(sem_s));
#endif
#if WIND_TIMER_SUPPORT
    WIND_MPOOL(timer,WIND_TIMER_MAX_NUM,sizeof(timer_s));
#endif
    WIND_MPOOL(lock,WIND_LOCK_NUM,sizeof(lock_s));
#if WIND_STKPOOL_SUPPORT
    WIND_MPOOL(stkbuf,WIND_STK_MAX_NUM,WIND_STK_SIZE * sizeof(w_stack_t));
#endif
}core_pools_s;

void wind_corepool_init(void);

void *wind_core_alloc(objid_e type);

w_err_t wind_core_free(objid_e type,void *block);

void print_core_pool(void);


#ifdef __cplusplus
}
#endif


#endif

