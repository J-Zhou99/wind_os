/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_core.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os�ĺ��Ĵ���
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

#include "wind_version.h"
#include "wind_thread.h"
#include "wind_sem.h"
#include "wind_time.h"
#include "wind_pipe.h"
#include "wind_message.h"
#include "wind_core.h"
#include "wind_list.h"
#include "wind_stat.h"
#include "wind_softint.h"
#include "wind_var.h"

#include "wind_string.h"
#include "wind_debug.h"
#include "wind_ticktimer.h"
#include "wind_err.h"
#include "wind_rtc.h"
#include "wind_heap.h"

#include "console_framework.h"




#define CTRL_STK_SIZE 2048

volatile s8_t gwind_int_cnt = 0;//ȫ�ֵ��жϼ���ֵ
//volatile int gwind_close_sw_cnt = 0;
static stack_t ctrlstk[CTRL_STK_SIZE];//�������ջ

extern err_t wind_time_init(void);
extern err_t wind_main(void);
extern void wind_thread_switch(void);
extern void wind_interrupt_switch(void);
extern void wind_start_switch(void);
extern err_t wind_thread_showlist(pnode_s nodes);
extern void listtest(void);
extern err_t consoleProc(s32_t argc,char **argv);

//wind core data section --- end






//�������û��߳�
static void wind_thread_open()
{
    g_core.usrprocen = B_TRUE;
}

//�鿴�Ƿ��������û��߳�
bool_t wind_thread_isopen()
{
    return g_core.usrprocen;
}

void wind_enter_int(void)
{
    if(RUN_FLAG == B_FALSE)
    {
        WIND_ERROR("enter not rd\r\n");
        return;
    }
    //WIND_INFO("in\r\n");
    if(gwind_int_cnt < 255)
        gwind_int_cnt ++;
    return;
}

void wind_update_curPCB(void);
void wind_exit_int(void)
{
    if(RUN_FLAG == B_FALSE)
    {
        WIND_ERROR("exit not rd %d\r\n",RUN_FLAG);
        return;
    }
    
    wind_close_interrupt();
    if(gwind_int_cnt > 0)
        gwind_int_cnt --;
    if(gwind_int_cnt == 0)
    {
        wind_update_curPCB();
        if(HIGH_PROC != gwind_cur_pcb)
        {
            wind_interrupt_switch();
        }
    }
    wind_open_interrupt();
}





//ϵͳ���ȿ�ʼ��������
static void wind_run()
{
    wind_update_curPCB();
    gwind_cur_pcb = HIGH_PROC;
    wind_start_switch();
}

void wind_update_curPCB()
{
    s16_t cnt,high = -1,hprio = 32767;
    pthread_s pthread;
    pnode_s pnode = g_core.pcblist;
    //for(cnt = 0;cnt < WIND_THREAD_MAX_NUM;cnt ++)
    wind_close_interrupt();
    while(pnode)
    {    
        
        pthread = (pthread_s)(pnode->obj);
        if((pthread->used) && (pthread->proc_status == PROC_STATUS_READY))
        {
            HIGH_PROC = pthread;
            break;
        }
        pnode = pnode->next;
        
    }
    wind_open_interrupt();
}
//���߳����л��������ȼ����߳�


#if WIND_REALTIME_CORE_SUPPORT > 0
void wind_thread_dispatch(void)
{
    if(RUN_FLAG == B_FALSE)
        return;
    wind_close_interrupt();
    //WIND_DEBUG("wind_update_curPCB\r\n");
    wind_update_curPCB();

    if(HIGH_PROC != gwind_cur_pcb)
    {
        wind_open_interrupt();
        wind_thread_switch();
    }
    wind_open_interrupt();
}

#endif

init_step_s g_wind_init[] = 
{
    {NULL,NULL}
};



//����ϵͳ��ʼ��
void wind_init()
{
    
    g_core.usrprocen = B_FALSE;
    wind_target_init();//Ŀ������л�����ʼ��
    wind_std_port_init();//���Զ˿ڳ�ʼ��
    wind_output_product_info();
    
#if WIND_STAT_SUPPORT > 0
    wind_stat_init();
#endif
    wind_corevar_init();
#if WIND_HEAP_SUPPORT > 0
    wind_heap_block_init();
#endif
    wind_time_init();//ʱ���ʼ��
#if WIND_RTC_SUPPORT > 0
    //WIND_INFO("system time initializing...\r\n");
    //wind_datetime_init();
#endif
    WIND_INFO("initialization completed!\r\n");
    
}


//*******************************daemon***********************************************
#if WIND_DAEMON_SUPPORT > 0
#define DAEMON_STK_SIZE 512
void daemon_test(void)
{
    s32_t i;
    int ret = 0;
    for(i = 0;i < 100000;i ++)
    {
        ret += (663487456758+i)/1554428587;
    }
    
}
extern err_t wind_hook_daemon(void);
static stack_t daemonstk[DAEMON_STK_SIZE];//�������ջ
static err_t daemon_proc(s16_t argc,s8_t **argv)
{
    err_t err;

    u32_t dmcnt = 0;
    argc = 0;
    while(1)
    {
        //WIND_INFO("daemon process:%d!\r\n",dmcnt);
        daemon_test();
        wind_thread_sleep(50);
        dmcnt ++;
    }
    return ERR_OK;
}
#endif //WIND_DAEMON_SUPPORT
//*******************************daemon***********************************************


