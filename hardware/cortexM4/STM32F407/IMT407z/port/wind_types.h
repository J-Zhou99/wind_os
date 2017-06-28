/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_types.h
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os�����Ͷ���
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
#ifndef WIND_TYPES_H__
#define WIND_TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifndef u8_t
typedef unsigned char u8_t;
#endif

#ifndef u16_t
typedef unsigned int u16_t;
#endif

#ifndef u32_t
typedef unsigned long u32_t; 
#endif

#ifndef u64_t
typedef unsigned long long u64_t; 
#endif

#ifndef s8_t
typedef char s8_t; 
#endif

#ifndef s16_t
typedef int s16_t; 
#endif

#ifndef s32_t
typedef long s32_t; 
#endif

#ifndef s64_t
typedef signed long long s64_t; 
#endif


#ifndef fp32_t
typedef float fp32_t;
#endif

//#ifndef 

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef bool_t
typedef enum __bool_t
{
    B_FALSE = 0,
    B_TRUE = 1
} bool_t;

#ifndef TRUE 
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif


#ifndef err_t
typedef s16_t err_t;
#endif


#ifndef stack_t  //��ջ��ȵĶ���
typedef u32_t stack_t;//,*pstack_t;
typedef u32_t *pstack_t;
#endif


#ifndef HANDLE 
typedef s16_t HANDLE;
#endif

typedef enum __lock_type_e
{
    LOCK_TYPE_NONE = 0,
    LOCK_TYPE_AREA = 1,
    LOCK_TYPE_GLOBAL = 2,
    LOCK_TYPE_BUTT
}lock_type_e;



// #else  //TYPEDEF_OK
// #define u8_t unsigned char 
// #define u16_t unsigned short
// #define u32_t unsigned int
// #define s8_t char
// #define s16_t short
// #define s32_t int
// #define bool_t unsigned char
// #define true 1
// #define false 0
// #define err_t s16_t
// #define HANDLE s16_t
// #define stack_t unsigned int
// #define pstack_t unsigned int *
// #ifndef NULL
// #define NULL (void*)0
// #endif

// #ifndef extern
// #define extern
// #endif  //extern
// 
// #endif //TYPEDEF_OK


#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  

