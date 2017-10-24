/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_pipe.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os�Ĺܵ���صĴ���ͷ�ļ�
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
#ifndef WIND_PIPE_H__
#define WIND_PIPE_H__

#include "wind_queue.h"
#include "wind_config.h"
#include "wind_type.h"
#include "wind_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (WIND_PIPE_SUPPORT && WIND_QUEUE_SUPPORT)

#define WIND_PIPE_MAGIC 0x56384c6a
#define PIPE_NAME_LEN 12

typedef struct __WIND_PIPE
{
    w_uint32_t magic;
    const char *name;
    w_bool_t used;
    queue_s *buff;
    w_uint32_t buflen;
}pipe_s,*ppipe_s;

w_err_t wind_pipe_init(void);
ppipe_s wind_pipe_create(const char *name,void *buff,w_uint32_t buflen);
w_err_t wind_pipe_connect(ppipe_s ppipe);
w_int16_t wind_pipe_read(ppipe_s ppipe,w_int8_t *str,w_int16_t len);
w_int16_t wind_pipe_write(ppipe_s ppipe,w_int8_t *str,w_int16_t len);
w_err_t wind_pipe_free(ppipe_s ppipe);




#endif //WIND_PIPE_SUPPORT
#ifdef __cplusplus
}
#endif


#endif

