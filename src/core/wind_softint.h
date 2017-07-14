/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_softint.h / wind_softint.c
**创   建   人: 周江村
**最后修改日期: 
**描        述: 用于实现系统的软中断的功能，软中断主要是提供一种在高优先级运行的任务模式，在该模块执行
**              执行期间将不会被其他的任务所中断，但却可以被硬件中断所打断的该优先级模式
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 2013.10.23未完待续
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef WIND_SOFTINT_H__
#define WIND_SOFTINT_H__

#include "wind_config.h"
#include "wind_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_SOFTINT_SUPPORT > 0
#define WIND_SOFTINT_STK_LEN 512 //软中断线程的堆栈深度
typedef void (*softinthandler_f)(void);

void wind_softint_init(void);
HANDLE wind_softint_reg(softinthandler_f func);
void wind_soft_int(HANDLE handler);
w_err_t wind_create_softint_proc(void);
w_err_t wind_softint_test(void);


#endif
#ifdef __cplusplus
}
#endif


#endif
