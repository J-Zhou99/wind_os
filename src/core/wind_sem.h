/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_sem.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os���ź���ͷ�ļ�
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
#ifndef WIND_SEM_H__
#define WIND_SEM_H__


#include "wind_config.h"
#include "wind_types.h"
//#include "wind_list.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_SEM_SUPPORT > 0
#define SEM_NAME_LEN 20 //�߳�������󳤶ȣ����� '\0'

typedef struct _wind_sem
{
    char name[SEM_NAME_LEN];
    w_bool_t used;         //�Ƿ���Ч�ı�־
    w_uint16_t sem_num;     //��ǰ���ź�����ֵ
    w_uint16_t tot_sem_num; //��ʼ�����ź�����ֵ
    list_s waitlist;  //�ȴ��̶߳���
}sem_s,*psem_s;



psem_s wind_sem_create(const char *name,w_uint16_t semValue);
w_err_t wind_sem_post(psem_s psem);
w_err_t wind_sem_fetch(psem_s psem,w_uint32_t timeout);
w_err_t wind_sem_tryfree(psem_s psem);
w_err_t wind_sem_free(psem_s psem);
w_err_t wind_sem_test(void);

#endif
#ifdef __cplusplus
}
#endif
#endif//#ifndef WIND_SEM_H__

