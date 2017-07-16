/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_stack.c
**��   ��   ��: �ܽ���
**����޸�����: 2013.10.05
**��        ��: wind os�Ķ�ջ��صĴ���
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

//#include "wind_core_inc.h"
#include "wind_types.h"
#include "wind_stack.h"
#include "wind_os_hwif.h"


static w_err_t defaultreadempty(pstack_s pstk,void *data)
{
    pstk->emptycnt ++;
    return STACK_EMPTY;
}

static w_err_t defaultwritefull(pstack_s pstk,void *data)
{
    pstk->fullcnt ++;
    return STACK_FULL;
}
/*********************************************************************************************************
** ��������: wind_stack_create
** ��������: ��ʼ�����ݶ�ջ
** �䡡��: Buf      ��Ϊ��ջ����Ĵ洢�ռ��ַ
**         lenth��Ϊ��ջ����Ĵ洢�ռ��С���ֽڣ�
**         DataWid  :���������ݿ��
**         ReadEmpty��Ϊ��ջ����ʱ�������
**         WriteFull��Ϊ��ջд��ʱ�������
** �䡡��: STACK_ERR:��������
**         STACK_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: wind_close_interrupt,wind_open_interrupt()
**
** ������: �ܽ���
** �ա���: 2012.09.26
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

w_err_t wind_stack_create(void *Buf,
                          w_uint32_t lenth,
                          w_uint16_t DataWid,
                          w_err_t (* ReadEmpty)(pstack_s pstk,void *data),
                          w_err_t (* WriteFull)(pstack_s pstk,void *data)
                          )
{
    pstack_s pstk;
    if ((Buf != NULL) && (lenth >= sizeof(stack_s)))        // && �жϲ����Ƿ���Ч 
    {
        pstk = (pstack_s )Buf;

        wind_close_interrupt();
        if(ReadEmpty)                                                        // ��ʼ���ṹ������ 
            pstk->ReadEmpty = ReadEmpty;
        else
            pstk->ReadEmpty = defaultreadempty;
        if(WriteFull)
            pstk->WriteFull = WriteFull;
        else
            pstk->WriteFull = defaultwritefull;
         // �����ջ���Դ洢��������Ŀ     
        pstk->MaxData = (lenth - (w_uint32_t)(((pstack_s)0)->Buf)) / sizeof(STACK_DATA_TYPE);
                                       
        pstk->Top = pstk->Buf + pstk->MaxData;               // �������ݻ���Ľ�����ַ 
        pstk->Out = pstk->Buf;
        pstk->In = pstk->Buf;
        pstk->DataWid = DataWid;
        pstk->NData = 0;
        pstk->emptycnt = 0;
        pstk->fullcnt = 0;        
        wind_open_interrupt();
        return STACK_OK;
    }
    else
    {
        return STACK_ERR;
    }
}


/*********************************************************************************************************
** ��������: wind_stack_read
** ��������: ��ȡ��ջ�е�����
** �䡡��: Ret:�洢���ص���Ϣ�ĵ�ַ
**         Buf:ָ���ջ��ָ��
** �䡡��: STACK_ERR     ����������
**         STACK_OK   ���յ���Ϣ
**         STACK_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: wind_close_interrupt,wind_open_interrupt()
**
** ������: �ܽ���
** �ա���: 2012.09.26
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
w_err_t wind_stack_read(void *Buf,void *Ret)
{
    w_err_t err;
    w_int8_t i;
    pstack_s pstk;
    //w_uint8_t *pEnd;
    w_uint8_t *ret = (w_uint8_t *)Ret;
    err = STACK_ERR;
    if (Buf != NULL)                                            /* ��ջ�Ƿ���Ч */
    {                                                           /* ��Ч */
        pstk = (pstack_s)Buf;
        
        wind_close_interrupt();
        if (pstk->NData >= pstk->DataWid)                     /* ��ջ�Ƿ�Ϊ�� */
        {                                                       /* ����         */
            pstk->In = pstk->Out;
            for(i = 0;i < pstk->DataWid;i ++)
            {
                *(ret + i) = pstk->Out[0];                               /* ���ݳ���     */
                pstk->Out++;                                       /* ��������ָ�� */
                pstk->NData--;                                     /* ���ݼ���      */
            }
            pstk->Out -= pstk->DataWid;
            err = STACK_OK;
        }
        else
        {                                                       /* ��              */
               err = pstk->ReadEmpty(pstk,Ret);
        }
        wind_open_interrupt();
    }
    return err;
}



