/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_var.h / wind_var.c
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
#ifndef WIND_VAR_H__
#define WIND_VAR_H__

#include "wind_config.h"
#include "wind_type.h"
#include "wind_thread.h"
#include "wind_list.h"
#include "dlist.h"
#include "wind_pipe.h"
#include "wind_message.h"
#include "wind_sem.h"
#include "wind_lock.h"
#include "wind_timer.h"
#include "wind_stati.h"
#include "wind_mpool.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct __core_var_s
{
    WIND_MPOOL(pcb,WIND_THREAD_MAX_NUM,sizeof(thread_s));
    WIND_MPOOL(node,WIND_NODE_MAX_NUM,sizeof(node_s));
#if WIND_PIPE_SUPPORT > 0
    WIND_MPOOL(pipe,WIND_PIPE_MAX_NUM,sizeof(pipe_s));
#endif
#if WIND_MESSAGE_SUPPORT > 0
    WIND_MPOOL(msg,WIND_MESSAGE_MAX_NUM,sizeof(msg_s));
    WIND_MPOOL(mbox,WIND_MBOX_MAX_NUM,sizeof(mbox_s));
#endif
#if WIND_SEM_SUPPORT > 0
    WIND_MPOOL(sem,WIND_SEM_MAX_NUM,sizeof(sem_s));
#endif
#if WIND_TIMER_SUPPORT > 0
    WIND_MPOOL(ttimer,WIND_TTIMER_MAX_NUM,sizeof(ttimer_s));
#endif
#if WIND_LOCK_SUPPORT > 0
    WIND_MPOOL(lock,WIND_LOCK_NUM,sizeof(lock_s));
#endif
    //����һЩ��ջ�Ա㴴��������ʱʹ��
    WIND_MPOOL(stkbuf,WIND_STK_MAX_NUM,WINS_STK_SIZE * sizeof(w_stack_t));
    
    list_s threadlist;
    dlist_s semlist;
    dlist_s locklist;
    list_s ttmerlist;
    w_int16_t pcbcnt;//�̼߳�����
    volatile w_bool_t run_falg;//���̵߳��ȿ�ʼ�ı�־
    w_bool_t usrprocen;/*�û��߳��������ı�־ */
    
    volatile w_uint32_t idle_cnt;//���м�����
    w_uint32_t ticks_cnt;//tick��ʱ
    w_uint32_t idle_cnt_max;//��һ��ʱ���ڵ�idle����ļ���ֵ
    w_uint32_t cpu_usage;

    //�߳����ö���
    pthread_s pmain;
    pthread_s pidle;
    pthread_s pstat;
    pthread_s pctrl;
    pthread_s pdaemon;
    pthread_s ptimer;
}core_var_s;
extern core_var_s g_core;//�ں���صĲ�����
extern volatile w_bool_t gwind_start_flag;//��ʼ���ȵı�־

extern pstack_t *gwind_high_stack;
extern pstack_t *gwind_cur_stack;

void wind_corevar_init(void);

void *wind_core_alloc(stat_e type);

w_err_t wind_core_free(stat_e type,void *block);

pstack_t wind_stack_alloc(w_uint32_t size);

w_err_t wind_stack_free(pstack_t pstack);

//CPU������ռ����
#define WIND_CPU_USAGE (g_core.cpu_usage)

#define IDLE_CNT_PER_SEC (g_core.idle_cnt_max)

#define RUN_FLAG (gwind_start_flag)

#define TICKS_CNT (g_core.ticks_cnt)


#ifdef __cplusplus
}
#endif


#endif

