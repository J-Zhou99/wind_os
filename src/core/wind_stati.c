/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_stat.c
**创   建   人: 周江村
**最后修改日期: wind os的资源统计相关的代码
**描        述: 
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 2013.08.13
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "wind_config.h"
#include "wind_stati.h"
#include "wind_string.h"
#include "wind_debug.h"

#if WIND_STAT_SUPPORT

stat_s g_stati[IDX_CNT];
void wind_stat_init(void)
{
    wind_notice("statistics initializing...\r\n");
    wind_memset(g_stati,0,sizeof(g_stati));
    g_stati[IDX_THREAD].tot = WIND_THREAD_MAX_NUM;
    wind_memcpy(g_stati[IDX_THREAD].name,"thread",4);
    g_stati[IDX_LOCK].tot = WIND_LOCK_NUM;
    wind_memcpy(g_stati[IDX_LOCK].name,"lock",4);
#if WIND_SEM_SUPPORT
    g_stati[IDX_SEM].tot = WIND_SEM_MAX_NUM;
    wind_memcpy(g_stati[IDX_SEM].name,"sem",3);
#endif
#if WIND_PIPE_SUPPORT
    g_stati[IDX_PIPE].tot = WIND_PIPE_MAX_NUM;
    wind_memcpy(g_stati[IDX_PIPE].name,"pipe",4);
#endif
#if WIND_MESSAGE_SUPPORT
    g_stati[IDX_MSG].tot = WIND_MESSAGE_MAX_NUM;
    wind_memcpy(g_stati[IDX_MSG].name,"message",7);
    g_stati[IDX_MBOX].tot = WIND_MBOX_MAX_NUM;
    wind_memcpy(g_stati[IDX_MBOX].name,"mbox",4);
#endif
#if WIND_TIMER_SUPPORT
    g_stati[IDX_TIMER].tot = WIND_TIMER_MAX_NUM;
    wind_memcpy(g_stati[IDX_TIMER].name,"timer",6);
#endif
    
    g_stati[IDX_STACK].tot = WIND_STK_MAX_NUM;
    wind_memcpy(g_stati[IDX_STACK].name,"stack",6);
}

#endif
