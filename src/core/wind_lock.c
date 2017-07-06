/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_lock.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: ϵͳ�Ļ�����
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.2
** �ա���: 2013.06.23
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2012.06.23
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "wind_config.h"
#include "wind_types.h"
#include "wind_thread.h"
#include "wind_lock.h"
#include "wind_os_hwif.h"

#include "wind_list.h"
#include "wind_err.h"
#include "wind_stat.h"
#include "wind_var.h"
#include "wind_assert.h"
#if WIND_LOCK_SUPPORT > 0

extern pthread_s wind_get_cur_proc(void);
extern void wind_thread_dispatch(void);

static plock_s lock_malloc()
{
    plock_s plock;
    plock = wind_core_alloc(STAT_LOCK);
    if(plock == NULL)
    {
        return NULL;;
    }
    plock->used = B_TRUE;
    return plock;
}




plock_s wind_lock_create(const char *name)
{
    plock_s plock;
    wind_close_interrupt();
    plock = lock_malloc();
    plock->name = name;
    if(plock == NULL)
    {
        wind_open_interrupt();
        return NULL;
    }
    plock->locked = B_FALSE;
    wind_list_init(&plock->waitlist);
    wind_open_interrupt();
    return plock;
}

//��ͼ�ͷ�һ����������������̱߳����������ͷŽ���ֹ
err_t wind_lock_tryfree(plock_s plock)
{

    WIND_ASSERT_RETURN(plock != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    WIND_ASSERT_TODO(plock->locked == B_FALSE,wind_open_interrupt(),ERR_COMMAN);
    plock->used = B_FALSE;
    plock->name = NULL;
    wind_open_interrupt();
    return ERR_OK;    
}

//ǿ�����ͷŻ��������������еı��û������������߳�ȫ������
err_t wind_lock_free(plock_s plock)
{
    //plock_s plock;
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(plock != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    while(plock->waitlist.head != NULL)
    {
        pnode = plock->waitlist.head;
        wind_list_remove(&plock->waitlist,pnode);
        pthread = (pthread_s)pnode->obj;
        pthread->proc_status = PROC_STATUS_READY;
        pthread->cause = CAUSE_LOCK;
        wind_node_free(pnode);
    }
    plock->used = B_FALSE;
    plock->name = NULL;
    wind_core_free(STAT_LOCK,plock);
    wind_open_interrupt();
    return ERR_OK;
}

//��ͼ����һ��������������Ѿ������������߳̽�������
err_t wind_lock_close(plock_s plock)
{
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(plock != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();

    if (plock->locked == B_FALSE)
    {
        plock->locked = B_TRUE;
        wind_open_interrupt();
        return ERR_OK; //�ź�����Ч��ֱ�ӷ���Ч��
    }
    pnode = wind_node_malloc(CORE_TYPE_SEM);
    WIND_ASSERT_RETURN(pnode != NULL,ERR_NULL_POINTER);
    pthread = wind_get_cur_proc();
    pthread->proc_status = PROC_STATUS_SUSPEND;
    pthread->cause = CAUSE_LOCK;
    wind_node_bindobj(pnode,CORE_TYPE_PCB,pthread->prio,pthread);
    wind_list_insert(&plock->waitlist,pnode);
    wind_thread_dispatch();
    wind_open_interrupt();
    return ERR_OK;
}

//��ͼ��һ����������������̱߳������������ȼ����߳�
err_t wind_lock_open(plock_s plock)
{
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(plock != NULL,ERR_NULL_POINTER);
    wind_close_interrupt();
    WIND_ASSERT_TODO(plock->locked,wind_open_interrupt(),ERR_OK);
    if (plock->waitlist.head == NULL)
    {
        plock->locked = B_FALSE;
        wind_open_interrupt();
        return ERR_OK; //�ź�����Ч��ֱ�ӷ���Ч��
    }
    pnode = wind_list_remove(&plock->waitlist,plock->waitlist.head);
    pthread = (pthread_s)pnode->obj;
    
    pthread->proc_status = PROC_STATUS_READY;
    pthread->cause = CAUSE_LOCK;
    wind_node_free(pnode);
    wind_open_interrupt();
    return ERR_OK;    
}

#endif //WIND_LOCK_SUPPORT > 0

