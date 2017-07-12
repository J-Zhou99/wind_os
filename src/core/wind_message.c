/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_message.c
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
** �ա���: 2012.10.20
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/

#include "wind_config.h"
#include "wind_types.h"
#include "wind_os_hwif.h"
#include "wind_message.h"
#include "wind_core.h"
#include "wind_list.h"
#include "wind_stat.h"
#include "wind_var.h"
#include "wind_err.h"
#include "wind_assert.h"
#if WIND_MESSAGE_SUPPORT > 0
//extern list_s procsleeplist;
extern void wind_thread_dispatch(void);

static pmsg_s msg_malloc(void)
{
    return (pmsg_s)wind_core_alloc(STAT_MESSAGE);
}

static err_t msg_free(pmsg_s msg)
{
    return wind_core_free(STAT_MESSAGE,(void *)msg);
}

static pmbox_s mbox_malloc(void)
{
    return (pmbox_s)wind_core_alloc(STAT_MBOX);
}

static err_t mbox_free(pmbox_s mbox)
{
    return wind_core_free(STAT_MBOX,(void *)mbox);
}

//********************************************internal functions******************************


static err_t mbox_delete_msgs(pnode_s list)
{
    pnode_s pnode = list,pnode1;
    WIND_ASSERT_RETURN(list != NULL,ERR_NULL_POINTER);
    pnode1 = pnode->next;
    while(pnode)
    {
        wind_core_free(STAT_MESSAGE,(void *)(pmsg_s)(pnode->obj));
        msg_free((void *)(pmsg_s)(pnode->obj));
        wind_node_free(pnode);
        pnode = pnode1;
        pnode1 = pnode->next;
    }
    return ERR_OK;    
}
//********************************************internal functions******************************


//**********************************************extern functions******************************

//����һ����Ϣ����������Ϣ
pmsg_s wind_message_create(const char *name,u16_t msgNo,u16_t param,void *lparam)
{
    pmsg_s pmsg;
    pmsg = wind_core_alloc(STAT_MESSAGE);
    WIND_ASSERT_RETURN(pmsg != NULL,NULL);
    pmsg->name = name;
    pmsg->used = B_TRUE;
    //�趨��ǰ���߳̾��
    pmsg->validtime = 3000;
    pmsg->msgNO = msgNo;
    pmsg->param = param;
    pmsg->lparam = lparam;
    pmsg->used = B_TRUE;
    return pmsg;
}

err_t wind_message_destroy(pmsg_s pmsg)
{
    err_t err;
    WIND_ASSERT_RETURN(pmsg != NULL,ERR_NULL_POINTER);
    pmsg->used = B_FALSE;
    pmsg->name = NULL;
    err = wind_core_free(STAT_MESSAGE,pmsg);
    return err;
}


//�������䣬ֻ�����߳��д������������ж��к��߳�����֮ǰ
pmbox_s wind_mbox_create(const char *name)
{
    pmbox_s pmbox;
    pmbox = wind_core_alloc(STAT_MBOX);
    WIND_ASSERT_RETURN(pmbox != NULL,NULL);

    pmbox->name = name;
    pmbox->used = B_TRUE;
    pmbox->owner = wind_get_cur_proc();
    pmbox->num = 0;
    wind_list_init(&pmbox->msgq);
    pmbox->valid = B_TRUE;
    return pmbox;
}

err_t wind_mbox_destroy(pmbox_s pmbox)
{
    
    err_t err;
    pthread_s pthread;
    WIND_ASSERT_RETURN(pmbox != NULL,ERR_NULL_POINTER);
    pthread = wind_get_cur_proc();
    WIND_ASSERT_RETURN(pthread == pmbox->owner,ERR_COMMAN);
    wind_close_interrupt();
    pmbox->valid = B_FALSE;
    pmbox->owner = NULL;
    pmbox->name = NULL;
    err = mbox_delete_msgs(pmbox->msgq.head);
    WIND_ASSERT_RETURN(err == ERR_OK,err);
    wind_core_free(STAT_MBOX,pmbox);
    wind_open_interrupt();
    return ERR_OK;
}

err_t wind_mbox_post(pmbox_s mbox,pmsg_s pmsg)
{

    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(mbox != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(pmsg != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(mbox->valid,ERR_COMMAN);
    WIND_ASSERT_RETURN(mbox->owner,ERR_COMMAN);
    
    pmsg->validtime = WIND_TICK_PER_SEC * 3;
    pnode = wind_node_malloc(CORE_TYPE_MSG);
    WIND_ASSERT_RETURN(pnode != NULL,ERR_NULL_POINTER);
    wind_node_bindobj(pnode,CORE_TYPE_MSG,0,pmsg);
    wind_list_inserttoend(&mbox->msgq,pnode);
    pthread = mbox->owner;
    pthread->cause = CAUSE_MSG;
    pthread->proc_status = PROC_STATUS_READY;
    wind_thread_dispatch();//�л��߳�
    return ERR_OK;
}


err_t wind_mbox_fetch(pmbox_s mbox,pmsg_s *pmsg,u32_t timeout)
{
    s16_t cnt;
    err_t err;
    u32_t ticks;
    pnode_s pnode;
    pthread_s pthread;
    WIND_ASSERT_RETURN(mbox != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(pmsg != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(mbox->valid,ERR_COMMAN);
    WIND_ASSERT_RETURN(mbox->owner,ERR_COMMAN);
    pthread = wind_get_cur_proc();
    WIND_ASSERT_RETURN(mbox->owner == pthread,ERR_COMMAN);
    
    //�������������Ϣ����ֱ�ӷ�����Ϣ
    
    if(mbox->msgq.head != NULL)
    {
        pnode = wind_list_remove(&mbox->msgq,mbox->msgq.head);
        *pmsg = (pmsg_s)pnode->obj;
        wind_node_free(pnode);
        return ERR_OK;
    }
    //�����̼߳���˯�߶���
    ticks = timeout * WIND_TICK_PER_SEC / 1000;
    if(ticks == 0)
        ticks = 1;
    pthread = mbox->owner;
    pthread->proc_status = PROC_STATUS_SUSPEND;
    pthread->cause = CAUSE_MSG;
    
    //pnode = wind_node_malloc(CORE_TYPE_PCB);
    //WIND_ASSERT_RETURN(pnode != NULL,ERR_NULL_POINTER);
    //wind_node_bindobj(pnode,CORE_TYPE_PCB,ticks,pthread);
    //wind_list_insert_with_minus(&procsleeplist,pnode);
    wind_thread_dispatch();

    
    if(pthread->cause == CAUSE_MSG)
    {
        //�������ᷢ������Ϊ�յ����
        if(mbox->msgq.head == NULL)
        {
            pthread->proc_status = PROC_STATUS_READY;
            return ERR_NULL_POINTER;
        }
        pnode = wind_list_remove(&mbox->msgq,mbox->msgq.head);
        *pmsg = (pmsg_s)pnode->obj;
        wind_node_free(pnode);
        err = ERR_OK;
    }
    else if(pthread->cause == CAUSE_SLEEP)
    {
        err = ERR_OK;
    }
    
    return err;
}


#endif  //WIND_MESSAGE_SUPPORT

