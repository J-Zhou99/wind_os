/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
** ��   ��   ��: cmd_proc.c
** ��   ��   ��: �ܽ���
** ����޸�����: 2015/9/29 22:28:32
** ��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2015/9/29 22:28:32
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: ��proc��ص��������
** ���ļ���C����Դ�ļ�ģ��������ɡ�------------��纣����Ʒ��������Ʒ��------------
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


/*********************************************ͷ�ļ�����***********************************************/
#include "wind_config.h"
#include "wind_types.h"
#include "wind_var.h"
#include "console_framework.h"
#include "wind_thread.h"

/********************************************�ڲ���������**********************************************/



/********************************************�ڲ���������*********************************************/
static err_t wind_thread_output(pthread_s pthread)
{
    WIND_ASSERT_RETURN(pthread != NULL,ERR_NULL_POINTER);
    wind_printf("proc name:%s\r\n",pthread->name);
    wind_printf("proc stack:%d\r\n",pthread->pstk);
    wind_printf("proc prio:0x%x\r\n\r\n",pthread->prio);
    wind_printf("proc state:%d\r\n",pthread->proc_status);
    wind_printf("proc stack size:%d\r\n",pthread->stksize);
    return ERR_OK;
}



/********************************************ȫ�ֱ�������**********************************************/



/********************************************ȫ�ֺ�������**********************************************/


err_t cmd_proc_show_list_main(s32_t argc,char **argv)
{
    pnode_s node = g_core.pcblist.head;
    pthread_s pthread;
    
    WIND_DEBUG("\r\n\r\nprocess list as following:\r\n");
    while(node != NULL)
    {
        pthread = (pthread_s)node->obj;
        wind_thread_output(pthread);
        node = node->next;
    }
    return ERR_OK;
}

err_t cmd_proc_show_obj_main(s32_t argc,char **argv)
{
    pthread_s pthread = wind_get_proc_byname(argv[0]);
    WIND_ASSERT_RETURN(pthread != NULL,ERR_NULL_POINTER);
    return wind_thread_output(pthread);
}


cmd_s g_cmd_proc[] = 
{
    {
        NULL,"proc show list","To show all the proc struct.",
        "",cmd_proc_show_list_main
    },
    {
        NULL,"proc show obj","To show a proc struct by name.",
        "proc show obj <name>,name is a proc name provided by creator.",
        cmd_proc_show_obj_main
    }
};
void register_cmd_proc(console_s *ctrl)
{
    wind_cmd_register(&ctrl->cmd_list,g_cmd_proc,sizeof(g_cmd_proc)/sizeof(cmd_s));
}

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
