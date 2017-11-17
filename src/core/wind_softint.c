/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_softint.h / wind_softint.c
**��   ��   ��: �ܽ���
**����޸�����: 
**��        ��: ����ʵ��ϵͳ�����жϵĹ��ܣ����ж���Ҫ���ṩһ���ڸ����ȼ����е�����ģʽ���������������
**              �̵߳�������ȼ�����ִ������ʱ�������Լ����������򲻻ᱻ�κ��������̴߳���ڸ�ģ��ִ��
**              ִ���ڼ佫���ᱻ�������������жϣ���ȴ���Ա�Ӳ���ж�����ϵĸ����ȼ�ģʽ
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 2013.10.23δ�����
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "wind_config.h"
#include "wind_type.h"
#include "wind_softint.h"
#include "wind_err.h"
#include "wind_string.h"
#include "wind_thread.h"
#include "wind_debug.h"
#include "wind_time.h"
#include "wind_assert.h"
#include "wind_core.h"
#if WIND_SOFTINT_SUPPORT

//���ж��̵߳Ķ�ջ
static w_stack_t softint_stk[WIND_SOFTINT_STK_LEN];
static w_uint16_t softint_index = 0;
static pthread_s softint_thread = NULL;
//static HANDLE softint_handle = -1;
softint_func wind_soft_vectors[WIND_SOFTINT_MAX_NUM];
//��ʼ�����жϵ�һЩ��ز���
void wind_softint_init(void)
{
    wind_memset(softint_stk,0,WIND_SOFTINT_STK_LEN * sizeof(w_stack_t));
}

static w_err_t wind_softint_thread(w_int32_t argc,w_int8_t **argv)
{
    w_int32_t i;
    for(i = 0;i < WIND_SOFTINT_MAX_NUM;i ++)
    {
        wind_soft_vectors[i] = NULL;
    }
    while(1)
    {
        wind_close_interrupt();
        softint_thread->runstat = THREAD_STATUS_SUSPEND;
        softint_thread->cause = CAUSE_COM;
        wind_open_interrupt();
        wind_thread_dispatch();
        if(wind_soft_vectors[softint_index] != NULL)
        {
            (wind_soft_vectors[softint_index])();
        }
        
    }
    //return ERR_OK;
}

//�����ж�ģ��ע��һ���ж�������Ӧ����
HANDLE wind_softint_reg(softint_func func)
{
    HANDLE hint = -1;
    w_int16_t i;
    for(i = 0;i < WIND_SOFTINT_MAX_NUM;i ++)
    {
        if(wind_soft_vectors[i] == func)
        {
            return i;
        }
    }
    for(i = 0;i < WIND_SOFTINT_MAX_NUM;i ++)
    {
        if(wind_soft_vectors[i] == NULL)
        {
            hint = i;
            wind_soft_vectors[i] = func;
            break;
        }
    }
    return hint;
}

//ȡ��һ�����жϵ�ע��
w_err_t wind_softint_unreg(HANDLE hint)
{
    if(hint < 0 || hint >= WIND_SOFTINT_MAX_NUM)
        return ERR_PARAM_OVERFLOW;
    wind_soft_vectors[hint] = NULL;
    return ERR_OK;
}

//����һ������ж�
void wind_soft_int(HANDLE handler)
{
    wind_close_interrupt();
    softint_index = handler;
    softint_thread->runstat = THREAD_STATUS_READY;
    wind_open_interrupt();
    wind_thread_dispatch();
}

//��������ж��߳�
w_err_t wind_create_softint_thread(void)
{
    WIND_INFO("create soft interrupt thread.\r\n");
    softint_thread = wind_thread_create("softint",PRIO_HIGH,wind_softint_thread,
                0,NULL,softint_stk,WIND_SOFTINT_STK_LEN);
                wind_thread_changeprio(softint_thread,0);
    return ERR_OK;
}


//-----------------------����жϲ���---------------------------------------------
void softint_output(void)
{
    wind_printf("wind softint test---OK\r\n");
}

w_err_t wind_softint_test(void)
{
    HANDLE h;
    h = wind_softint_reg(softint_output);
    wind_soft_int(h);
    return ERR_OK;
}
//-----------------------����жϲ���---------------------------------------------
#endif

