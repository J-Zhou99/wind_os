/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_cpu_port.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os��ʱ��������ͷ�ļ�
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
#ifndef WIND_OS_HWIF_H_
#define WIND_OS_HWIF_H_


#include "wind_config.h"
#include "wind_type.h"

#ifdef __cplusplus
extern "C" {
#endif
void wind_target_init(void);

void wind_std_port_init(void);
w_int32_t wind_std_input(w_uint8_t *str,w_int32_t len);
w_int32_t wind_std_output(w_uint8_t *str,w_int32_t len);

void wind_close_interrupt(void);
void wind_open_interrupt(void);

typedef  void (*thread_run_f)(void *pargs);
//�̶߳�ջ�ĳ�ʼ����ڣ���ֲ��Ҫ����ʵ��
w_pstack_t wind_stk_init(thread_run_f pfunc,void *pdata, w_pstack_t pstkbt);


#ifdef __cplusplus
}
#endif

#endif
