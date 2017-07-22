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
#include "wind_types.h"
#include "wind_debug.h"
#ifdef __cplusplus
extern "C" {
#endif
#define WIND_CTL_TEST_EN 1//����̨���ܵ�ʱ�ܱ�־
#define WIND_CTL_DEBUG_EN 0//����̨����ʱ�ܱ�־

#define WIND_CMD_MAX_LEN 512//һ���������󳤶�
#define WIND_CMD_NAME_LEN 12//һ�������ʾ����󳤶�
#define WIND_CTL_USRNAME_LEN 20//�û����ĳ���
#define WIND_CTL_PWD_LEN 20//�������󳤶�
#define WIND_CONSOLE_COUNT 1//֧�ֵĿ������ն˵�����
//#define WIND_CTL_OUT(...) WIND_STD_OUT(__VA_ARGS__)
#define WIND_CTL_GET(pch) core_get_cmd_ch(pch)




typedef enum __cslstat_e
{
    CSLSTAT_USER,//��Ҫ�����û���
    CSLSTAT_PWD,//��Ҫ��������
    CSLSTAT_CMD,//��������ģʽ
    CSLSTAT_APP //������Ӧ�ó���ģʽ
} cslstat_e;

typedef struct __cmd_s
{
    struct __cmd_s *next;
    char* cmd;//���������
    char *helpinfo;//��Ҫ������Ϣ
    char *helpdetails;//��ϸ�İ���˵��
    w_err_t (*execute)(w_int32_t argc,char **argv);//�������ں���
    
}cmd_s,*pcmd_s;


#endif//ifndef WIND_CONSOLE_H__
#ifdef __cplusplus
}
#endif

