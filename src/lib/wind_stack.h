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
#include "wind_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_STACK_SUPPORT > 0


#ifndef NOT_OK
#define NOT_OK              -3                        /* ��������                                     */
#endif

#define STACK_OK            0                           /* �����ɹ�                                     */
#define STACK_ERR            -1
#define STACK_FULL          -2                           /* ������                                       */
#define STACK_EMPTY         -3                           /* ������                                       */

//#define Q_WRITE_MODE        1                           /* �����ɹ�                                     */
//#define Q_WRITE_FRONT_MODE  2                           /* �����ɹ�                                     */

#ifndef STACK_DATA_TYPE
#define STACK_DATA_TYPE     u8_t
#endif

//__packed
//#pragma pack(1)
typedef struct __stack_s{
    STACK_DATA_TYPE     *Out;                   /* ָ���������λ��         */
    STACK_DATA_TYPE     *In;                    /* ָ����������λ��         */      
    STACK_DATA_TYPE     *Top;                    /* ָ��Buf�Ľ���λ��        */
    u16_t              NData;                  /* ���������ݸ���           */
    u16_t              MaxData;                /* ����������洢�����ݸ��� */
    u8_t                DataWid;                /* Ԫ�ص����ݿ�� */  
    u32_t              emptycnt;
    u32_t              fullcnt;
    err_t               (* ReadEmpty)(struct __stack_s *pstk,void *data);     /* ���մ�����             */
    err_t               (* WriteFull)(struct __stack_s *pstk,void *data);        /* д��������             */
    STACK_DATA_TYPE     Buf[1];                 /* �洢���ݵĿռ�           */
} stack_s,*pstack_s;


//#ifndef IN_STACK
err_t wind_stack_create(void *Buf,
                          u32_t SizeOfBuf,
                          u16_t DataWid,
                          err_t (* ReadEmpty)(),
                          err_t (* WriteFull)()
                          );


err_t wind_stack_read(void *Buf,void *Ret);

err_t wind_stack_write(void *Buf, void *Data);

u16_t wind_stack_datalen(void *Buf);

u16_t wind_stack_size(void *Buf);

void wind_stack_flush(void *Buf);
//#endif //IN_STACK

#define EN_STACK_WRITE            1     /* ��ֹ(0)������(1)FIFO��������       */
#define EN_STACK_WRITE_FRONT      1     /* ��ֹ(0)������(1)LIFO��������       */
#define EN_STACK_NDATA            1     /* ��ֹ(0)������(1)ȡ�ö���������Ŀ   */
#define EN_STACK_SIZE             1     /* ��ֹ(0)������(1)ȡ�ö������������� */
#define EN_STACK_FLUSH            1     /* ��ֹ(0)������(1)��ն���           */


#endif

#ifdef __cplusplus
}
#endif

#endif  //WIND_STACK_H_

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

