/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
** ��   ��   ��: cmd_help.h / cmd_help.c
** ��   ��   ��: �ܽ���
** ����޸�����: 2015/1/24 20:52:30
** ��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2015/1/24 20:52:30
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: �����˵�����庯��
** ���ļ���C����Դ�ļ�ģ��������ɡ�------------��纣����Ʒ��������Ʒ��------------
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


/*********************************************ͷ�ļ�����***********************************************/
//#include "wind_config.h"
//#include "wind_types.h"
#include "console_framework.h"



/********************************************�ڲ���������**********************************************/
	#if 0
static const char * const help_str = "help [opt]:to show some helping infomation\r\n";
static const char * const help_details[] = 
{
    "help:to show list of commands\r\n",
    "help [cmd]:to get more info of the command\r\n"
};
#endif

/********************************************�ڲ���������*********************************************/



/********************************************ȫ�ֱ�������**********************************************/



/********************************************ȫ�ֺ�������**********************************************/

static w_err_t cmd_help_main(w_int32_t argc,char **argv)
{
    cmd_s *cmdlist = wind_get_cmdlist();
    if(argc < 1)
    {
        wind_printf("\r\ncommand list as following:\r\n");
        while(cmdlist)
        {
            wind_printf("%s : %s\r\n",cmdlist->cmd,cmdlist->helpdetails);
            cmdlist = cmdlist->next;
        }
        return ERR_OK;
    }
    while(cmdlist)
    {
        if(wind_strcmp(argv[0],cmdlist->cmd) == 0)
        {
            wind_printf("%s : %s\r\n",cmdlist->cmd,cmdlist->helpdetails);
            break;
        }
        cmdlist = cmdlist->next;
    }
    return ERR_OK;
}


cmd_s g_cmd_help[] = 
{
    {
        NULL,
        "help",
        "get help infomation.",
        "",
        cmd_help_main
    }
};

void register_cmd_help(console_s *ctrl)
{
    wind_cmd_register(&ctrl->cmd_list,g_cmd_help,sizeof(g_cmd_help)/sizeof(cmd_s));
}

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
