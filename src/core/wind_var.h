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
#include "wind_types.h"
#include "wind_thread.h"
#include "wind_list.h"
#include "wind_pipe.h"
#include "wind_message.h"
#include "wind_sem.h"
#include "wind_lock.h"
#include "wind_ticktimer.h"
#include "wind_stat.h"
#include "wind_mpool.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct __core_var_s
{
    WIND_MPOOL(pcb,WIND_THREAD_MAX_NUM,sizeof(pcb_s));
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
#if WIND_TTIMER_SUPPORT > 0
    WIND_MPOOL(ttimer,WIND_TTIMER_MAX_NUM,sizeof(ttimer_s));
#endif
#if WIND_LOCK_SUPPORT > 0
    WIND_MPOOL(lock,WIND_LOCK_NUM,sizeof(lock_s));
#endif
    //����һЩ��ջ�Ա㴴��������ʱʹ��
    WIND_MPOOL(stk128,WIND_STK128_MAX_NUM,128 * sizeof(stack_t));
    WIND_MPOOL(stk256,WIND_STK256_MAX_NUM,256 * sizeof(stack_t));
    WIND_MPOOL(stk512,WIND_STK512_MAX_NUM,512 * sizeof(stack_t));
    WIND_MPOOL(stk1024,WIND_STK1024_MAX_NUM,1024 * sizeof(stack_t));
    WIND_MPOOL(stk2048,WIND_STK2048_MAX_NUM,2048 * sizeof(stack_t));
    
    pnode_s pcblist;
    pnode_s ttmerlist;
    s16_t pcbcnt;//�̼߳�����
    u32_t idle_cnt;//���м�����
    bool_t usrprocen;/*�û��߳��������ı�־ */
    bool_t is_incore;
    
    u32_t g_wind_cpu_usage;
    pthread_s pmain,pidle,pstat,pctrl,pdaemon;
    
    u32_t idle_cnt_per_sec;//��һ��ʱ���ڵ�idle����ļ���ֵ
    volatile bool_t run_falg;//���̵߳��ȿ�ʼ�ı�־
    u32_t ticks_cnt;//tick��ʱ

}core_var_s;
extern core_var_s g_core;//�ں���صĲ�����
extern volatile bool_t gwind_start_flag;//��ʼ���ȵı�־
extern pthread_s gwind_cur_pcb;//��ǰ��
extern pthread_s gwind_high_pcb;//������ȼ�PCBָ��

void wind_corevar_init(void);

void *wind_core_alloc(stat_e type);

err_t wind_core_free(stat_e type,void *block);

pstack_t wind_stack_alloc(u32_t size);

err_t wind_stack_free(pstack_t pstack);

//CPU������ռ����
#define WIND_CPU_USAGE (g_core.g_wind_cpu_usage)

#define IDLE_CNT_PER_SEC (g_core.idle_cnt_per_sec)

#define RUN_FLAG (gwind_start_flag)

#define TICKS_CNT (g_core.ticks_cnt)

#define HIGH_PROC (gwind_high_pcb)
#ifdef __cplusplus
}
#endif


#endif

