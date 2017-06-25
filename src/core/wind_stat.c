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
#include "wind_stat.h"
#include "wind_string.h"
#include "wind_debug.h"

#if WIND_STAT_SUPPORT > 0

stat_s G_STAT[STAT_CNT];
void wind_stat_init(void)
{
    WIND_INFO("statistics initializing...\r\n");
    wind_memset(G_STAT,0,sizeof(G_STAT));
    G_STAT[STAT_PROC].tot = WIND_THREAD_MAX_NUM;
    wind_memcpy(G_STAT[STAT_PROC].name,"proc",4);
    G_STAT[STAT_NODE].tot = WIND_NODE_MAX_NUM;
    wind_memcpy(G_STAT[STAT_NODE].name,"node",4);
#if WIND_LOCK_SUPPORT > 0
    G_STAT[STAT_LOCK].tot = WIND_LOCK_NUM;
    wind_memcpy(G_STAT[STAT_LOCK].name,"lock",4);
#endif
#if WIND_SEM_SUPPORT > 0
    G_STAT[STAT_SEM].tot = WIND_SEM_MAX_NUM;
    wind_memcpy(G_STAT[STAT_SEM].name,"sem",3);
#endif
#if WIND_PIPE_SUPPORT > 0
    G_STAT[STAT_PIPE].tot = WIND_PIPE_MAX_NUM;
    wind_memcpy(G_STAT[STAT_PIPE].name,"pipe",4);
#endif
#if WIND_MESSAGE_SUPPORT > 0
    G_STAT[STAT_MESSAGE].tot = WIND_MESSAGE_MAX_NUM;
    wind_memcpy(G_STAT[STAT_MESSAGE].name,"message",7);
    G_STAT[STAT_MBOX].tot = WIND_MBOX_MAX_NUM;
    wind_memcpy(G_STAT[STAT_MBOX].name,"mbox",4);
#endif
#if WIND_TTIMER_SUPPORT > 0
    G_STAT[STAT_TTIMER].tot = WIND_TTIMER_MAX_NUM;
    wind_memcpy(G_STAT[STAT_TTIMER].name,"ttimer",6);
#endif
}

#endif
