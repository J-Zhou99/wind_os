/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_lock.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: ϵͳ�Ļ�����
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.2
** �ա���: 2013.06.23
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2012.06.23
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef WIND_LOCK_H__
#define WIND_LOCK_H__

#include "wind_config.h"
#include "wind_type.h"
#include "wind_list.h"

#ifdef __cplusplus
extern "C" {
#endif
#if WIND_LOCK_SUPPORT > 0

typedef struct _wind_lock
{
    //HANDLE handler;    //���������
    const char *name;
    w_bool_t used;         //�Ƿ���Ч�ı�־
    w_bool_t locked;     //��ǰ���ź�����ֵ
    list_s waitlist;  //�ȴ��̶߳���
}lock_s,*plock_s;

//w_err_t wind_lock_init(void);
plock_s wind_lock_create(const char *name);
w_err_t wind_lock_tryfree(plock_s plock);
w_err_t wind_lock_free(plock_s plock);
w_err_t wind_lock_close(plock_s plock);
w_err_t wind_lock_open(plock_s plock);
w_err_t wind_lock_print(plist_s list);


#endif //WIND_LOCK_SUPPORT > 0
#ifdef __cplusplus
}
#endif


#endif

