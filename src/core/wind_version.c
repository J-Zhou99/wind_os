/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_version.h / wind_version.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os�ĺ��Ĵ���
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
#include "wind_config.h"
#include "wind_types.h"
#include "wind_version.h"
#include "wind_debug.h"
const s8_t versioninfo[] = \
"yzfcer@163.com,thank you for your support!\r\n";

u8_t *g_wind_logo[4] = 
{
    " _    __    _",
    " \\\\  //\\\\  // ",
    "  \\\\//  \\\\//  ",
    "   \\/    \\/   "   
};
const s8_t *wind_get_core_version_info(void)
{
    return versioninfo;
}

const s8_t *wind_get_core_version(void)
{
    return WIND_CORE_VERSION;
}
static void output_logo(void)
{
    int i;
    for(i = 0;i < 4;i++)
    {
        WIND_INFO("%s\r\n",g_wind_logo[i]);
    }
}

void wind_output_product_info(void)
{
    const s8_t *str;
    WIND_INFO("\r\n\r\nwind_os version:%s\r\n",wind_get_core_version());
    output_logo();
    WIND_INFO("core created on:%s @ chengdu,sichuan province,china.\r\n",__DATE__);
    str = wind_get_core_version_info();
    WIND_INFO(str);
    WIND_INFO("core is built at %s %s\r\n",__TIME__,__DATE__);
}

