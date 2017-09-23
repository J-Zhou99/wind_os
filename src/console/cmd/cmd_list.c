/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_echo.c
**��   ��   ��: �ܽ���
**����޸�����: 2013.10.19
**��        ��: ϵͳ�Ŀ���̨����show�����������һЩϵͳ�ڲ���Ϣ
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2013.10.19
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2013.10.19
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "wind_config.h"
#include "wind_type.h"
#include "console_framework.h"
#include "wind_err.h"
#include "wind_debug.h"
#include "wind_mem.h"
#include "wind_string.h"
#include "wind_thread.h"
#include "wind_sem.h"
#include "wind_list.h"
#include "wind_var.h"

#include "console_framework.h"


static void cmd_showdisc(void)
{
    wind_printf("show core objects infomation.\r\n");
}

static void cmd_showusage(void)
{
    wind_printf("list thread:to show thread infomation.\r\n");
    wind_printf("list sem:to show sem infomation.\r\n");
}

static w_err_t cmd_main(w_int32_t argc,char **argv)
{
    if(argc < 2)
    {
        cmd_showusage();
        return ERR_OK;
    }
    else if(0 == wind_strcmp(argv[1],"thread"))
    {
        wind_thread_print(&g_core.threadlist);
        return ERR_OK;
    }
    else if(0 == wind_strcmp(argv[1],"sem"))
    {
        wind_sem_print(&g_core.semlist);
        return ERR_COMMAN;
    }
    return ERR_COMMAN;
}

CMD_DEF(list);

