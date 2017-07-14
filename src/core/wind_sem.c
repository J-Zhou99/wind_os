/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_sem.h
**创   建   人: 周江村
**最后修改日期: 2012.09.26
**描        述: wind os的信号量头文件
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 2012.09.26
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 周江村
** 日　期: 2012.10.20
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "wind_config.h"
#include "wind_types.h"
#include "wind_thread.h"
#include "wind_sem.h"
#include "wind_time.h"
#include "wind_os_hwif.h"
#include "wind_list.h"
#include "wind_err.h"
#include "wind_stat.h"
#include "wind_debug.h"
#include "wind_var.h"
#include "wind_assert.h"
#if WIND_SEM_SUPPORT > 0


extern void wind_thread_dispatch(void);
extern list_s procsleeplist;

static psem_s sem_malloc()
{
    psem_s psem;
    psem = wind_core_alloc(STAT_SEM);
    WIND_ASSERT_RETURN(psem != NULL,NULL);
    psem->used = B_TRUE;
    return psem;
}


psem_s wind_sem_create(const char *name,w_uint16_t semValue)
{
    psem_s psem;
    psem = sem_malloc();
    WIND_ASSERT_RETURN(psem != NULL,NULL);
    wind_close_interrupt();
    psem->name = name;
    psem->used = B_TRUE;
    psem->sem_num = semValue;
    wind_list_init(&psem->waitlist);
    wind_open_interrupt();
    return psem;
}



w_err_t wind_sem_post(psem_s psem)
{
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    if(psem->waitlist.head == NULL)
    {
        if(psem->sem_num < 65535)
            psem->sem_num ++;
        wind_open_interrupt();
        return ERR_OK;
    }
    pnode = wind_list_remove(&psem->waitlist,psem->waitlist.head);
    pthread = (pthread_s)pnode->obj;
    pthread->proc_status = PROC_STATUS_READY;
    pthread->cause = CAUSE_NONE;
    wind_node_free(pnode);
    wind_open_interrupt();
    wind_thread_dispatch();
    return ERR_OK;
}


w_err_t wind_sem_fetch(psem_s psem,w_uint32_t timeout)
{
    pnode_s pnode;
    pnode_s pnode1;
    w_int32_t ticks;
    pthread_s pthread;
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);

    wind_close_interrupt();
    if (psem->sem_num > 0)
    {
        psem->sem_num --;
        wind_open_interrupt();
        return ERR_OK; //信号量有效，直接返回效，
    }
    wind_open_interrupt();
    pnode = wind_node_malloc(CORE_TYPE_PCB);
    WIND_ASSERT_RETURN(pnode != NULL,ERR_NULL_POINTER);
    ticks = timeout * WIND_TICK_PER_SEC / 1000;
    if(ticks == 0)
        ticks = 1;
    pthread = wind_get_cur_proc();
    pthread->proc_status = PROC_STATUS_SUSPEND;
    pthread->cause = CAUSE_SEM;
    pthread->sleep_ticks = ticks;

    wind_node_bindobj(pnode,CORE_TYPE_PCB,pthread->prio,pthread);
    wind_close_interrupt();
    //wind_list_insert_with_minus(&psem->waitlist,pnode);
    wind_list_insert(&psem->waitlist,pnode);
    wind_thread_dispatch();
    if(pthread->cause == CAUSE_SEM)
    {
        wind_list_remove(&procsleeplist,pnode1);
        wind_node_free(pnode1);
    }
    else if(pthread->cause == CAUSE_SLEEP)
    {
        wind_list_remove(&psem->waitlist,pnode);
        wind_node_free(pnode1);
    }
    wind_open_interrupt();
    if(pthread->cause == CAUSE_SLEEP)
        return ERR_TIMEOUT;
    return ERR_OK;
}



//试图释放一个信号量，如果有线程被阻塞，则释放将终止
w_err_t wind_sem_tryfree(psem_s psem)
{
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    if(psem->waitlist.head != NULL)
    {
        wind_open_interrupt();
        return ERR_COMMAN;
    }
    psem->used = B_FALSE;
    psem->name = NULL;
    wind_core_free(STAT_SEM,psem);
    wind_open_interrupt();
    return ERR_OK;
}
w_err_t wind_sem_free(psem_s psem)
{
    w_err_t err;
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    
    while(psem->waitlist.head)
    {
        pnode = wind_list_remove(&psem->waitlist,psem->waitlist.head);
        pthread = (pthread_s)pnode->obj;
        pthread->proc_status = PROC_STATUS_READY;
        pthread->cause = CAUSE_SEM;
        wind_node_free(pnode);
        pnode = psem->waitlist.head;
    }
    psem->used = B_FALSE;
    psem->name = NULL;
    wind_core_free(STAT_SEM,psem);
    wind_open_interrupt();
    return err;    
}

w_err_t wind_sem_test(void)
{
    psem_s psem = wind_sem_create("semtest",1);
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);
    WIND_ERROR("wind_sem:%d\r\n",psem);
    wind_sem_fetch(psem,1000);
    wind_sem_post(psem);
    wind_sem_free(psem);
    return ERR_OK;
}

#endif

