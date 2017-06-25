/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_message.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os����Ϣ��صĴ���ͷ�ļ�
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
#ifndef WIND_MESSAGE_H__
#define WIND_MESSAGE_H__


#include "wind_config.h"
#include "wind_types.h"
#include "wind_list.h"
#include "wind_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_MESSAGE_SUPPORT > 0

typedef struct _wind_message
{
    const char *name;
    bool_t used;
    u16_t validtime;
    u16_t msgNO;
    u16_t param;
    void *lparam;
}msg_s, *pmsg_s; 

typedef struct __mbox_s
{
    const char *name;
    int num;//��Ϣ������
    bool_t used;
    bool_t valid;//�����Ƿ����
    pnode_s msgq;//��Ϣ����
    pthread_s owner;
}mbox_s, *pmbox_s;


pmsg_s wind_message_create(const char *name,u16_t msgNo,u16_t param,void *lparam);
err_t wind_message_destroy(pmsg_s pmsg);

pmbox_s wind_mbox_create(const char *name);
err_t wind_mbox_destroy(pmbox_s pmbox);

err_t wind_mbox_post(pmbox_s mbox,pmsg_s pmsg);
err_t wind_mbox_fetch(pmbox_s mbox,pmsg_s *pmsg,u32_t timeout);
/**/
#endif //WIND_MESSAGE_SUPPORT

#ifdef __cplusplus
}
#endif
#endif  //WIND_MESSAGE_H__

