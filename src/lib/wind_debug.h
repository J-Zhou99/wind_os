/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_debug.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: ������Ϣ�������ʽͷ�ļ�
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

#ifndef WIND_DEBUG_H__
#define WIND_DEBUG_H__

#include "wind_config.h"
#include "wind_os_hwif.h"

#ifdef __cplusplus
extern "C" {
#endif
#define WIND_ERROR_OUT_EN 1 //�������������Ϣ
#define WIND_INFO_OUT_EN  1 //���������ͨ����Ϣ��Ϣ
#define WIND_WARN_OUT_EN  1 //�������������Ϣ
#define WIND_DEBUG_OUT_EN 0 //���������Ϣ���


//void wind_printf(const w_int8_t *string,...);
int wind_printf(const char *fmt, ...);
//WIND_DEBUG
#if WIND_DEBUG_OUT_EN
#define WIND_DEBUG(fmt,...) do{wind_printf("debug:"fmt" [%s,%d]\r\n",##__VA_ARGS__,__FILE__,__LINE__);wind_printf("[%s,%d]\r\n",__FILE__,__LINE__);}while(0)
#else 
#define WIND_DEBUG(fmt,...)
#endif

//WIND_INFO
#if WIND_INFO_OUT_EN
#define WIND_INFO(...) do{wind_printf(__VA_ARGS__);}while(0)
#else 
#define WIND_INFO(...) 
#endif

//WIND_WARN
#if WIND_WARN_OUT_EN
#define WIND_WARN(...) do{wind_printf(__VA_ARGS__);wind_printf("[%s,%d]\r\n",__FILE__,__LINE__);}while(0)
#else 
#define WIND_WARN(...)
#endif

//WIND_ERROR
#if WIND_ERROR_OUT_EN
#define WIND_ERROR(...) do{wind_printf(__VA_ARGS__);wind_printf("[%s,%d]\r\n",__FILE__,__LINE__);}while(0)
#else 
#define WIND_ERROR(...)
#endif


#ifdef __cplusplus
}
#endif

#endif  //WIND_DEBUG_H__