/*********************************************************************************************************
** ��������: wind_stack_write
** ��������: FIFO��ʽ��������
** �䡡��: Buf :ָ���ջ��ָ��
**         Data:��Ϣ����
** �䡡��: STACK_ERR   :��������
**         STACK_FULL:��ջ��
**         STACK_OK  :���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: wind_close_interrupt,wind_open_interrupt()
**
** ������: �ܽ���
** �ա���: 2012.09.26
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_STACK_WRITE
#define EN_STACK_WRITE      0
#endif

#if EN_STACK_WRITE > 0

w_err_t wind_stack_write(void *Buf, void *Data)
{
    w_err_t err;
    w_int8_t i;
    pstack_s pstk;
    //w_uint8_t *pEnd;
    w_uint8_t *data = (w_uint8_t *)Data;
    err = STACK_ERR;
    if (Buf != NULL)                                                    /* ��ջ�Ƿ���Ч */
    {
        pstk = (pstack_s )Buf;
        wind_close_interrupt();
        if (pstk->NData <= pstk->MaxData - pstk->DataWid)                              /* ��ջ�Ƿ���  */
        {                                                               /* ����        */
            for(i = 0;i < pstk->DataWid;i ++)
            {
                pstk->In[0] = *(data + i);                                        /* �������    */
                pstk->In++;
                pstk->NData++;                                             /* ��������    */
            }
            pstk->Out = pstk->In - pstk->DataWid;
            err = STACK_OK;
        }
        else
        {                                                               /* ��           */
            err = STACK_FULL;
            if (pstk->WriteFull != NULL)                               /* �����û������� */
            {
                err = pstk->WriteFull(pstk, Data);
            }
        }
        wind_open_interrupt();
    }
    return err;
}
#endif



/*********************************************************************************************************
** ��������: wind_stack_datalen
** ��������: ȡ�ö�ջ��������
** �䡡��: Buf:ָ���ջ��ָ��
** �䡡��: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: wind_close_interrupt,wind_open_interrupt()
**
** ������: �ܽ���
** �ա���: 2012.09.26
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_STACK_NDATA
#define EN_STACK_NDATA    0
#endif

#if EN_STACK_NDATA > 0

w_uint16_t wind_stack_datalen(void *Buf)
{
    w_uint16_t temp;
    temp = 0;                                                   /* ��ջ��Ч����0 */
    if (Buf != NULL)
    {
        wind_close_interrupt();
        temp = ((pstack_s )Buf)->NData / ((pstack_s )Buf)->DataWid;
        wind_open_interrupt();
    }
    return temp;
}

#endif

/*********************************************************************************************************
** ��������: wind_stack_size
** ��������: ȡ�ö�ջ������
** �䡡��: Buf:ָ���ջ��ָ��
** �䡡��: ��ջ������
** ȫ�ֱ���: ��
** ����ģ��: wind_close_interrupt,wind_open_interrupt()
**
** ������: �ܽ���
** �ա���: 2012.09.26
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_stack_sIZE
#define EN_stack_sIZE    0
#endif

#if EN_stack_sIZE 

w_uint16_t wind_stack_size(void *Buf)
{
    w_uint16_t temp;
    temp = 0;                                                   /* ��ջ��Ч����0 */
    if (Buf != NULL)
    {
        wind_close_interrupt();
        temp = ((pstack_s )Buf)->MaxData / ((pstack_s )Buf)->DataWid;
        wind_open_interrupt();
    }
    return temp;
}

#endif

/*********************************************************************************************************
** ��������: wind_stack_flush
** ��������: ��ն�ջ
** �䡡��: Buf:ָ���ջ��ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: wind_close_interrupt,wind_open_interrupt()
**
** ������: �ܽ���
** �ա���: 2012.09.26
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef EN_STACK_FLUSH
#define EN_STACK_FLUSH    0
#endif

#if EN_STACK_FLUSH 

void wind_stack_flush(void *Buf)
{
    pstack_s pstk;
    if (Buf != NULL)                                                /* ��ջ�Ƿ���Ч */
    {                                                               /* ��Ч         */
        pstk = (pstack_s)Buf;
        wind_close_interrupt();
        pstk->NData = 0;                                           /* ������ĿΪ0 */
        pstk->Top = pstk->Buf + pstk->MaxData;               // �������ݻ���Ľ�����ַ 
        pstk->Out = pstk->Buf;
        pstk->In = pstk->Buf;
        wind_open_interrupt();
    }
}
#endif



/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

