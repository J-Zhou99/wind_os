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
#include "wind_config.h"
#include "wind_type.h"
#include "wind_thread.h"
#include "dlist.h"
#include "wind_pipe.h"
#include "wind_msgbox.h"
#include "wind_sem.h"
#include "wind_lock.h"
#include "wind_timer.h"
#include "wind_err.h"
#include "wind_mpool.h"
#include "core_obj.h"
#include "wind_assert.h"
core_pools_s g_pool;

void wind_corepool_init(void)
{
    wind_pool_create("thread",g_pool.thread,sizeof(g_pool.thread),sizeof(thread_s));
#if (WIND_PIPE_SUPPORT && WIND_QUEUE_SUPPORT)
    wind_pool_create("pipe",g_pool.pipe,sizeof(g_pool.pipe),sizeof(pipe_s));    
#endif
#if WIND_MESSAGE_SUPPORT
    wind_pool_create("msg",g_pool.msg,sizeof(g_pool.msg),sizeof(msg_s));
    wind_pool_create("mbox",g_pool.mbox,sizeof(g_pool.mbox),sizeof(mbox_s));
#endif
#if WIND_SEM_SUPPORT
    wind_pool_create("sem",g_pool.sem,sizeof(g_pool.sem),sizeof(sem_s));
#endif
#if WIND_TIMER_SUPPORT
    wind_pool_create("timer",g_pool.timer,sizeof(g_pool.timer),sizeof(timer_s));
#endif
    wind_pool_create("lock",g_pool.lock,sizeof(g_pool.lock),sizeof(lock_s));
    
#if WIND_STKPOOL_SUPPORT
    wind_pool_create("stkbuf",g_pool.stkbuf,sizeof(g_pool.stkbuf),WIND_STK_SIZE * sizeof(w_stack_t));
#endif
}


void *wind_core_alloc(obj_id_e type)
{
    void *p;
    switch(type)
    {
    case IDX_THREAD:
        p = wind_pool_alloc(g_pool.thread);
        break;
    case IDX_LOCK:
        p = wind_pool_alloc(g_pool.lock);
        break;
#if WIND_SEM_SUPPORT
    case IDX_SEM:
        p = wind_pool_alloc(g_pool.sem);
        break;
#endif
#if (WIND_PIPE_SUPPORT && WIND_QUEUE_SUPPORT)
    case IDX_PIPE:
        p = wind_pool_alloc(g_pool.pipe);
        break;
#endif
#if WIND_MESSAGE_SUPPORT
    case IDX_MSG:
        p = wind_pool_alloc(g_pool.msg);
        break;
#endif
#if WIND_TIMER_SUPPORT
    case IDX_TIMER:
        p = wind_pool_alloc(g_pool.timer);
        break;
#endif
#if WIND_STKPOOL_SUPPORT
    case IDX_STACK:
        p = wind_pool_alloc(g_pool.stkbuf);
        break;
#endif
    default:p = NULL;
    }
    if(p != NULL)
    {
        WIND_STAT_INC(type);
        WIND_STAT_MAX(type);
    }
    return p;
}

w_err_t wind_core_free(obj_id_e type,void *block)
{
    w_err_t err;
    WIND_ASSERT_RETURN(block != NULL,ERR_NULL_POINTER);
    switch(type)
    {
    case IDX_THREAD:
        err = wind_pool_free(g_pool.thread,block);
        break;
    case IDX_LOCK:
        err = wind_pool_free(g_pool.lock,block);
        break;
#if WIND_SEM_SUPPORT
    case IDX_SEM:
        err = wind_pool_free(g_pool.sem,block);
        break;
#endif
#if (WIND_PIPE_SUPPORT && WIND_QUEUE_SUPPORT)
    case IDX_PIPE:
        err = wind_pool_free(g_pool.pipe,block);
        break;
#endif
#if WIND_MESSAGE_SUPPORT
    case IDX_MSG:
        err = wind_pool_free(g_pool.msg,block);
        break;
#endif
#if WIND_TIMER_SUPPORT
    case IDX_TIMER:
        err = wind_pool_free(g_pool.timer,block);
        break;
#endif
#if WIND_STKPOOL_SUPPORT
    case IDX_STACK:
        err = wind_pool_free(g_pool.stkbuf,block);
        break;
#endif
    default:
        err = ERR_COMMAN;
        break;
    }
    if(err == ERR_OK)
    {
        WIND_STAT_MINUS(type);
    }
    return err;
}

void print_core_pool(void)
{
    
}
