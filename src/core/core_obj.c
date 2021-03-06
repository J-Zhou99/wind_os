/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: core_obj.h / core_obj.c
**创   建   人: 周江村
**最后修改日期: 2012.09.26
**描        述: wind os的内核相关的变量的集合
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 2013.11.27
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 周江村
** 日　期: 2013.11.27
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "wind_config.h"
#include "wind_type.h"
#include "wind_thread.h"
#include "wind_dlist.h"
#include "wind_pipe.h"
#include "wind_msgbox.h"
#include "wind_sem.h"
#include "wind_lock.h"
#include "wind_timer.h"
#include "wind_debug.h"
#include "wind_mpool.h"
#include "core_obj.h"
core_pools_s g_pool;

void wind_corepool_init(void)
{
    wind_pool_create("thread",g_pool.thread,sizeof(g_pool.thread),sizeof(thread_s));
#if (WIND_PIPE_SUPPORT)
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
    wind_pool_create("stkbuf",g_pool.stkbuf,sizeof(g_pool.stkbuf),WIND_STK_SIZE *sizeof(w_stack_t));
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
#if (WIND_PIPE_SUPPORT)
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
#if (WIND_PIPE_SUPPORT)
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
    return err;
}

void print_core_pool(void)
{
    
}

