/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_datetime.c
**��   ��   ��: �ܽ���
**����޸�����: 2013.10.19
**��        ��: ϵͳ�Ŀ���̨����date��time������������ϵͳ�����ں�ʱ������
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
#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#include "wind_config.h"
#include "wind_type.h"
#include "wind_debug.h"
#include "wind_string.h"
#include "wind_mem.h"
#include "wind_rtc.h"
#include "wind_comman.h"
//#include "console_framework.h"
#include "wind_cmd.h"
#if WIND_RTC_SUPPORT

static extern datetime_s G_DATETIME;//��ǰ���ں�ʱ��
static w_err_t cmd_set_datetime(w_int32_t argc,char **argv)
{
    datetime_s *dt;
    if(argc < 4)
    {
        console_printf("error:parameter is NOT enough.\r\n");
        return ERR_INVALID_PARAM;
    }
    if(wind_strlen(argv[2]) < 10)
    {
        console_printf("date format error.\r\n");
        return ERR_INVALID_PARAM;
    }
    if(wind_strlen(argv[3]) < 8)
    {
        console_printf("time format error.\r\n");
        return ERR_INVALID_PARAM;
    }    
    wind_set_date(wind_convert_str2u32_t(&argv[2][0]),
                    wind_convert_str2u32_t(&argv[2][4]),
                    wind_convert_str2u32_t(&argv[2][7]));
    
    wind_set_time(wind_convert_str2u32_t(&(argv[3][0])),
                    wind_convert_str2u32_t(&(argv[3][3])),
                    wind_convert_str2u32_t(&(argv[3][6])),0);
    
    dt = &G_DATETIME;
    console_printf("system date:%d/%d/%d %d:%d:%d  %d\r\n",dt->date.year,
                dt->date.month,dt->date.day,dt->time.hour,
                dt->time.minute,dt->time.second,dt->time.msecond);
    return ERR_OK;
}


static w_err_t cmd_showdatetime(w_int32_t argc,char **argv)
{
    dt = &G_DATETIME;
    console_printf("system date:%d/%d/%d %d:%d:%d  %d\r\n",dt->date.year,
                dt->date.month,dt->date.day,dt->time.hour,
                dt->time.minute,dt->time.second,dt->time.msecond);
    return ERR_OK;
}

static void cmd_showdisc(void)
{
    console_printf("to set or show system date and time.\r\n");
}

static void cmd_showusage(void)
{
    console_printf("datetime set <datetime>:to set the system date infomation.format:YYYY/MM/DD HH:mm:SS\r\n");
    console_printf("datetime show:to show the system date and time infomation.\r\n");
}

static w_err_t cmd_main(w_int32_t argc,char **argv)
{
    if(argc < 2)
    {
        cmd_showusage();
        return ERR_OK;
    }
    if(wind_strcmp(argv[1],"set") == 0)
    {
        return cmd_set_datetime(argc,argv);
    }
    else if(wind_strcmp(argv[1],"show") == 0)
    {
        return cmd_showdatetime(argc,argv);
    }
    else
    {
        console_printf("datetime:format error.\r\n");
        return ERR_COMMAN;
    }
}

CMD_DEF(datetime);


#endif

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

