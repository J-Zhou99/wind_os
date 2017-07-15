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





volatile w_int8_t gwind_int_cnt = 0;//ȫ�ֵ��жϼ���ֵ

extern w_err_t wind_time_init(void);

extern void wind_thread_switch(void);
extern void wind_interrupt_switch(void);
extern void wind_start_switch(void);
extern w_err_t wind_thread_showlist(pnode_s nodes);
extern void listtest(void);
extern w_err_t consoleProc(w_int32_t argc,char **argv);

//wind core data section --- end






//�������û��߳�
static void wind_thread_open()
{
    g_core.usrprocen = B_TRUE;
}

//�鿴�Ƿ��������û��߳�
w_bool_t wind_thread_isopen()
{
    return g_core.usrprocen;
}

void wind_enter_core(void)
{

}
void wind_exit_core(void);

void wind_enter_int(void)
{
    if(RUN_FLAG == B_FALSE)
    {
        WIND_ERROR("enter not rd\r\n");
        return;
    }
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
    pthread_s pthread;
    pnode_s pnode = g_core.pcblist.head;
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





//����ϵͳ��ʼ��
void wind_init()
{
    g_core.usrprocen = B_FALSE;
    wind_target_init();//Ŀ������л�����ʼ��
    wind_std_port_init();//���Զ˿ڳ�ʼ��
    wind_print_os_info();
    
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



//****************************wind_entry***********************************************
int create_init_thread(void);

int wind_os_lunch(void)
{
    //wind_close_interrupt();
    wind_init();
    create_init_thread();
    wind_thread_open();
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


