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
** �衡��: �����ڴ�ѵķ����㷨����Ҫ����ֲ��RT-threadϵͳ���ڴ�ѹ�������Ϊ������wind_osʵ�ּ��ݣ���
**         ���еĺ�������һ���̶��ϵ��޸ģ������ѿռ�ĳ�ʼ�����ٽ��������ȹ���
**
** ��  ע����Ҫ���жϷ��������з�����ͷŶ�̬�ڴ��
**------------------------------------------------------------------------------------------------------
*******************************************************************************************************/
/*
 * File      : mhp.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2012, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-04-10     Bernard      first implementation
 * 2012-10-16     Bernard      add the mutex lock for heap object.
 * 2012-12-29     Bernard      mhp can be used as system heap.
 *                             change mutex lock to semaphore lock.
 * 2013-04-10     Bernard      add wind_heap_realloc function.
 * 2013-05-24     Bernard      fix the wind_heap_realloc issue.
 * 2013-10-25     andy zhou    �޸ĳ�������Ӧwind_osϵͳ.
 */

//#include <rthw.h>
//#include <rtthread.h>

//#ifdef RT_USING_MEMHEAP



#include "wind_config.h"
#include "wind_type.h"
#include "wind_heap.h"
#include "wind_lock.h"
#include "WIND_ERR.h"
#include "wind_string.h"
#include "wind_debug.h"
#include "wind_list.h"
#include "wind_assert.h"


#if WIND_HEAP_SUPPORT > 0
dlist_s gwind_heaplist = {NULL,NULL};//�����ڴ������
//�ڴ�ѵĿռ��ı����λ��
memheap_s gwind_heap[HEAP_BLOCK_CNT];
#define WIND_HEAP_DEBUG(...)