//****************************static***********************************************
static u32_t core_get_ticks_of_idle(u32_t ms)
{
    pthread_s pthread;
    pnode_s node;
    u32_t cnts;
    pthread_s pproc = wind_get_cur_proc();
    node = g_core.pcblist;
    g_core.idle_cnt = 0;
    WIND_DEBUG("RUN_FLAG=%d\r\n",RUN_FLAG);
    //WIND_DEBUG("static ticks idle\r\n");
    while(node)
    {
        pthread = (pthread_s)node->obj;
        if((pthread != pproc) && (pthread->prio != 32767))
        {
            pthread->proc_status = PROC_STATUS_SUSPEND;
            pthread->cause = CAUSE_COM;
            
        }
        node = node->next;
    }
    
    //wind_thread_showlist(g_core.pcblist);
    //wind_heap_showinfo();
    wind_thread_sleep(ms);
    cnts = g_core.idle_cnt;
    node = g_core.pcblist;
    while(node)
    {
        pthread = (pthread_s)node->obj;
        if((pthread != pproc) && (pthread->prio != 32767))
        {
            pthread->proc_status = PROC_STATUS_READY;
            pthread->cause = CAUSE_COM;
        }
        node = node->next;
    }
    //wind_thread_showlist(g_core.pcblist);
    //wind_heap_showinfo();
    return cnts;
}

//���̺߳���
//ͳ���̺߳���
#define STATI_STK_SIZE 1024
static stack_t statisstk[STATI_STK_SIZE];//ͳ�������ջ
static err_t stati_proc(s16_t argc,s8_t **argv)
{
    
    u32_t statcnt = 0,i = 3;
    argc = 0;
    //wind_heap_showinfo();
    wind_thread_sleep(3000);
    IDLE_CNT_PER_SEC = core_get_ticks_of_idle(2000);
    wind_printf("idle cnt:%d\r\n",IDLE_CNT_PER_SEC);
    //wind_heap_showinfo();
    while(1)
    {
        statcnt = g_core.idle_cnt;
        wind_thread_sleep(2000);
        //wind_heap_showinfo();
        statcnt = g_core.idle_cnt - statcnt;
        wind_printf("run cnt:%d\r\n",statcnt);
        WIND_CPU_USAGE = (IDLE_CNT_PER_SEC - statcnt) * 100 / IDLE_CNT_PER_SEC;
        wind_printf("cpu usage:%d%%\r\n",WIND_CPU_USAGE);
    }
    return ERR_OK;
}
//****************************static***********************************************

//****************************idle_proc***********************************************
#define IDLE_STK_SIZE 256
static stack_t idlestk[IDLE_STK_SIZE];//���������ջ
//�����̺߳���
//extern void wind_sem_test(void);
static err_t idle_proc(s16_t argc,s8_t **argv)
{    
    //wind_sem_test();
    while(1)
    {
        g_core.idle_cnt ++;
    }
    return ERR_OK;
}

//****************************idle_proc***********************************************



//****************************wind_entry***********************************************
#define MAIN_STK_SIZE 512
static stack_t mainstk[MAIN_STK_SIZE];//�������ջ
static err_t wind_entry(s16_t argc,s8_t **argv)
{   
    wind_tick_init();
    WIND_INFO("tick init\r\n");
    
#if WIND_SOFTINT_SUPPORT > 0    
        wind_create_softint_proc();
#endif
        g_core.pstat = wind_thread_create("statistics",PRIO_HIGH,stati_proc,
                         0,NULL,statisstk,STATI_STK_SIZE);
#if WIND_DAEMON_SUPPORT > 0
        g_core.pdaemon = wind_thread_create("daemon",PRIO_HIGH,daemon_proc,
                         0,NULL,daemonstk,DAEMON_STK_SIZE);
#endif
        //���������߳�
        g_core.pidle = wind_thread_create("idle",PRIO_LOW,idle_proc,
                        0,NULL,idlestk,IDLE_STK_SIZE);
        wind_thread_changeprio(g_core.pidle,32767);
        
    
       
#if WIND_CONSOLE_SUPPORT > 0
        g_core.pctrl = wind_thread_create("console",PRIO_LOW,consoleProc,
                   0,NULL,ctrlstk,CTRL_STK_SIZE);
        wind_thread_changeprio(g_core.pctrl,32760);
#endif

    wind_thread_showlist(g_core.pcblist);
    WIND_INFO("Running main proc...\r\n");
    wind_main();
    while(1)
        wind_thread_sleep(1000);
    return ERR_OK;
}

//****************************wind_entry***********************************************

int wind_os_lunch(void)
{
    
    wind_close_interrupt();
    wind_init();//ϵͳ��ʼ��

    //�����û������������߳�
    g_core.pmain = wind_thread_create("entry",PRIO_HIGH,wind_entry,
                    0,NULL,mainstk,MAIN_STK_SIZE);
    wind_thread_open();//��ϵͳ��ʼ��֮ǰ����������
    //��Ҫ�����������̵߳���
    WIND_INFO("wind is ready for running!\r\n");
    wind_thread_open();
    wind_run();
    return ERR_OK;
}


//C��������
//int main(int argc,char **argv)
int main(void)
{
    wind_os_lunch();
    return 0;
}


