/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
** ��   ��   ��: console_framework.h / console_framework.c
** ��   ��   ��: �ܽ���
** ����޸�����: 2015/1/24 20:08:03
** ��        ��: 
**  
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2015/1/24 20:08:03
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: �ܹ�֧�ֶ����׼��������Ŀ���̨�Ŀ��
** ���ļ���C����Դ�ļ�ģ��������ɡ�------------��纣����Ʒ��������Ʒ��------------
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef __CONSOLE_FRAMEWORK_H__
#define __CONSOLE_FRAMEWORK_H__
#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

/*********************************************ͷ�ļ�����***********************************************/
#include "wind_config.h"
#include "wind_types.h"
#include "wind_err.h"
#include "wind_key.h"
#include "wind_console.h"
#include "wind_assert.h"
#include "wind_string.h"
#include "wind_thread.h"

/***********************************************�궨��*************************************************/
#define WIND_CMD_MAX_LEN 512//һ���������󳤶�
#define WIND_CMD_NAME_LEN 12//һ�������ʾ����󳤶�
#define CMD_PARAM_CNT 10
#define WIND_CTL_USRNAME_LEN 20//�û����ĳ���
#define WIND_CTL_PWD_LEN 20//�������󳤶�
#define WIND_CONSOLE_COUNT 1//֧�ֵĿ������ն˵�����
#define CONSOLE_OUT(...) wind_printf(__VA_ARGS__)


/**********************************************ö�ٶ���************************************************/





/*********************************************�ṹ�嶨��***********************************************/




//ȫ�ֵ�test suite�б�
typedef struct __cmd_global
{
    cmd_s *head;
    cmd_s *tail;
    u32_t cnt;
}cmd_global_s;

//�õ��ֽ��Ĳ����б�
typedef struct __cmd_param_s
{
    u32_t cnt;
    char * param[CMD_PARAM_CNT];
}cmd_param_s;
typedef struct __console_s
{
    char cmdstr[WIND_CMD_MAX_LEN];//����Ļ�����
    char buf[WIND_CMD_MAX_LEN];//���յ����ݻ�����
    u32_t index;//�������һ���ַ��±�
    cslstat_e stat;//��ǰ�Ľ���״̬
    char user[WIND_CTL_USRNAME_LEN];//�û���
    char pwd[WIND_CTL_PWD_LEN];//�����ֵ
    cmd_param_s param;
    cmd_global_s cmd_list;
}console_s;
/********************************************ȫ�ֱ�������**********************************************/



/********************************************ȫ�ֺ�������**********************************************/

void register_cmd_echo(console_s *ctrl);
void register_cmd_help(console_s *ctrl);
void register_cmd_proc(console_s *ctrl);
void register_cmd_show(console_s *ctrl);
void register_cmd_stat(console_s *ctrl);
void register_cmd_test(console_s *ctrl);



//��������б�
err_t wind_output_cmdlist(void);

void console_framework_init(console_s *ctlobj);
cmd_s *wind_get_cmdlist(void);
s32_t is_string_equal(char *dest,char *src);
err_t wind_cmd_register(cmd_global_s *cgl,cmd_s *cmd,int cnt);
//err_t consoleProc(s32_t argc,char **argv);
pthread_s lunch_console(void);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif // #ifndef __CONSOLE_FRAMEWORK_H__
