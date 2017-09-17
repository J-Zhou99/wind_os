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
#include "wind_type.h"
#include "wind_err.h"
#include "wind_key.h"
#include "wind_console.h"
#include "wind_assert.h"
#include "wind_string.h"
#include "wind_thread.h"
#include "cmd_history.h"
/***********************************************�궨��*************************************************/
#define WIND_CMD_MAX_LEN 512//һ���������󳤶�
#define WIND_CMD_NAME_LEN 12//һ�������ʾ����󳤶�
#define CMD_PARAM_CNT 10
#define WIND_CTL_USRNAME_LEN 20//�û����ĳ���
#define WIND_CTL_PWD_LEN 20//�������󳤶�
#define WIND_CONSOLE_COUNT 1//֧�ֵĿ������ն˵�����


/**********************************************ö�ٶ���************************************************/





/*********************************************�ṹ�嶨��***********************************************/




//ȫ�ֵ�cmd�б�
typedef struct __cmd_list
{
    cmd_s *head;
    cmd_s *tail;
    w_uint32_t cnt;
}cmd_list_s;



//�õ��ֽ��Ĳ����б�
typedef struct __cmd_param_s
{
    w_uint32_t argc;
    char * argv[CMD_PARAM_CNT];
}cmd_param_s;

typedef struct 
{
    w_err_t (*getchar)(char *ch);
    w_int32_t (*printf)(const char *fmt,...);
}console_ops;


typedef struct __console_s
{
    cslstat_e stat;//��ǰ�Ľ���״̬
    w_int32_t index;//�������һ���ַ��±�
    char buf[WIND_CMD_MAX_LEN];//���յ����ݻ�����
    char user[WIND_CTL_USRNAME_LEN];//�û���
    char pwd[WIND_CTL_PWD_LEN];//�����ֵ
    cmd_his_s his;
    cmd_param_s param;
    cmd_list_s cmd_list;
    console_ops ops;
}console_s;
/********************************************ȫ�ֱ�������**********************************************/



/********************************************ȫ�ֺ�������**********************************************/

//��������б�
w_err_t wind_output_cmdlist(void);

void console_framework_init(console_s *ctlobj);
cmd_s *wind_get_cmdlist(void);
w_err_t wind_cmd_register(cmd_list_s *cgl,cmd_s *cmd,int cnt);
void create_console_thread(void);
void register_all_cmd(console_s *ctrl);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus
#endif // #ifndef __CONSOLE_FRAMEWORK_H__
