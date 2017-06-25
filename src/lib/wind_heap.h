/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_heap.h / wind_heap.c
**��   ��   ��: �ܽ���
**����޸�����: 
**��        ��: ϵͳ���ڴ�ѵĶ�̬���亯��
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: �ܽ���
** ��  ��: v1.0
** �ա���: 
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�----------------------------------------------------------------------------
** �޸���: 
** �ա���: 2013.11.03,��������ͨ��
** �衡��: �����ڴ�ѵķ����㷨����Ҫ��������RT-threadϵͳ���ڴ�ѹ�������Ϊ������wind_osʵ�ּ��ݣ���
**         �����ĺ�������һ���̶��ϵ��޸ģ������ѿռ�ĳ�ʼ�����ٽ��������ȹ���
**
** ��  ע����Ҫ���жϷ��������з�����ͷŶ�̬�ڴ��
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
#ifndef WIND_HEAP_H__
#define WIND_HEAP_H__

#include "wind_config.h"
#include "wind_types.h"
#include "wind_lock.h"

#ifdef __cplusplus
extern "C" {
#endif
#if WIND_HEAP_SUPPORT > 0


#define WIND_HEAP_NAME_LEN 12
/* dynamic pool magic and mask */
#define WIND_HEAP_ALIGN_SIZE 4
#define WIND_HEAP_MAGIC        0x1ea01ea0
#define WIND_HEAP_MASK         0xfffffffe
#define WIND_HEAP_USED         0x01
#define wind_heap_FREED        0x00

#define WIND_HEAP_IS_USED(h)   ((h)->magic & WIND_HEAP_USED)
#define WIND_HEAP_MINIALLOC    12

#define WIND_HEAP_ALIGN(size,N) ((size + N -1) / N * N)
#define WIND_HEAP_SIZE         WIND_HEAP_ALIGN(sizeof(heapitem_s), WIND_HEAP_ALIGN_SIZE)
#define MEMITEM_SIZE(item)      ((u32_t)item->next - (u32_t)item - WIND_HEAP_SIZE)


//�ڴ��Ŀ����Ķ��壬���ϵͳ���ڶ�����������ڴ�������Ӧ����������Ӷ���
//ͬʱ��Ӧ����wind_heap_block_init������ע���µ��ڴ��
typedef enum __HeapBlock_e
{
    HEAP_BLOCK_1 = 0,//��һ��
    HEAP_BLOCK_CNT//����
}HeapBlock_e;

#define HEAP1_HEAD  0X68000000
#define HEAD1_LENTH (800*1024)







//struct __memheap;

typedef struct __heapitem_s
{
    u32_t magic;                      /**< magic number for memheap */
    struct __memheap *pool_ptr;                   /**< point of pool */

    struct __heapitem_s *next;                       /**< next memheap item */
    struct __heapitem_s *prev;                       /**< prev memheap item */

    struct __heapitem_s *next_free;                  /**< next free memheap item */
    struct __heapitem_s *prev_free;                  /**< prev free memheap item */
}heapitem_s,*pheapitem_s;


typedef struct __memheap
{
    s8_t name[WIND_HEAP_NAME_LEN];
    void *start_addr;                 /**< pool start address and size */
    u32_t pool_size;                  /**< pool size */
    u32_t available_size;             /**< available size */
    u32_t max_used_size;              /**< maximum allocated size */

    pheapitem_s block_list;                 /**< used block list */
    pheapitem_s free_list;                  /**< free block list */
    heapitem_s free_header;                /**< free block list header */
    plock_s plock;                       /**< semaphore lock */
}memheap_s,*pmemheap_s;

err_t wind_heap_init(pmemheap_s mhp,
                         const char *name,
                         void *start_addr,
                         u32_t size);

void wind_heap_block_init(void);

void *wind_heap_alloc(pmemheap_s heap, u32_t size);
void *wind_heap_alloc_default(u32_t size);

void *wind_heap_realloc(pmemheap_s heap, void* ptr, u32_t newsize);

err_t wind_heap_free(void *ptr);

void *wind_hmalloc(u32_t size);

err_t wind_hfree(void *rmem);

void *wind_hcalloc(u32_t count, u32_t size);

void wind_heap_showinfo(void);

#endif

#ifdef __cplusplus
}
#endif


#endif
