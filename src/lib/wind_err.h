#ifndef WIND_ERR_H__
#define WIND_ERR_H__
#include "wind_config.h"
#include "wind_types.h"
#include "wind_os_hwif.h"
#include "wind_debug.h"

#define ERR_OK                 0 //�������ؽ��
#define ERR_COMMAN            -1 //һ�����
#define ERR_NULL_POINTER      -2 //ָ��Ϊ��
#define ERR_PARAM_OVERFLOW    -3 //����Ĳ��������˷�Χ
#define ERR_EMPTY             -5 //���ʵĶ���û�л�ȡ��
#define ERR_NO_SPACE          -4 //�洢�ռ��ܵ�����
#define ERR_INVALID_PARAM     -6//������Ч
#define ERR_TIMEOUT           -7//������ʱ������ʧ��

#define ERR_INVALID_HANDLER -1 //��Ч�ľ��

extern w_int8_t *wind_err_set[];
#define WIND_FUNC_NAME(name) w_int8_t funcname = x
#define WIND_ERR(i) WIND_ERROR("%s:%s",funcname,wind_err_set[i])

#endif

