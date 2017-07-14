/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_mem.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: �ڴ������صĴ��루δ�������
**��        ע: Ƕ��ʽϵͳ���ڴ�ռ�������������ޣ����Ҷ����ʱЧ����Ҫ����˽���������ر��Ҫ��
**              ��������Ҫ�ö�̬����ķ�ʽ��ͬʱ��ʹ����ɺ�Ҫ�����ͷţ����������϶����Ƭ����
**              Ӱ��ϵͳ������
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
#include "wind_types.h"
#include "wind_mem.h"
#include "wind_debug.h"
#include "wind_os_hwif.h"
#include "wind_err.h"
#include "wind_heap.h"
#include "wind_assert.h"
#if WIND_MEM_SUPPORT > 0





meminfo_s g_mem_for_allcoc[] = 
{
    {
        0x30100000,
        0x4000000,
        0,
        NULL,
        NULL
    }
};

w_err_t wind_mem_init(w_uint8_t ix,w_uint32_t base,w_uint32_t lenth)
{
    WIND_ASSERT_RETURN((lenth > 0),ERR_COMMAN);
    g_mem_for_allcoc[ix].base = base;
    g_mem_for_allcoc[ix].lenth = lenth;
    g_mem_for_allcoc[ix].offset = 0;
    g_mem_for_allcoc[ix].plast = NULL;
    g_mem_for_allcoc[ix].pfree = NULL;
    return ERR_OK;
    
}

//���Ѿ����յĿռ����ҵ�һ��
static void *core_get_free_space(w_uint32_t size)
{
    return NULL;
}

//����һ�������Ŀռ䣬�������ɹ����ͻ᷵�ؿռ�ָ�룬ʧ�ܷ���NULL
void *wind_malloc(w_uint32_t size)
{
    w_uint32_t si;
    void *p;
    pmemhead_s phead;
#if MEM_ALIGN_4
    si = (size + 3) & 0xfffffffc;
#else
    si = size;
#endif
    wind_close_interrupt();
    //��������ж�������������ɿռ䣬���ܻ���Ҫ�Ľ�
    //phead = (pmemhead_s)(g_mem_for_allcoc[0].base + g_mem_for_allcoc[0].offset);
    phead = (pmemhead_s)wind_heap_alloc_default(si + sizeof(memhead_s));
    //��Ϊǰ��ĳ�ʼ�ռ��Ѿ��������ˣ���ˣ����Կ�ʼ���Ǵӻ��յĿռ��з����µĿռ�
    //����ӻ��յĿռ��з������µĿռ䣬��ô������Գɹ���������佫ʧ��
    //phead = core_get_free_space(si + sizeof(memhead_s));
    if(!phead)
    {
        wind_open_interrupt();
        return NULL;
    }
    
    p = (void*)((phead) + sizeof(memhead_s));
    phead->pre = g_mem_for_allcoc[0].plast;
    phead->next = NULL;
    phead->lenth = si + sizeof(memhead_s);
    if(g_mem_for_allcoc[0].plast)
        g_mem_for_allcoc[0].plast->next = phead;
    g_mem_for_allcoc[0].plast = phead;
    wind_open_interrupt();
    return p;
}

//����һ����ά����ָ�룬������Ŀռ䲻��ֱ����Ϊһάָ��ʹ��
void **wind_calloc(w_uint32_t block,w_uint32_t size)
{
    w_uint32_t si,i,base;
    void **p;
#if MEM_ALIGN_4
    si = (size + 3) & 0xfffffffc;
#else
    si = size;
#endif
    p = (void **)wind_malloc(block * sizeof(void *) + block * si);
    if(p == NULL)
        return NULL;
    base = block * sizeof(void *) + (w_uint32_t)p;
    for(i = 0;i < block;i ++)
    {
        p[i] = (void *)(base + si * i);
    }
    return p;
}


//����һ����ά����ָ�룬������Ŀռ䲻��ֱ����Ϊһάָ��ʹ��
void ***wind_talloc(w_uint32_t num,w_uint32_t block,w_uint32_t size)
{
    w_uint32_t si,i,j,base;
    
    void ***p;
#if MEM_ALIGN_4
    si = (size + 3) & 0xfffffffc;
#else
    si = size;
#endif
    p = (void ***)wind_malloc(num * sizeof(void **) + num * block * sizeof(void *) + num * block * si);
    if(p == NULL)
        return NULL;
    base = num * sizeof(void **) + num * block * sizeof(void *) + (w_uint32_t)p;
    for(i = 0;i < num;i ++)
    {
        p[i] = (void **)(base + num * sizeof(void **) + i * block * sizeof(void *));
        for(j = 0;j < block;j ++)
        {
            p[i][j] = (void *)(base + i * block * si + j *si);
        }
    }
    return p;
}

//Ϊϵͳ���տռ䣬�������ڵĿռ����ӳ�������Ƭ�Ŀռ�,���������Щ���Ѱ�����������
void wind_free(void *p)
{
    pmemhead_s phead,ph1,ph2;
    if(p == NULL || (w_uint32_t)p < g_mem_for_allcoc[0].base 
        || (w_uint32_t)p > (g_mem_for_allcoc[0].base + g_mem_for_allcoc[0].lenth))
        return;
    phead = (pmemhead_s)(((w_uint32_t)p) - sizeof(memhead_s));
    wind_close_interrupt();
    if(g_mem_for_allcoc[0].pfree == NULL)
    {
        g_mem_for_allcoc[0].pfree = phead;
        phead->next = NULL;
        phead->pre = NULL;
        wind_open_interrupt();
        return;        
    }
    else 
    {
        ph1 = NULL;
        ph2 = g_mem_for_allcoc[0].pfree;
        //�ҵ����صĿ���ʵ�λ��
        while(ph2 != NULL)
        {
            if(phead < ph2)
            {
                ph1 = ph2;
                ph2 = ph2->next;
            }
            else
                break;
        }
        //���ڴ�����ӵ�������
        if(ph1 == NULL)
        {
            g_mem_for_allcoc[0].pfree = phead;
            phead->pre = NULL;
        }
        else
        {
            ph1->next = phead;
            phead->pre = ph1;
        }
        
        
        phead->next = ph2;
        if(ph2 != NULL)
            ph2->pre = phead;
        //�ϲ����ڵ��ڴ��
        if(ph1->lenth + sizeof(memhead_s) + (w_uint32_t)ph1 == (w_uint32_t)phead)
        {
            ph1->lenth += (phead->lenth + sizeof(memhead_s));
            ph1->next = ph2;
            ph2->pre = ph1;
            phead = ph1;
        }
        if(ph2 && (phead->lenth + sizeof(memhead_s) + (w_uint32_t)phead == (w_uint32_t)ph2))
        {
            phead->lenth += (ph2->lenth + sizeof(memhead_s));
            phead->next = ph2->next;
            ph2->next->pre = phead;
        }
    }
    wind_open_interrupt();
    return;
}

#endif //WIND_MEM_SUPPORT
