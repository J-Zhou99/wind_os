/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_stat.c
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
#include "wind_config.h"
#include "wind_stati.h"
#include "wind_string.h"
#include "wind_debug.h"

#if WIND_STAT_SUPPORT > 0

stat_s g_stati[IDX_CNT];
void wind_stat_init(void)
{
    WIND_INFO("statistics initializing...\r\n");
    wind_memset(g_stati,0,sizeof(g_stati));
    g_stati[IDX_THREAD].tot = WIND_THREAD_MAX_NUM;
    wind_memcpy(g_stati[IDX_THREAD].name,"thread",4);
#if WIND_LOCK_SUPPORT > 0
    g_stati[IDX_LOCK].tot = WIND_LOCK_NUM;
    wind_memcpy(g_stati[IDX_LOCK].name,"lock",4);
#endif
#if WIND_SEM_SUPPORT > 0
    g_stati[IDX_SEM].tot = WIND_SEM_MAX_NUM;
    wind_memcpy(g_stati[IDX_SEM].name,"sem",3);
#endif
#if WIND_PIPE_SUPPORT > 0
    g_stati[IDX_PIPE].tot = WIND_PIPE_MAX_NUM;
    wind_memcpy(g_stati[IDX_PIPE].name,"pipe",4);
#endif
#if WIND_MESSAGE_SUPPORT > 0
    g_stati[IDX_MSG].tot = WIND_MESSAGE_MAX_NUM;
    wind_memcpy(g_stati[IDX_MSG].name,"message",7);
    g_stati[IDX_MBOX].tot = WIND_MBOX_MAX_NUM;
    wind_memcpy(g_stati[IDX_MBOX].name,"mbox",4);
#endif
#if WIND_TIMER_SUPPORT > 0
    g_stati[IDX_TIMER].tot = WIND_TTIMER_MAX_NUM;
    wind_memcpy(g_stati[IDX_TIMER].name,"timer",6);
#endif
    
    g_stati[IDX_STACK].tot = WIND_STK_MAX_NUM;
    wind_memcpy(g_stati[IDX_STACK].name,"stack",6);
}

#endif
