/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_os_hwif.c
**创   建   人: 周江村
**最后修改日期: 2012.09.26
**描        述: wind os的CPU体系相关的代码
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
#include "wind_os_hwif.h"
#include "wind_debug.h"
#include "misc.h"
#include "usart1.h"
#include "core_cm4.h"
void wind_target_init(void)
{

}

void wind_system_reset(void)
{
    NVIC_SystemReset();
}


//SREG，CPU状态寄存器对应的数据位宽，当关闭中断时需要保存这个寄存器
typedef unsigned int sreg_t;
extern sreg_t  wind_save_sr(void);
extern void   wind_restore_sr(sreg_t cpu_sr);
static sreg_t ssr[100];
static w_int32_t sreg_idx = 0;
void wind_close_interrupt(void)
{
    sreg_t cpu_sr;
    cpu_sr = wind_save_sr();
    ssr[sreg_idx++] = cpu_sr;
}

#if WIND_HEAP_SUPPORT
#include "wind_heap.h"
#define HEAP1_HEAD  0X10000000
#define HEAD1_LENTH (64*1024)

//堆可自由分配的内存空间进行初始化
void wind_heaps_init(void)
{
    wind_heap_init("heap0",
                (void *)HEAP1_HEAD,
                HEAD1_LENTH);
    //wind_heap_showinfo();
}

#endif

void wind_open_interrupt(void)
{
    sreg_t cpu_sr;
    if(sreg_idx > 0)
        sreg_idx --;
    cpu_sr = ssr[sreg_idx];
    wind_restore_sr(cpu_sr);
}


w_pstack_t wind_stk_init(thread_run_f pfunc,void *pdata, w_pstack_t pstkbt)
{
    w_pstack_t stk;
    stk = pstkbt;                            /* Load stack pointer                                 */

#if (__FPU_PRESENT==1)&&(__FPU_USED==1)	
    *(--stk) = (w_uint32_t)0x00000000L; //No Name Register  
    *(--stk) = (w_uint32_t)0x00001000L; //FPSCR
    *(--stk) = (w_uint32_t)0x00000015L; //s15
    *(--stk) = (w_uint32_t)0x00000014L; //s14
    *(--stk) = (w_uint32_t)0x00000013L; //s13
    *(--stk) = (w_uint32_t)0x00000012L; //s12
    *(--stk) = (w_uint32_t)0x00000011L; //s11
    *(--stk) = (w_uint32_t)0x00000010L; //s10
    *(--stk) = (w_uint32_t)0x00000009L; //s9
    *(--stk) = (w_uint32_t)0x00000008L; //s8
    *(--stk) = (w_uint32_t)0x00000007L; //s7
    *(--stk) = (w_uint32_t)0x00000006L; //s6
    *(--stk) = (w_uint32_t)0x00000005L; //s5
    *(--stk) = (w_uint32_t)0x00000004L; //s4
    *(--stk) = (w_uint32_t)0x00000003L; //s3
    *(--stk) = (w_uint32_t)0x00000002L; //s2
    *(--stk) = (w_uint32_t)0x00000001L; //s1
    *(--stk) = (w_uint32_t)0x00000000L; //s0
#endif
                                         /* Registers stacked as if auto-saved on exception    */
    *(stk)    = (w_uint32_t)0x01000000L;             /* xPSR                                               */
    *(--stk)  = (w_uint32_t)pfunc;                    /* Entry Point                                        */
    *(--stk)  = (w_uint32_t)NULL;//OS_TaskReturn;           /* R14 (LR) (init value will cause fault if ever used)*/
    *(--stk)  = (w_uint32_t)0x12121212L;             /* R12                                                */
    *(--stk)  = (w_uint32_t)0x03030303L;             /* R3                                                 */
    *(--stk)  = (w_uint32_t)0x02020202L;             /* R2                                                 */
    *(--stk)  = (w_uint32_t)0x01010101L;             /* R1                                                 */
    *(--stk)  = (w_uint32_t)pdata;                   /* R0 : argument                                      */

#if (__FPU_PRESENT==1)&&(__FPU_USED==1)	
    *(--stk) = (w_uint32_t)0x00000031L; //s31
    *(--stk) = (w_uint32_t)0x00000030L; //s30
    *(--stk) = (w_uint32_t)0x00000029L; //s29
    *(--stk) = (w_uint32_t)0x00000028L; //s28
    *(--stk) = (w_uint32_t)0x00000027L; //s27
    *(--stk) = (w_uint32_t)0x00000026L; //s26	
    *(--stk) = (w_uint32_t)0x00000025L; //s25
    *(--stk) = (w_uint32_t)0x00000024L; //s24
    *(--stk) = (w_uint32_t)0x00000023L; //s23
    *(--stk) = (w_uint32_t)0x00000022L; //s22
    *(--stk) = (w_uint32_t)0x00000021L; //s21
    *(--stk) = (w_uint32_t)0x00000020L; //s20
    *(--stk) = (w_uint32_t)0x00000019L; //s19
    *(--stk) = (w_uint32_t)0x00000018L; //s18
    *(--stk) = (w_uint32_t)0x00000017L; //s17
    *(--stk) = (w_uint32_t)0x00000016L; //s16
#endif

                                        /* Remaining registers saved on process stack         */
    *(--stk)  = (w_uint32_t)0x11111111L;             /* R11                                                */
    *(--stk)  = (w_uint32_t)0x10101010L;             /* R10                                                */
    *(--stk)  = (w_uint32_t)0x09090909L;             /* R9                                                 */
    *(--stk)  = (w_uint32_t)0x08080808L;             /* R8                                                 */
    *(--stk)  = (w_uint32_t)0x07070707L;             /* R7                                                 */
    *(--stk)  = (w_uint32_t)0x06060606L;             /* R6                                                 */
    *(--stk)  = (w_uint32_t)0x05050505L;             /* R5                                                 */
    *(--stk)  = (w_uint32_t)0x04040404L;             /* R4                                                 */

    return (stk);
}

void wind_tick_init(void)
{
	w_uint32_t reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//fac_us=SYSCLK/8;		//不论是否使用ucos,fac_us都需要使用

	reload=SYSCLK/8;		//每秒钟的计数次数 单位为K	   
	reload*=1000000/WIND_TICK_PER_SEC;//根据OS_TICKS_PER_SEC设定溢出时间
	//reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右	
	//fac_ms=1000/WIND_TICK_PER_SEC;//代表ucos可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK

}

