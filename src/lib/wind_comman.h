/****************************************Copyright (c)**************************************************
**                                       清  风  海  岸
**
**                                       yzfcer@163.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: wind_comman.h / wind_comman.c
**创   建   人: 周江村
**最后修改日期: 2012.09.26
**描        述: wind os的一些通用的函数
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 周江村
** 版  本: v1.0
** 日　期: 2013.10.19
** 描　述: 原始版本
**
**--------------当前版本修订----------------------------------------------------------------------------
** 修改人: 周江村
** 日　期: 2013.10.20
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef WIND_COMMAN_H__
#define WIND_COMMAN_H__
#ifdef __cplusplus
extern "C" {
#endif
w_int32_t wind_convert_str2s32_t(w_int8_t *str);

w_uint32_t wind_convert_str2u32_t(w_int8_t *str);




#ifdef __cplusplus
}
#endif
#endif//#ifndef WIND_COMMAN_H__
