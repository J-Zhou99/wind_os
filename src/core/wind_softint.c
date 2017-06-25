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
#include "wind_types.h"
#include "wind_softint.h"
#include "wind_err.h"
#include "wind_string.h"
#include "wind_thread.h"
#include "wind_debug.h"
#include "wind_time.h"
#include "wind_assert.h"

#if WIND_SOFTINT_SUPPORT > 0
extern pthread_s wind_get_cur_proc(void);
extern void wind_thread_dispatch(void);
//���ж��̵߳Ķ�ջ
static stack_t softint_stk[WIND_SOFTINT_STK_LEN];
static u16_t softint_index = 0;
static pthread_s softint_ppcb = NULL;
//static HANDLE softint_handle = -1;
softinthandler_f wind_soft_vectors[WIND_SOFTINT_MAX_NUM];
//��ʼ�����жϵ�һЩ��ز���
void wind_softint_init(void)
{
    wind_memset(softint_stk,0,WIND_SOFTINT_STK_LEN * sizeof(stack_t));
}
static err_t wind_softint_proc(s16_t argc,s8_t **argv)
{
    err_t err = -1;
    s32_t i;
    for(i = 0;i < WIND_SOFTINT_MAX_NUM;i ++)
    {
        wind_soft_vectors[i] = NULL;
    }
    while(1)
    {
        wind_close_interrupt();
        softint_ppcb->proc_status = PROC_STATUS_SLEEP;
        softint_ppcb->cause = CAUSE_COM;
        wind_open_interrupt();
        wind_thread_dispatch();
        if(wind_soft_vectors[softint_index] != NULL)
        {
            (wind_soft_vectors[softint_index])();
        }
        
    }
    return ERR_OK;
}

//�����ж�ģ��ע��һ���ж�������Ӧ����
HANDLE wind_softint_reg(softinthandler_f func)
{
    HANDLE hint = -1;
    s16_t i;
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
err_t wind_softint_unreg(HANDLE hint)
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
    softint_ppcb->proc_status = PROC_STATUS_READY;
    wind_open_interrupt();
    wind_thread_dispatch();
}

//��������ж��߳�
err_t wind_create_softint_proc(void)
{
    softint_ppcb = wind_thread_create("softint",PRIO_HIGH,wind_softint_proc,
                0,NULL,softint_stk,WIND_SOFTINT_STK_LEN);
                wind_thread_changeprio(softint_ppcb,0);
    //WIND_ERROR("software OK\r\n");
    return ERR_OK;
}


//-----------------------����жϲ���---------------------------------------------
void softint_output(void)
{
    WIND_STD_OUT("wind softint test---OK\r\n");
}

err_t wind_softint_test(void)
{
    HANDLE h;
    h = wind_softint_reg(softint_output);
    wind_soft_int(h);
    //wind_softint_unreg(h);
    return ERR_OK;
}
//-----------------------����жϲ���---------------------------------------------
#endif

