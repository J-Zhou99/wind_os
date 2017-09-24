/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: console_framework.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: ϵͳ�Ŀ���̨���
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2013.06.26
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2012.06.26
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/

#ifndef WIND_CONSOLE_H__
#define WIND_CONSOLE_H__
#include "wind_config.h"
#include "wind_type.h"
#include "wind_debug.h"
#include "console_framework.h"
#ifdef __cplusplus
extern "C" {
#endif

#define console_printf(fmt,...) wind_printf(fmt,##__VA_ARGS__)
#define CMD_DECLARE(CMD) extern cmd_s g_cmd_##CMD
#define COMMAND(CMD) &g_cmd_##CMD
#define CMD_DEF(CMD) \
cmd_s g_cmd_##CMD = { \
NULL,#CMD,cmd_showdisc,\
cmd_showusage,cmd_main}



#endif//ifndef WIND_CONSOLE_H__
#ifdef __cplusplus
}
#endif
