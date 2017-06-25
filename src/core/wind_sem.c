/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_sem.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os���ź���ͷ�ļ�
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


//extern pthread_s wind_get_cur_proc(void);
extern void wind_thread_dispatch(void);
extern pnode_s procsleeplist;//sleeping process list

static psem_s sem_malloc()
{
    psem_s psem;
    psem = wind_core_alloc(STAT_SEM);
    WIND_ASSERT_RETURN(psem != NULL,NULL);
    psem->used = B_TRUE;
    return psem;
}


psem_s wind_sem_create(const char *name,u16_t semValue)
{
    psem_s psem;
    psem = sem_malloc();
    WIND_ASSERT_RETURN(psem != NULL,NULL);
    wind_close_interrupt();
    psem->name = name;
    psem->used = B_TRUE;
    psem->sem_num = semValue;
    psem->waitlist = NULL;
    wind_open_interrupt();
    return psem;
}



err_t wind_sem_post(psem_s psem)
{
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    if(psem->waitlist == NULL)
    {
        if(psem->sem_num < 65535)
            psem->sem_num ++;
        wind_open_interrupt();
        return ERR_OK;
    }
    pnode = psem->waitlist;
    psem->waitlist = pnode->next;
    pthread = (pthread_s)pnode->obj;
    pthread->proc_status = PROC_STATUS_READY;
    pthread->cause = CAUSE_SEM;
    wind_node_free(pnode);
    wind_open_interrupt();
    wind_thread_dispatch();
    return ERR_OK;
}


err_t wind_sem_fetch(psem_s psem,u32_t timeout)
{
    s16_t index;
    pnode_s pnode,pnode1;
    s32_t ticks;
    pthread_s pthread;
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);

    wind_close_interrupt();
    if (psem->sem_num > 0)
    {
        psem->sem_num --;
        wind_open_interrupt();
        return ERR_OK; //�ź�����Ч��ֱ�ӷ���Ч��
    }
    wind_open_interrupt();
    //���������ڵ�
    pnode = wind_node_malloc(CORE_TYPE_PCB);
    WIND_ASSERT_RETURN(pnode != NULL,ERR_NULL_POINTER);
    pnode1 = wind_node_malloc(CORE_TYPE_PCB);
    if(pnode1 == NULL)
    {
        wind_node_free(pnode);
        return ERR_NULL_POINTER;
    }
    ticks = timeout * WIND_TICK_PER_SEC / 1000;
    if(ticks == 0)
        ticks = 1;
    pthread = wind_get_cur_proc();
    pthread->proc_status = PROC_STATUS_SUSPEND;
    pthread->cause = CAUSE_SEM;
    pnode->obj = pthread;
    pnode->key = pthread->prio;
    pnode1->obj = pthread;
    pnode1->key = ticks;
    wind_close_interrupt();
    wind_list_insert_with_minus(&psem->waitlist,pnode);
    wind_list_insert_with_minus(&procsleeplist,pnode1);
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



//��ͼ�ͷ�һ���ź�����������̱߳����������ͷŽ���ֹ
err_t wind_sem_tryfree(psem_s psem)
{
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    if(psem->waitlist != NULL)
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
err_t wind_sem_free(psem_s psem)
{
    err_t err;
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(psem != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    while(psem->waitlist)
    {
        pnode = psem->waitlist;
        pthread = (pthread_s)pnode->obj;
        pthread->proc_status = PROC_STATUS_READY;
        pthread->cause = CAUSE_SEM;
        wind_node_free(pnode);
    }
    psem->used = B_FALSE;
    psem->name = NULL;
    wind_core_free(STAT_SEM,psem);
    wind_open_interrupt();
    return err;    
}

err_t wind_sem_test(void)
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

