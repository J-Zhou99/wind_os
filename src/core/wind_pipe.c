/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_pipe.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os�Ĺܵ���صĴ���,�ܵ���һ������ͨ�Ż��ƣ������߳�ͬ������
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
#include "wind_config.h"
#include "wind_type.h"
#include "wind_pipe.h"
#include "wind_os_hwif.h"
#include "wind_core.h"
#include "wind_err.h"
#include "wind_stat.h"
#include "wind_var.h"
#include "wind_assert.h"
#if WIND_PIPE_SUPPORT > 0

//********************************************internal functions******************************


//********************************************internal functions******************************




//**********************************************extern functions******************************



ppipe_s wind_pipe_create(const char *name,void *inbuf,w_uint32_t inlen,void *outbuf,w_uint32_t outlen)
{
    ppipe_s ppipe;
    w_err_t err;
    WIND_ASSERT_RETURN((inbuf != NULL) || (outbuf != NULL),NULL);
    if(inbuf != NULL)
    {
        WIND_ASSERT_RETURN(inlen > 0,NULL);
    }
    if(outbuf != NULL)
    {
        WIND_ASSERT_RETURN(outlen > 0,NULL);
    }
    
    wind_close_interrupt();
    ppipe = wind_core_alloc(STAT_PIPE);
    
    WIND_ASSERT_TODO(ppipe != NULL,wind_open_interrupt(),NULL);
    if(inbuf != NULL)
    {
        ppipe->in = inbuf;
        err = wind_queue_create(inbuf,inlen,1,LOCK_TYPE_AREA);
        if(ERR_OK != err)
            return NULL;
    }
    if(inbuf != NULL)
    {
        ppipe->out = outbuf;
        err = wind_queue_create(outbuf,outlen,1,LOCK_TYPE_AREA);
        if(ERR_OK != err)
        {
            wind_queue_destory(inbuf);
            return NULL;
        }
    }
    ppipe->name = name;
    ppipe->inlen= inlen;
    ppipe->outlen= outlen;
    //�趨��ǰ���߳̾��
    ppipe->owner = wind_thread_current();
    ppipe->client = NULL;
    ppipe->used = B_TRUE;
    ppipe->magic = WIND_PIPE_MAGIC;
    wind_open_interrupt();
    
    return ppipe;
}


w_err_t wind_pipe_connect(ppipe_s ppipe)
{
    pthread_s pthread = NULL;
    WIND_ASSERT_RETURN(ppipe != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ppipe->magic == WIND_PIPE_MAGIC,ERR_INVALID_PARAM);
    pthread = wind_thread_current();
    WIND_ASSERT_RETURN(pthread != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ppipe->client == NULL,ERR_COMMAN);
    ppipe->client = pthread;
    return ERR_OK;
}


w_int16_t wind_pipe_read(ppipe_s ppipe,w_int8_t *str,w_int16_t len)
{
    pthread_s pthread;
    w_int16_t cnt = -1;
    WIND_ASSERT_RETURN(ppipe != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(str != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(len > 0,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(ppipe->magic == WIND_PIPE_MAGIC,ERR_INVALID_PARAM);
    
    wind_close_interrupt();
    pthread = wind_thread_current();
    WIND_ASSERT_TODO(pthread != NULL,wind_open_interrupt(),ERR_COMMAN);
    if(pthread == ppipe->client)
    {
        cnt = wind_queue_read(ppipe->out,str,len);
    }
    else if(pthread == ppipe->owner)
    {
        cnt = wind_queue_read(ppipe->in,str,len);
    }
    wind_open_interrupt();
    return cnt;
}

w_int16_t wind_pipe_write(ppipe_s ppipe,w_int8_t *str,w_int16_t len)
{
    pthread_s pthread;
    w_int16_t cnt = -1;
    WIND_ASSERT_RETURN(ppipe != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ppipe->magic == WIND_PIPE_MAGIC,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(str != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(len > 0,ERR_INVALID_PARAM);
    wind_close_interrupt();
    pthread = wind_thread_current();
    WIND_ASSERT_TODO(pthread != NULL,wind_open_interrupt(),ERR_COMMAN);
    if(pthread == ppipe->client)
    {
        cnt = wind_queue_write(ppipe->in,str,len);
    }
    else if(pthread == ppipe->owner)
    {
        cnt = wind_queue_write(ppipe->out,str,len);
    }
    wind_open_interrupt();
    return cnt;
}

w_err_t wind_pipe_free(ppipe_s ppipe)
{
    pthread_s pthread;
    WIND_ASSERT_RETURN(ppipe != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ppipe->magic == WIND_PIPE_MAGIC,ERR_INVALID_PARAM);
    pthread = wind_thread_current();
    WIND_ASSERT_RETURN(pthread == ppipe->owner,ERR_COMMAN);
    wind_close_interrupt();
    ppipe->magic = 0;
    ppipe->used = B_FALSE;
    ppipe->name = NULL;
    wind_core_free(STAT_PIPE,ppipe);
    return ERR_OK;
}
#endif //WIND_PIPE_SUPPORT


//**********************************************extern functions******************************

