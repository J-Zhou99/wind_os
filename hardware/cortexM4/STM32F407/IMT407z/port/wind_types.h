/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_types.h
**创   建   人: 周江村
**最后修改日期: 2012.09.26
**描        述: wind os的类型定义
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
#ifndef WIND_TYPES_H__
#define WIND_TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifndef w_uint8_t
typedef unsigned char w_uint8_t;
#endif

#ifndef w_uint16_t
typedef unsigned int w_uint16_t;
#endif

#ifndef w_uint32_t
typedef unsigned long w_uint32_t; 
#endif

#ifndef w_uint64_t
typedef unsigned long long w_uint64_t; 
#endif

#ifndef w_int8_t
typedef char w_int8_t; 
#endif

#ifndef w_int16_t
typedef int w_int16_t; 
#endif

#ifndef w_int32_t
typedef long w_int32_t; 
#endif

#ifndef w_int64_t
typedef long long w_int64_t; 
#endif


#ifndef w_fp32_t
typedef float w_fp32_t;
#endif

#ifndef w_fp64_t
typedef float w_fp64_t;
#endif


#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef w_bool_t
typedef enum __w_bool_t
{
    B_FALSE = 0,
    B_TRUE = 1
} w_bool_t;
#endif


#ifndef w_err_t
typedef w_int16_t w_err_t;
#endif


#ifndef w_stack_t  //堆栈宽度的定义
typedef w_uint32_t w_stack_t;//,*pstack_t;
typedef w_uint32_t *pstack_t;
#endif


#ifndef HANDLE 
typedef w_int16_t HANDLE;
#endif

typedef enum __lock_type_e
{
    LOCK_TYPE_NONE = 0,
    LOCK_TYPE_AREA = 1,
    LOCK_TYPE_GLOBAL = 2,
    LOCK_TYPE_BUTT
}lock_type_e;






#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  

