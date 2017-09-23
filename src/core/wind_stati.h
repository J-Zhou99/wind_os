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
    STAT_PROC,
    STAT_NODE,
#if WIND_LOCK_SUPPORT > 0
    STAT_LOCK,
#endif
#if WIND_SEM_SUPPORT > 0
    STAT_SEM,
#endif
#if WIND_PIPE_SUPPORT > 0
    STAT_PIPE,
#endif
#if WIND_MESSAGE_SUPPORT > 0
    STAT_MESSAGE,
    STAT_MBOX,
#endif
#if WIND_TIMER_SUPPORT > 0
    STAT_TIMER,
#endif
    STAT_CNT
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

extern stat_s G_STAT[STAT_CNT];
#define WIND_STAT_INC(stat_type) G_STAT[stat_type].used++
#define WIND_STAT_MINUS(stat_type) G_STAT[stat_type].used--

#define WIND_STAT_MAX(stat_type) \
        do{if(G_STAT[stat_type].used > G_STAT[stat_type].max) \
        G_STAT[stat_type].max = G_STAT[stat_type].used;}while(0)


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

