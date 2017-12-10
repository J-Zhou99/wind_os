/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_stack.h
**��   ��   ��: �ܽ���
**����޸�����: 2013.10.05
**��        ��: wind os����Ϣ��صĴ���
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 2013.10.05
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: �ܽ���
** �ա���: 2013.10.05
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef    WIND_STACK_H_
#define WIND_STACK_H_

#include "wind_config.h"
#include "wind_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_STACK_SUPPORT

#ifndef NOT_OK
#define NOT_OK              -3                        /* ��������                                     */
#endif

#define STACK_OK            0                           /* �����ɹ�                                     */
#define STACK_ERR           -1
#define STACK_FULL          -2                           /* ������                                       */
#define STACK_EMPTY         -3                           /* ������                                       */

//#define Q_WRITE_MODE        1                           /* �����ɹ�                                     */
//#define Q_WRITE_FRONT_MODE  2                           /* �����ɹ�                                     */

#ifndef STACK_DATA_TYPE
#define STACK_DATA_TYPE     w_uint8_t
#endif

typedef struct __stack_s{
    STACK_DATA_TYPE     *Out;                   /* ָ���������λ��         */
    STACK_DATA_TYPE     *In;                    /* ָ����������λ��         */      
    STACK_DATA_TYPE     *Top;                    /* ָ��Buf�Ľ���λ��        */
    w_uint16_t              NData;                  /* ���������ݸ���           */
    w_uint16_t              MaxData;                /* ����������洢�����ݸ��� */
    w_uint8_t                DataWid;                /* Ԫ�ص����ݿ�� */  
    w_uint32_t              emptycnt;
    w_uint32_t              fullcnt;
    w_err_t               (* ReadEmpty)(struct __stack_s *pstk,void *data);     /* ���մ�����             */
    w_err_t               (* WriteFull)(struct __stack_s *pstk,void *data);        /* д��������             */
    STACK_DATA_TYPE     Buf[1];                 /* �洢���ݵĿռ�           */
} stack_s,*pstack_s;


w_err_t wind_stack_create(void *Buf,
                          w_uint32_t SizeOfBuf,
                          w_uint16_t DataWid,
                          w_err_t (* ReadEmpty)(),
                          w_err_t (* WriteFull)()
                          );


w_err_t wind_stack_read(void *Buf,void *Ret);

w_err_t wind_stack_write(void *Buf, void *Data);

w_uint16_t wind_stack_datalen(void *Buf);

w_uint16_t wind_stack_size(void *Buf);

void wind_stack_flush(void *Buf);

#endif

#ifdef __cplusplus
}
#endif

#endif  //WIND_STACK_H_

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

