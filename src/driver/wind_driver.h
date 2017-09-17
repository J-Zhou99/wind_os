/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_driver.h
**��   ��   ��: �ܽ���
**����޸�����: 
**��        ��: ϵͳ����������ص�ͷ�ļ�
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: 
** ��  ��: v1.0
** �ա���: 
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef WIND_DRIVER_H__
#define WIND_DRIVER_H__

#include "port/wind_config.h"
#include "port/wind_type.h"
#include "driver/wind_driver.h"
#include "core/wind_lock.h"

#ifdef __cplusplus
extern "C" {
#endif


#define WIND_CHER_DEVICE 1
#define WIND_BLOCK_DEVICE 2

typedef struct __chdev_s
{
    struct __chdev_s *next;
    wint8_t name[12];
    HANDLE hdev;
    bool_t sole;//�Ƿ��ռ�豸�ı�־
    bool_t opened;//�豸�Ƿ�򿪵ı�־
    HANDLE lock;//�豸�������ľ��
    err_t (*open)(void *param);//��һ���豸��һ���ǳ�ʼ��
    err_t (*ioctl)(wuint8_t com,void *param);//�����豸�Ĳ�������Բ�ͬ���豸���Զ���
    u16_t (*write)(wuint8_t *buf,wuint16_t len);//���豸дһ��ϵ���ֽ�
    u16_t (*read)(wuint8_t *buf,wuint16_t len);//���豸��ȡһϵ���ֽ�
    err_t (*close)(void);//�ر�һ���豸���򿪵������
}chdev_s,*pchdev_s;

void wind_insert_chdev(pchdev_s dev);
err_t wind_remove_chdev(pchdev_s dev);

HANDLE wind_chdev_get(wint8_t name);
err_t wind_chdev_open(HANDLE hdev,bool_t sole,void *param);
err_t wind_chdev_ioctr(HANDLE hdev,wuint8_t com,void *param);
wuint16_t wind_chdev_read(HANDLE hdev,wuint8_t *buf,wuint16_t len);
wuint16_t wind_chdev_write(HANDLE hdev,wuint8_t *buf,wuint16_t len);
err_t wind_chdev_close(HANDLE hdev);

#ifdef __cplusplus
}
#endif


#endif

