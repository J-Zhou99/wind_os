/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_ticktimer.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os����tickΪ���ȵĴ���ʱ���Ĵ���
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
#ifndef WIND_TIMER_H__
#define WIND_TIMER_H__

#include "wind_config.h"
#include "wind_type.h"
#include "wind_os_hwif.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_TTIMER_SUPPORT > 0
typedef void (*softtimer_fn)(void *arg);
typedef struct tagTicktimer
{
    //HANDLE handler;
    w_bool_t used;
    w_bool_t running;
    w_uint32_t count;
    w_uint32_t init_count;
    softtimer_fn handle;
    void *arg;
}ttimer_s,*ptimer_s;

w_err_t wind_timer_init(void);
ptimer_s wind_timer_create(w_uint32_t t_ms,softtimer_fn func,void *arg,w_bool_t run);
w_err_t wind_timer_start(ptimer_s pttimer);
w_err_t wind_timer_stop(ptimer_s pttimer);
w_err_t wind_timer_free(ptimer_s pttimer);
w_err_t wind_timer_setticks(ptimer_s pttimer,w_uint32_t ticks);
extern void wind_timer_handler(void);//��ʱ�����


#endif //#if WIND_TTIMER_SUPPORT > 0
#ifdef __cplusplus
}
#endif

#endif //#define WIND_TICKTIMER_H__

