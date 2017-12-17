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
#include "wind_debug.h"
#include "wind_stati.h"
#include "wind_var.h"
#include "core_obj.h"
#if (WIND_PIPE_SUPPORT)

//********************************************internal functions******************************


//********************************************internal functions******************************




//**********************************************extern functions******************************

ppipe_s wind_pipe_create(const char *name,void *buff,w_uint32_t buflen)
{
    ppipe_s ppipe;
    w_err_t err;
    WIND_ASSERT_RETURN((buff != NULL),NULL);
    WIND_ASSERT_RETURN(buflen > 0,NULL);
    
    ppipe = wind_core_alloc(IDX_PIPE);
    WIND_ASSERT_RETURN(ppipe != NULL,NULL);
    ppipe->magic = WIND_PIPE_MAGIC;
    ppipe->name = name;
    ppipe->buflen = buflen;
    ppipe->used = B_TRUE;
    ppipe->buff = buff;
    err = wind_queue_create(buff,buflen,1);
    if(ERR_OK != err)
    {
        wind_core_free(IDX_PIPE,ppipe);
        return NULL;
    }
    return ppipe;
}


w_int16_t wind_pipe_read(ppipe_s ppipe,w_int8_t *str,w_int16_t len)
{
    w_int16_t cnt = -1;
    WIND_ASSERT_RETURN(ppipe != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(str != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(len > 0,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(ppipe->magic == WIND_PIPE_MAGIC,ERR_INVALID_PARAM);
    cnt = wind_queue_read(ppipe->buff,str,len);
    return cnt;
}

w_int16_t wind_pipe_write(ppipe_s ppipe,w_int8_t *str,w_int16_t len)
{
    w_int16_t cnt = -1;
    WIND_ASSERT_RETURN(ppipe != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ppipe->magic == WIND_PIPE_MAGIC,ERR_INVALID_PARAM);
    WIND_ASSERT_RETURN(str != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(len > 0,ERR_INVALID_PARAM);
    cnt = wind_queue_write(ppipe->buff,str,len);
    return cnt;
}

w_err_t wind_pipe_free(ppipe_s ppipe)
{
    WIND_ASSERT_RETURN(ppipe != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(ppipe->magic == WIND_PIPE_MAGIC,ERR_INVALID_PARAM);
    ppipe->magic = 0;
    ppipe->used = B_FALSE;
    ppipe->name = NULL;
    wind_core_free(IDX_PIPE,ppipe);
    return ERR_OK;
}
#endif //WIND_PIPE_SUPPORT


//**********************************************extern functions******************************

