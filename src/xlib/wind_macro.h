/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: 
**��   ��   ��: 
**����޸�����: 
**��        ��: 
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: 
** ��  ��: v1.0
** �ա���: 
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef INTER_PROCRESS_H__
#define INTER_PROCRESS_H__

#include "wind_types.h"
#ifdef c_plusplus__
extern {
#endif
#define MEM_B(addr) (*((u8_t *)(addr)))//�Ӹ����ĵ�ַ�ϻ�ȡһ���ֽ�
#define MEM_W(addr) (*((u16_t *)(addr)))//�Ӹ����ĵ�ַ�ϻ�ȡһ����
#define MEM_DW(addr) (*(u32_t *)(addr))//�Ӹ����ĵ�ַ�ϻ�ȡһ��˫��

#define MAX(x, y) (((x) > (y))?(x):(y))
#define MAX3(x, y, z) MAX(MAX(x,y),z)
#define MAX4(x, y, z, w) MAX(MAX(x,y),MAX(z,w))
#define MIN(x, y) (((x) < (y))?(x):(y))
#define MIN3(x, y, z) MINMIN(x,y),z)
#define MIN4(x, y, z, w) MIN(MIN(x,y),MIN(z,w))

//����˸�ʽת����С�˸�ʽ���߽�С�˸�ʽת���ɴ�˸�ʽ
#define BE2LE2(bit16) ((bit16 >> 8) + (bit16 << 8))
#define BE2LE4(bit32) (((bit32 >> 24) & 0xff) \
            + ((bit32 >> 16) & 0xff00)\
            + ((bit32 << 16) & 0xff0000)\
            + ((bit32 << 24) & 0xff000000))
            

//��ýṹ����һ���ֶε�ƫ����
#define FPOS( type, field ) ((u32_t) &((type *)0)->field)
//�õ�һ���ṹ���е�ĳ���ֶεĳ���
#define FSIZ(type,field) sizeof(((type *)0)->field)

//����LSB��ʽ�������ֽ�ת��Ϊһ��u16_t
#define FLIPW( ray ) ( (((u16_t) (ray)[0]) * 256) + (ray)[1] )

//����LSB��ʽ��һ��u16_tת��Ϊ�����ֽ�
#define FLOPW( ray, val ) \
(ray)[0] = ((val) / 256); \
(ray)[1] = ((val) & 0xFF)

//�õ�һ�������ĵ�ַ��u16_t��ȣ�
#define B_PTR( var ) ( (u8_t *) (void *) &(var) )
#define W_PTR( var ) ( (u16_t *) (void *) &(var) )

//�õ�һ���ֵĸ�λ�͵�λ�ֽ�
#define WORD_LO(x) ((u8_t) ((u16_t)(x) & 0xff))
#define WORD_HI(x) ((u8_t) ((u16_t)(x) >> 8))

//����һ����X�����ӽ���8�ı���
#define RND8( x ) ((((x) + 7) / 8 ) * 8 )

//��һ����ĸת��Ϊ��д
#define UPCASE(c) (((c) >= 'a' && (c) <= 'z')?((c) - 0x20):(c))

////��һ����ĸת��Ϊ��д
#define DOWNCASE(c) (((c) >= 'A' && (c) <= 'Z')?((c) + 0x20):(c))

//�ж��ַ��ǲ���10��ֵ������
#define DECCHK( c ) ((c) >= '0' && (c) <= '9')

//�ж��ַ��ǲ���16��ֵ������
#define HEXCHK( c ) (((c) >= '0' && (c) <= '9') ||\
((c) >= 'A' && (c) <= 'F') ||\
((c) >= 'a' && (c) <= 'f'))

//��ֹ�����һ������
#define INC_SAT(val) (val=((val)+1>(val))?(val)+1:(val))


//��������Ԫ�صĸ���
#define ARR_SIZE(a) (sizeof((a))/sizeof((a[0])))

//����һ���޷�����nβ��ֵMOD_BY_POWER_OF_TWO(X,n)=X%(2^n)
#define MOD_BY_POWER_OF_TWO( val, mod_by ) \
((u32_t)(val) & (u32_t)((mod_by)-1))


#ifdef _DEBUG
#define DEBUGMSG(msg,date) printf(msg);printf("?d%d%d"?date,_LINE_,_FILE_)
#else
#define DEBUGMSG(msg,date)
#endif


















#ifdef c_plusplus__
}
#endif


#endif
