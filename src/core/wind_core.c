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

#include "wind_type.h"
#include "wind_config.h"
#include "wind_os_hwif.h"

#include "wind_version.h"
#include "wind_thread.h"
#include "wind_core.h"
#include "wind_softint.h"
#include "wind_var.h"
#include "wind_heap.h"
#include "wind_time.h"
#include "core_obj.h"

volatile w_int8_t gwind_int_cnt = 0;//ȫ�ֵ��жϼ���ֵ
volatile w_int8_t gwind_core_cnt = 0;//ȫ�ֵĽ�ֹ�л�����ֵ

extern void wind_thread_switch(void);
extern void wind_interrupt_switch(void);
extern void wind_start_switch(void);


//�������û��߳�
static void wind_thread_open()
{
    g_core.usrthren = B_TRUE;
}

//�鿴�Ƿ��������û��߳�
w_bool_t wind_thread_isopen()
{
    return g_core.usrthren;
}

void wind_disable_switch(void)
{
    gwind_core_cnt ++;
}

void wind_enable_switch(void)
{
    if(gwind_core_cnt > 0)
        gwind_core_cnt --;
}

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

static w_bool_t is_switch_enable(void)
{
    return gwind_core_cnt>0?B_FALSE:B_TRUE;
}

static pthread_s wind_search_highthread(void)
{
    pdnode_s pnode;
    pthread_s pthread = NULL;
    wind_close_interrupt();
    if(gwind_core_cnt > 0)
    {
        pthread = wind_thread_current();
        wind_open_interrupt();
        return pthread;
    }
    foreach_node(pnode,&g_core.threadlist)
    {
        pthread = PRI_DLIST_OBJ(pnode,thread_s,validthr);
        if((pthread->used) && (pthread->runstat == THREAD_STATUS_READY))
        {
            wind_open_interrupt();
            return pthread;
        }
    }
    wind_open_interrupt();
    WIND_ERROR("core NOT find valid thread!");
    return NULL;
}
void wind_exit_int(void)
{
    pthread_s pthr;
    if(RUN_FLAG == B_FALSE)
    {
        WIND_ERROR("exit not rd %d\r\n",RUN_FLAG);
        return;
    }
    
    wind_close_interrupt();
    if(gwind_int_cnt > 0)
        gwind_int_cnt --;
    if((gwind_int_cnt == 0) && is_switch_enable())
    {
        pthr = wind_search_highthread();
        gwind_high_stack = &pthr->stack;
        if(gwind_high_stack != gwind_cur_stack)
        {
            wind_interrupt_switch();
        }
    }
    wind_open_interrupt();
}


//ϵͳ���ȿ�ʼ��������
static void wind_run()
{
    pthread_s pthr;
    pthr = wind_search_highthread();
    gwind_high_stack = &pthr->stack;
    gwind_cur_stack = gwind_high_stack;
    wind_start_switch();
}




//���߳����л��������ȼ����߳�
#if WIND_REALTIME_CORE_SUPPORT
void wind_thread_dispatch(void)
{
    pthread_s pthr;
    if(RUN_FLAG == B_FALSE)
        return;
    wind_close_interrupt();
    if(!is_switch_enable())
    {
        wind_open_interrupt();
        return;
    }
    pthr = wind_search_highthread();
    if(pthr == wind_thread_current())
    {
        wind_open_interrupt();
        return;
    }
    gwind_high_stack = &pthr->stack;
    if(gwind_high_stack != gwind_cur_stack)
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
    g_core.usrthren = B_FALSE;
    wind_target_init();//Ŀ������л�����ʼ��
    wind_std_port_init();//���Զ˿ڳ�ʼ��
    wind_print_os_info();
    
#if WIND_STAT_SUPPORT
    wind_stat_init();
#endif
    wind_corepool_init();
    wind_corevar_init();
#if WIND_HEAP_SUPPORT
    wind_heaps_init();
#endif
    wind_time_init();//ʱ���ʼ��
#if WIND_RTC_SUPPORT
    wind_datetime_init();
#endif
    WIND_INFO("initialization completed!\r\n");
}



//****************************wind_entry***********************************************
int create_init_thread(void);

int wind_os_lunch(void)
{
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


