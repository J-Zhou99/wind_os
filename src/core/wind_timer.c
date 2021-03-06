/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_timer.h
**创   建   人: 周江村
**最后修改日期: 2012.09.26
**描        述: wind os的以tick为精度的代定时器的代码
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 2012.09.26
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 周江村
** 日　期: 2012.10.20
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "wind_config.h"
#include "wind_type.h"
#include "wind_timer.h"
#include "wind_debug.h"
#include "wind_stati.h"
#include "wind_var.h"
#include "core_obj.h"
#if WIND_TIMER_SUPPORT
static timer_s* ttimer_malloc(void)
{
    timer_s* timer;
    timer = wind_core_alloc(IDX_TIMER);
    if(timer == NULL)
    {
        return NULL;
    }
    timer->used = B_TRUE;
    timer->running = B_FALSE;
    return timer;
}
w_err_t ttimer_free(timer_s* timer)
{
    if(timer == NULL)
        return ERR_NULL_POINTER;
    timer->running = B_FALSE;
    timer->used = B_FALSE;
    timer->count = 0;
    timer->init_count = 0;
    timer->handle = NULL;
    wind_core_free(IDX_TIMER,timer);
    return ERR_OK;
}


timer_s* wind_timer_create(w_uint32_t t_ms,softtimer_fn func,void *arg,w_bool_t run)
{
    timer_s* timer;
    dnode_s *pnode;
    w_int32_t count = t_ms / TIMER_PERIOD;
    if(count <= 0)
        count = 1;
    wind_notice("creating soft timer:%d ms",t_ms);
    WIND_ASSERT_RETURN(func != NULL,NULL);
    timer = ttimer_malloc();
    WIND_ASSERT_RETURN(timer != NULL,NULL);
    wind_notice("creat soft timer OK");
    DNODE_INIT(timer->tmrnode);

    timer->count = count;
    timer->init_count = count;
    timer->running = run;
    timer->arg = arg;
    timer->handle = func;
    dlist_insert_tail(&g_core.ttmerlist,pnode);
    return timer; 
}

w_err_t wind_timer_start(timer_s* ptimer)
{
    WIND_ASSERT_RETURN(ptimer != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ptimer->used != B_FALSE,ERR_COMMAN);
    ptimer->running = B_TRUE;
    return ERR_OK;
}

w_err_t wind_timer_stop(timer_s* ptimer)
{
    WIND_ASSERT_RETURN(ptimer != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ptimer->used != B_FALSE,ERR_COMMAN);
    ptimer->running = B_FALSE;
    return ERR_OK;
}

w_err_t wind_timer_free(timer_s* ptimer)
{
    WIND_ASSERT_RETURN(ptimer != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ptimer->used != B_FALSE,ERR_COMMAN);
    wind_close_interrupt();
    dlist_remove(&g_core.ttmerlist,&ptimer->tmrnode);
    ttimer_free(ptimer);
    wind_open_interrupt();
    return ERR_OK;
}

w_err_t wind_timer_set_period(timer_s* ptimer,w_uint32_t t_ms)
{
    w_int32_t count = t_ms / TIMER_PERIOD;
    if(count <= 0)
        count = 1;
    WIND_ASSERT_RETURN(ptimer != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ptimer->used != B_FALSE,ERR_COMMAN);
    ptimer->count = count;
    return ERR_OK;
}

void wind_timer_event(void)
{
    timer_s* ptmr;
    dnode_s *pdnode;
    foreach_node(pdnode,&g_core.ttmerlist)
    {
        ptmr = DLIST_OBJ(pdnode,timer_s,tmrnode);
        if(ptmr->count > 0)
            ptmr->count --;
        if(ptmr->count == 0 && ptmr->running)
        {
            ptmr->handle(ptmr->arg);
            ptmr->count = ptmr->init_count;
        }
    }
}

#endif //#if WIND_TIMER_SUPPORT


