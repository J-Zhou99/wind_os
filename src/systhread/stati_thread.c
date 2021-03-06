#include "wind_type.h"
#include "wind_var.h"
#include "wind_thread.h"


#define STATI_STK_SIZE 256
static w_stack_t statisstk[STATI_STK_SIZE];
static w_err_t stati_thread(w_int32_t argc,w_int8_t **argv)
{
    
    w_uint32_t statcnt = 0;
    w_uint32_t stati_ms = 1000;
    while(1)
    {
        statcnt = g_core.idle_cnt;
        wind_thread_sleep(stati_ms);
        statcnt = g_core.idle_cnt - statcnt;
        WIND_CPU_USAGE = (IDLE_CNT_PER_SEC - statcnt) *100 / IDLE_CNT_PER_SEC;
        if(WIND_CPU_USAGE > 100)
            WIND_CPU_USAGE = 0;
    }

}

void create_stati_thread(void)
{
    g_core.pstat = wind_thread_create("statistics",PRIO_HIGH,stati_thread,
                     0,NULL,statisstk,STATI_STK_SIZE);
}

