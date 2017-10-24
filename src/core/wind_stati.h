/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_stat.h
**��   ��   ��: �ܽ���
**����޸�����: wind os����Դͳ����صĴ���
**��        ��: 
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2013.08.13
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef WIND_STAT_H__
#define WIND_STAT_H__
#include "wind_type.h"
#include "wind_config.h"


#ifdef __cplusplus
extern "C" {
#endif

#if WIND_STAT_SUPPORT > 0
typedef enum __stat_e
{
    IDX_THREAD,
#if WIND_LOCK_SUPPORT > 0
    IDX_LOCK,
#endif
#if WIND_SEM_SUPPORT > 0
    IDX_SEM,
#endif
#if WIND_PIPE_SUPPORT > 0
    IDX_PIPE,
#endif
#if WIND_MESSAGE_SUPPORT > 0
    IDX_MSG,
    IDX_MBOX,
#endif
#if WIND_TIMER_SUPPORT > 0
    IDX_TIMER,
#endif
    IDX_STACK,
    IDX_CNT
}stat_e;

#define STAT_NAME_LEN 12
typedef struct __stat_s
{
    w_int8_t name[STAT_NAME_LEN];
    w_uint32_t tot;
    w_uint32_t used;
    w_uint32_t max;
    w_uint32_t err;
}stat_s,*pstat_s;

extern stat_s g_stati[IDX_CNT];
#define WIND_STAT_INC(stat_type) g_stati[stat_type].used++
#define WIND_STAT_MINUS(stat_type) g_stati[stat_type].used--

#define WIND_STAT_MAX(stat_type) \
        do{if(g_stati[stat_type].used > g_stati[stat_type].max) \
        g_stati[stat_type].max = g_stati[stat_type].used;}while(0)


void wind_stat_init(void);
#else
#define wind_stat_init()
#define WIND_STAT_INC(stat_type)
#define WIND_STAT_MINUS(stat_type)
#define WIND_STAT_MAX(stat_type)
#endif

#ifdef __cplusplus
}
#endif


#endif

