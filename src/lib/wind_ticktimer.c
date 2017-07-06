/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_ticktimer.h
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

#include "wind_config.h"
#include "wind_types.h"
#include "wind_config.h"
#include "wind_ticktimer.h"
#include "wind_list.h"
#include "wind_debug.h"
#include "wind_err.h"
#include "wind_stat.h"
#include "wind_var.h"
#if WIND_TTIMER_SUPPORT > 0
static pttimer_s ttimer_malloc(void)
{
    pttimer_s pttimer;
    pttimer = wind_core_alloc(STAT_TTIMER);
    if(pttimer == NULL)
    {
        return NULL;
    }
    pttimer->used = B_TRUE;
    pttimer->running = B_FALSE;
    return pttimer;
}
err_t ttimer_free(pttimer_s ttimer)
{
    if(ttimer == NULL)
        return ERR_NULL_POINTER;
    wind_close_interrupt();
    
    ttimer->running = B_FALSE;
    ttimer->used = B_FALSE;
    ttimer->count = 0;
    ttimer->inittick = 0;
    ttimer->timercallback = NULL;
    wind_open_interrupt();
    return ERR_OK;
}


pttimer_s wind_ttimer_create(u32_t ticks,void (*timercallback)(void),bool_t run)
{
    pttimer_s ttimer;
    pnode_s pnode;
    WIND_INFO("creating tick timer:%d ms\r\n",ticks);
    if(timercallback == NULL)
    {
        WIND_ERROR("wind_ttimer_create err 1\r\n");
        return NULL;
    }
    wind_close_interrupt();
    ttimer = ttimer_malloc();
    if(!ttimer)
    {
        wind_open_interrupt();
        WIND_ERROR("wind_ttimer_create err 2\r\n");
        return NULL;
    }
    pnode = wind_node_malloc(CORE_TYPE_TTIMER);
    if(!pnode)
    {
        wind_ttimer_free(ttimer);
        wind_open_interrupt();
        WIND_ERROR("wind_ttimer_create err 3\r\n");
        return NULL;
    }
    wind_open_interrupt();
    WIND_INFO("wind_ttimer_create OK\r\n");

    ttimer->count = ticks;
    ttimer->inittick = ticks;
    ttimer->running = run;
    ttimer->timercallback = timercallback;
    wind_node_bindobj(pnode,CORE_TYPE_TTIMER,ticks,ttimer);
    wind_list_insert(&g_core.ttmerlist,pnode);
    return ttimer; 
}

err_t wind_ttimer_start(pttimer_s pttimer)
{
    if(pttimer == NULL)
        return ERR_NULL_POINTER;
    //pttmr = ttimer_search(htimer);
    if(!pttimer->used)
        return ERR_COMMAN;
    pttimer->running = B_TRUE;
    return ERR_OK;
}

err_t wind_ttimer_stop(pttimer_s pttimer)
{
    if(pttimer == NULL)
        return ERR_NULL_POINTER;
    if(!pttimer->used)
        return ERR_COMMAN;
    pttimer->running = B_FALSE;
    return ERR_OK;
}

err_t wind_ttimer_free(pttimer_s pttimer)
{
    pnode_s pnode;
    if(pttimer == NULL)
        return ERR_NULL_POINTER;
    if(!pttimer->used)
        return ERR_COMMAN;
    wind_close_interrupt();    
    pnode = wind_list_search(&g_core.ttmerlist,pttimer);
    if(pnode == NULL)
    {
        wind_open_interrupt();    
        return ERR_INVALID_PARAM;
    }    
    wind_list_remove(&g_core.ttmerlist,pnode);
    wind_node_free(pnode);
    ttimer_free(pttimer);
    wind_open_interrupt();
    return ERR_OK;
}

err_t wind_ttimer_setticks(pttimer_s pttimer,u32_t ticks)
{
    if(pttimer == NULL)
        return ERR_NULL_POINTER;
    if(!pttimer->used)
        return ERR_COMMAN;
    pttimer->count = ticks;
    return ERR_OK;
}

extern void ticktimerhandler(void)
{
    pttimer_s pttmr;
    pnode_s tmpttimerlist = g_core.ttmerlist.head;
    while(tmpttimerlist)
    {
        //WIND_DEBUG("ticktimerhandler\r\n");
        pttmr = (pttimer_s)(tmpttimerlist->obj);
        //WIND_DEBUG("pttmr = %d,%d\r\n",pttmr,pttmr->tick);    
        if(pttmr->count > 0)
            pttmr->count --;
        if(pttmr->count == 0 && pttmr->running)
        {
            pttmr->timercallback();
            
            pttmr->count = pttmr->inittick;
        }
        tmpttimerlist = tmpttimerlist->next;
    }
    //WIND_DEBUG("timerhandler tick=%d\r\n",pttmr->tick);
}
#endif //#if WIND_TTIMER_SUPPORT > 0