w_err_t wind_heap_init(pmemheap_s mhp,
                         const char *name,
                         void *base,
                         w_uint32_t size)
{
    w_err_t err;
    pheapitem_s item;
    WIND_ASSERT_RETURN(mhp != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(name != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(base != NULL,ERR_NULL_POINTER);
    WIND_ASSERT_RETURN(err == ERR_OK,ERR_COMMAN);
    DNODE_INIT(mhp->mnode);
    wind_memset(mhp->name,0,WIND_HEAP_NAME_LEN);
    wind_memcpy(mhp->name,name,WIND_HEAP_NAME_LEN - 1);
    mhp->base = base;
    mhp->pool_size = WIND_HEAP_ALIGN(size, WIND_HEAP_ALIGN_SIZE);//
    mhp->available_size = mhp->pool_size - (2 * WIND_HEAP_SIZE);
    mhp->max_used_size = mhp->pool_size - mhp->available_size;

    item = &(mhp->free_header);
    item->magic = WIND_HEAP_MAGIC;
    item->pool_ptr = mhp;
    item->next = NULL;
    item->prev = NULL;
    item->next_free = item;
    item->prev_free = item;

    mhp->free_list = item;

    item = (pheapitem_s )base;
    item->magic = WIND_HEAP_MAGIC;
    item->pool_ptr = mhp;
    item->next = NULL;
    item->prev = NULL;
    item->next_free = item;
    item->prev_free = item;

    item->next = (pheapitem_s)((w_uint8_t *)item + mhp->available_size + WIND_HEAP_SIZE);
    item->prev = item->next;

    mhp->block_list = item;


    item->next_free = mhp->free_list->next_free;
    item->prev_free = mhp->free_list;
    mhp->free_list->next_free->prev_free = item;
    mhp->free_list->next_free  = item;


    item = item->next;

    item->magic = WIND_HEAP_MAGIC | WIND_HEAP_USED;
    item->pool_ptr = mhp;
    item->next = (pheapitem_s )base;
    item->prev = (pheapitem_s )base;

    item->next_free = item->prev_free = NULL;
    mhp->plock = wind_lock_create(name);
    wind_close_interrupt();
    dlist_insert_tail(&gwind_heaplist,&mhp->mnode);
    wind_open_interrupt();
    return ERR_OK;
}



//�ѿ����ɷ�����ڴ�ռ���г�ʼ��
void wind_heap_block_init(void)
{
    WIND_INFO("memory heap initializing...\r\n");
    wind_heap_init(&gwind_heap[0],
                    "heap0",
                    (void *)HEAP1_HEAD,
                    HEAD1_LENTH);
    wind_heap_showinfo();
}

void *wind_heap_alloc_default(w_uint32_t size)
{
    int i;
    void *p;
    for(i = 0;i < HEAP_BLOCK_CNT;i ++)
    {
        p = wind_heap_alloc(&gwind_heap[0],size);
        if(NULL != p)
            return p;
    }
    return p;
}

//���ڴ���зֳ�һ��ռ�
void *wind_heap_alloc(pmemheap_s heap, w_uint32_t size)
{
    w_err_t result;
    w_uint32_t free_size;
    pheapitem_s header_ptr;
    pheapitem_s new_ptr;
    WIND_ASSERT_RETURN(heap != NULL,NULL);
    size = WIND_HEAP_ALIGN(size, WIND_HEAP_ALIGN_SIZE);
    if (size < WIND_HEAP_MINIALLOC)
        size = WIND_HEAP_MINIALLOC;
    
    WIND_HEAP_DEBUG("allocate %d on heap\r\n",size);
    WIND_HEAP_DEBUG("pool_size:0x%x\r\n",heap->pool_size);
    WIND_HEAP_DEBUG("available_size:0x%x\r\n",heap->available_size);
    
    if (size < heap->available_size)
    {

        free_size = 0;
        result = wind_lock_close(heap->plock);
        if (result != ERR_OK)
        {
            WIND_HEAP_DEBUG("lock close err\r\n");
            return NULL;
        }
        header_ptr = heap->free_list->next_free;
        
        while (header_ptr != heap->free_list && free_size < size)
        {
            free_size = MEMITEM_SIZE(header_ptr);
            if (free_size < size)
            {
                header_ptr = header_ptr->next_free;
            }
        }

        if (free_size >= size)
        {

            if (free_size >= (size + WIND_HEAP_SIZE + WIND_HEAP_MINIALLOC))
            {

                new_ptr = (pheapitem_s)
                          (((w_uint8_t *)header_ptr) + size + WIND_HEAP_SIZE);

                WIND_HEAP_DEBUG("split: block[0x%x] nextm[0x%x] prevm[0x%x] to new[0x%x]\r\n",
                              header_ptr,
                              header_ptr->next,
                              header_ptr->prev,
                              new_ptr);


                new_ptr->magic = WIND_HEAP_MAGIC;


                new_ptr->pool_ptr = heap;


                new_ptr->prev          = header_ptr;
                new_ptr->next          = header_ptr->next;
                header_ptr->next->prev = new_ptr;
                header_ptr->next       = new_ptr;


                header_ptr->next_free->prev_free = header_ptr->prev_free;
                header_ptr->prev_free->next_free = header_ptr->next_free;
                header_ptr->next_free = NULL;
                header_ptr->prev_free = NULL;


                new_ptr->next_free = heap->free_list->next_free;
                new_ptr->prev_free = heap->free_list;
                heap->free_list->next_free->prev_free = new_ptr;
                heap->free_list->next_free            = new_ptr;
                heap->available_size = heap->available_size -
                                       size -
                                       WIND_HEAP_SIZE;
                if (heap->pool_size - heap->available_size > heap->max_used_size)
                    heap->max_used_size = heap->pool_size - heap->available_size;
            }
            else
            {

                heap->available_size = heap->available_size - free_size;
                if (heap->pool_size - heap->available_size > heap->max_used_size)
                    heap->max_used_size = heap->pool_size - heap->available_size;



                header_ptr->next_free->prev_free = header_ptr->prev_free;
                header_ptr->prev_free->next_free = header_ptr->next_free;
                header_ptr->next_free = NULL;
                header_ptr->prev_free = NULL;
            }


            header_ptr->magic |= WIND_HEAP_USED;

            wind_lock_open(heap->plock);
            WIND_HEAP_DEBUG("alloc mem: memory[0x%08x], heap[0x%08x], size: %d\r\n",
                          (void *)((w_uint8_t *)header_ptr + WIND_HEAP_SIZE),
                          header_ptr,
                          size);


            return (void *)((w_uint8_t *)header_ptr + WIND_HEAP_SIZE);
        }
        wind_lock_open(heap->plock);
    }
    return NULL;
}


void *wind_heap_realloc(pmemheap_s heap, void* ptr, w_uint32_t newsize)
{
    w_err_t result;
    w_uint32_t oldsize;
    pheapitem_s header_ptr;
    pheapitem_s new_ptr;
    WIND_ASSERT_RETURN(heap != NULL,NULL);
    if (newsize == 0)
    {
        wind_heap_free(ptr);
        return NULL;
    }

    newsize = WIND_HEAP_ALIGN(newsize, WIND_HEAP_ALIGN_SIZE);
    if (newsize < WIND_HEAP_MINIALLOC)
        newsize = WIND_HEAP_MINIALLOC;

    if (ptr == NULL)
    {
        return wind_heap_alloc(heap, newsize);
    }


    header_ptr = (heapitem_s*)((w_uint8_t *)ptr - WIND_HEAP_SIZE);
    oldsize = MEMITEM_SIZE(header_ptr);

    if (newsize > oldsize)
    {
        void* new_ptr;

        new_ptr = (void*)wind_heap_alloc(heap, newsize);
        if (new_ptr != NULL)
        {
            wind_memcpy(new_ptr, ptr, oldsize < newsize ? oldsize : newsize);
            wind_heap_free(ptr);
        }

        return new_ptr;
    }


    result = wind_lock_close(heap->plock);
    if (result != ERR_OK)
    {
        return NULL;
    }


    new_ptr = (pheapitem_s)(((w_uint8_t *)header_ptr) + newsize + WIND_HEAP_SIZE);




    new_ptr->magic = WIND_HEAP_MAGIC;

    new_ptr->pool_ptr = heap;


    new_ptr->prev           = header_ptr;
    new_ptr->next           = header_ptr->next;
    header_ptr->next->prev = new_ptr;
    header_ptr->next       = new_ptr;


    if (!WIND_HEAP_IS_USED(new_ptr->next))
    {
        pheapitem_s free_ptr;


        free_ptr = new_ptr->next;
        heap->available_size = heap->available_size - MEMITEM_SIZE(free_ptr);



        free_ptr->next->prev = new_ptr;
        new_ptr->next    = free_ptr->next;


        free_ptr->next_free->prev_free = free_ptr->prev_free;
        free_ptr->prev_free->next_free = free_ptr->next_free;
    }


    new_ptr->next_free = heap->free_list->next_free;
    new_ptr->prev_free = heap->free_list;
    heap->free_list->next_free->prev_free = new_ptr;
    heap->free_list->next_free              = new_ptr;

    heap->available_size = heap->available_size + MEMITEM_SIZE(new_ptr);


    wind_lock_open(heap->plock);
    return ptr;
}


w_err_t wind_heap_free(void *ptr)
{
    w_err_t result;
    pmemheap_s heap;
    pheapitem_s header_ptr, new_ptr;
    w_uint32_t insert_header;
    WIND_ASSERT_RETURN(ptr != NULL,ERR_NULL_POINTER);


    insert_header = 1;
    new_ptr       = NULL;
    header_ptr    = (pheapitem_s )((w_uint8_t *)ptr - WIND_HEAP_SIZE);


    heap = header_ptr->pool_ptr;


    result = wind_lock_close(heap->plock);
    if (result != ERR_OK)
    {

        return result;
    }


    header_ptr->magic &= ~WIND_HEAP_USED;

    heap->available_size = heap->available_size + MEMITEM_SIZE(header_ptr);


    if (!WIND_HEAP_IS_USED(header_ptr->prev))
    {

        heap->available_size = heap->available_size + WIND_HEAP_SIZE;


        (header_ptr->prev)->next = header_ptr->next;
        (header_ptr->next)->prev = header_ptr->prev;


        header_ptr = header_ptr->prev;

        insert_header = 0;
    }


    if (!WIND_HEAP_IS_USED(header_ptr->next))
    {

        heap->available_size = heap->available_size + WIND_HEAP_SIZE;


        new_ptr = header_ptr->next;



        new_ptr->next->prev = header_ptr;
        header_ptr->next    = new_ptr->next;


        new_ptr->next_free->prev_free = new_ptr->prev_free;
        new_ptr->prev_free->next_free = new_ptr->next_free;
    }

    if (insert_header)
    {

        header_ptr->next_free = heap->free_list->next_free;
        header_ptr->prev_free = heap->free_list;
        heap->free_list->next_free->prev_free = header_ptr;
        heap->free_list->next_free            = header_ptr;


    }


    result = wind_lock_open(heap->plock);
    return result;
}


void *wind_hmalloc(w_uint32_t size)
{
    void *ptr = NULL;
    pmemheap_s heap;
    pdlist_s list = &gwind_heaplist;
    pdnode_s pnode = dlist_head(list);
    if(!pnode)
    {
        WIND_HEAP_DEBUG("NULL pnode in heap\r\n");
        return NULL;
    }
    while(pnode)
    {
        heap = DLIST_OBJ(pnode,memheap_s,mnode);
        WIND_HEAP_DEBUG("malloc in heap:0x%x\r\n",heap);
        WIND_HEAP_DEBUG("pool_size:0x%x\r\n",heap->pool_size);
        WIND_HEAP_DEBUG("available_size:0x%x\r\n",heap->available_size);
        ptr = wind_heap_alloc(heap, size);
        if(ptr)
        {
            break;
        }
        pnode= dnode_next(pnode);
        
    }
    if(!ptr)
    {
        WIND_HEAP_DEBUG("NULL ptr in heap\r\n");
    }
    return ptr;
}


w_err_t wind_hfree(void *rmem)
{
    WIND_ASSERT_RETURN(rmem != NULL,ERR_NULL_POINTER);
    return wind_heap_free(rmem);
}


void *wind_hrealloc(void *rmem, w_uint32_t newsize)
{
    void *pnew;
    pheapitem_s phead;
    w_uint32_t oldsize;
    if (rmem == NULL) 
        return wind_hmalloc(newsize);
    phead = (pheapitem_s )((w_uint8_t *)rmem - WIND_HEAP_SIZE);

    pnew = wind_heap_realloc(phead->pool_ptr, rmem, newsize);
    if (pnew == NULL && newsize != 0)
    {

        pnew = wind_hmalloc(newsize);
        if (pnew != NULL && rmem != NULL)
        {

            oldsize = MEMITEM_SIZE(phead);
            if (newsize > oldsize) wind_memcpy(pnew, rmem, oldsize);
            else wind_memcpy(pnew, rmem, newsize);
        }
    }
    return pnew;
}


void *wind_hcalloc(w_uint32_t count, w_uint32_t size)
{
    void *ptr;
    w_uint32_t tot_size;
    WIND_ASSERT_RETURN(count > 0,NULL);
    WIND_ASSERT_RETURN(size > 0,NULL);
    tot_size = count * size;
    ptr = wind_hmalloc(tot_size);
    WIND_ASSERT_RETURN(ptr != NULL,NULL);
    wind_memset(ptr, 0, tot_size);
    return ptr;
}


void wind_heap_showinfo(void)
{
    pdnode_s pnode;
    pmemheap_s heap;
    wind_printf("heap list:\r\n");
    pnode = dlist_head(&gwind_heaplist);
    
    while(pnode)
    {
        heap = DLIST_OBJ(pnode,memheap_s,mnode);
        wind_printf("name:%s\r\n",heap->name);
        wind_printf("start:0x%x\r\n",heap->base);
        wind_printf("size:0x%x\r\n",heap->pool_size);
        pnode = dnode_next(pnode);
    }
}

w_err_t wind_heap_test(void)
{
    w_uint8_t *p1,*p2,*p3;
    p1 = wind_hmalloc(120);
    if(p1)
    {
        wind_printf("p1:0x%x,size:120\r\n",p1);
    }
    else
    {
        WIND_ERROR("p1:malloc err\r\n");
    }
    p2 = wind_hmalloc(1024);
    if(p2)
    {
        wind_printf("p2:0x%x,size:1024\r\n",p2);
    }
    else
    {
        WIND_ERROR("p2:malloc err\r\n");
    }
    p3 = wind_hmalloc(0x1000000);
    if(p3)
    {
        wind_printf("p3:0x%x,size:0x10000\r\n",p3);
    }
    else
    {
        WIND_ERROR("p3:malloc err\r\n");
    }
    wind_printf("memory free:p1\r\n");
    wind_hfree(p1);
    wind_printf("memory free:p2\r\n");
    wind_hfree(p2);
    wind_printf("memory free:p3\r\n");
    wind_hfree(p3);
    return ERR_OK;
    
}
#endif

