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
#ifndef WIND_TICKTIMER_H__
#define WIND_TICKTIMER_H__

#include "wind_config.h"
#include "wind_type.h"
#include "wind_os_hwif.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_TTIMER_SUPPORT > 0

typedef struct tagTicktimer
{
    //HANDLE handler;
    w_bool_t used;
    w_bool_t running;
    w_uint32_t count;
    w_uint32_t inittick;
    void (*timercallback)(void);
}ttimer_s,*pttimer_s;

w_err_t wind_ttimer_init(void);
pttimer_s wind_ttimer_create(w_uint32_t ticks,void (*timercallback)(void),w_bool_t run);
w_err_t wind_ttimer_start(pttimer_s pttimer);
w_err_t wind_ttimer_stop(pttimer_s pttimer);
w_err_t wind_ttimer_free(pttimer_s pttimer);
w_err_t wind_ttimer_setticks(pttimer_s pttimer,w_uint32_t ticks);
extern void ticktimerhandler(void);//��ʱ�����


#endif //#if WIND_TTIMER_SUPPORT > 0
#ifdef __cplusplus
}
#endif

#endif //#define WIND_TICKTIMER_H__

