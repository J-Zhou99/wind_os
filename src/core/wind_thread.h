/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_thread.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: �߳���صĴ���ͷ�ļ�
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
#ifndef WIND_THREADESS_H_
#define WIND_THREADESS_H_


#include "wind_config.h"
#include "wind_types.h"
#include "wind_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PCB_NUM_LIMIT 32767 //�߳�����������ֵ
#define PROCESS_NAME_LEN 20 //�߳�������󳤶ȣ����� '\0'

//�̵߳����ȵȼ���
typedef enum _PRIOLEVEL
{
    PRIO_ZERO,
    PRIO_HIGH,
    PRIO_MID,
    PRIO_LOW,
    PRIO_SYS_LOW
} prio_e;
//�߳�״̬�б�
typedef enum __proc_status
{
    PROC_STATUS_READY = 1,//����״̬
    PROC_STATUS_SUSPEND,//����״̬������ͨ����ʱ������
    PROC_STATUS_SLEEP,//����״̬�����򽫲���ͨ����ʱ�����ѣ�����Ҫ�ֶ�����
    PROC_STATUS_DEAD,//����״̬������Զ���ᱻ����
    PROC_STATUS_UNKNOWN//δ֪״̬
}proc_status_e;

typedef enum __suscause
{
    CAUSE_COM,
    CAUSE_SLEEP,
    CAUSE_SEM,
    CAUSE_MSG,
    CAUSE_LOCK,
    CAUSE_CNT
}suscause_e;


//�������߳���ص�һЩ�ص���������Ҫ����ѡ��֧��
#if WIND_THREAD_CALLBACK_SUPPORT > 0
typedef enum __procevt_e
{
    //PROCEVT_CREATE,
    PROCEVT_START,
    PROCEVT_SUSPEND,
    PROCEVT_RESUME,
    PROCEVT_DEAD
}procevt_e;


struct _pcb_s;
typedef struct __procCB_s
{
    //void (*proc_created)(struct _pcb_s *pthread);
    void (*start)(struct _pcb_s *pthread);
    void (*suspend)(struct _pcb_s *pthread);
    void (*resume)(struct _pcb_s *pthread);
    void (*dead)(struct _pcb_s *pthread);
}procCB_s;
#endif 


//�߳̿���PCB
typedef struct _pcb_s
{
    pstack_t pstk;//ע��pstk�������Ҫ���ڵ�һ��λ���ϣ�����Ų��
    pstack_t pstktop;
    err_t (*procfunc)(s32_t argc,s8_t **argv);
    s16_t argc;
    s8_t **argv;
    struct _pcb_s *parent;
    char name[PROCESS_NAME_LEN];//;//
    bool_t used;
    s16_t prio;
    s8_t proc_status;
    u16_t stksize;
    suscause_e cause;//����״̬�仯��ԭ��
#if WIND_THREAD_CALLBACK_SUPPORT > 0
    procCB_s cb;
#endif

#if WIND_HEAP_SUPPORT > 0 && WIND_PRIVATE_HEAP_SUPPORT > 0
    void *private_heap;
#endif
}pcb_s,*pthread_s;


//��ȡ��ǰ���߳�PCB
pthread_s wind_get_cur_proc(void);
//���ⲿ���񴴽�
bool_t wind_thread_isopen(void);

err_t wind_thread_getname(pthread_s pthread,s8_t *name);
pthread_s wind_get_proc_byname(s8_t *name);
//err_t wind_thread_get_super_permission(void);
pthread_s get_pcb_byname(s8_t *name);
//�����������֧����Ϣ����ģʽ������ʱ���ܲ���֧�֣���ΪһЩ֧�Ŵ��뻹û�н���
pthread_s wind_thread_create(const s8_t *name,
                   prio_e priolevel,
                   err_t (*procfunc)(s32_t argc,s8_t **argv),
                   s16_t argc,
                   s8_t **argv,
                   pstack_t psck,
                   u16_t stksize);
err_t wind_thread_changeprio(pthread_s pthread,s16_t prio);
err_t wind_thread_start(pthread_s pthread);
err_t wind_thread_suspend(pthread_s pthread);
err_t wind_thread_resume(pthread_s pthread);
err_t wind_thread_kill(pthread_s pthread);
err_t wind_thread_killN(s8_t *name);
s8_t *wind_thread_get_curname(void);
err_t wind_thread_exit(err_t exitcode);

#if WIND_THREAD_CALLBACK_SUPPORT > 0
err_t wind_thread_callback_register(pthread_s pthread,procevt_e id,void(*cb)(pthread_s));
#endif
err_t wind_thread_showlist(pnode_s nodes);

#if WIND_THREAD_MAX_NUM > PCB_NUM_LIMIT
#error "PCB_MAX_NUM should not be a number larger than PCB_NUM_LIMIT!"
#endif //PCB_MAX_NUM > PCB_NUM_LIMIT

#if WIND_THREAD_MAX_NUM < 8
#error "WIND_THREAD_MAX_NUM should not be a number smaller than 8,as operation system reserves some processes!"
#endif


#ifdef __cplusplus
}
#endif

#endif //WIND_THREADESS_H_
