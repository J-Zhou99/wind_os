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
#include "wind_types.h"
#include "wind_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

#if WIND_PIPE_SUPPORT > 0

#define WIND_PIPE_MAGIC 0x56384c6a
#define PIPE_NAME_LEN 10

typedef struct __WIND_PIPE
{
    u32_t magic;
    const char *name;//[PIPE_NAME_LEN];
    pthread_s client;
    pthread_s owner;
    bool_t used;
    queue_s *in;
    queue_s *out;
    u32_t inlen;
    u32_t outlen;
}pipe_s,*ppipe_s;

err_t wind_pipe_init(void);
ppipe_s wind_pipe_create(const char *name,void *inbuf,u32_t inlen,void *outbuf,u32_t outlen);
err_t wind_pipe_connect(ppipe_s ppipe);
s16_t wind_pipe_read(ppipe_s ppipe,s8_t *str,s16_t len);
s16_t wind_pipe_write(ppipe_s ppipe,s8_t *str,s16_t len);
err_t wind_pipe_free(ppipe_s ppipe);




#endif //WIND_PIPE_SUPPORT > 0
#ifdef __cplusplus
}
#endif


#endif

