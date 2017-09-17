/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_driver.c
**��   ��   ��: �ܽ���
**����޸�����: 
**��        ��: ϵͳ����������ص�Դ�ļ�
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: 
** ��  ��: v1.0
** �ա���: 
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#include "port/wind_config.h"
#include "port/wind_type.h"
#include "lib/wind_err.h"
#include "driver/wind_driver.h"
#include "core/wind_lock.h"
#include "port/wind_cpu_port.h"
pchdev_s g_chdev_list = NULL;
pchdev_s g_chdev_last = NULL;
void wind_insert_chdev(pchdev_s dev)
{
    wind_close_interrupt();
    if(g_chdev_list == NULL)
    {
        g_chdev_list = dev;
        g_chdev_last = dev;
        g_chdev_list->next = NULL;
        wind_open_interrupt();
        return;
    }
    g_chdev_last = dev;
    g_chdev_last->next = NULL;
    wind_open_interrupt();
}

err_t wind_remove_chdev(pchdev_s dev)
{
    return ERR_OK;
    
}

