#ifndef __WIND_DLIST_H__
#define __WIND_DLIST_H__
#include "wind_type.h"
typedef struct __dnode_s dnode_s, *pdnode_s;
typedef struct __dlist_s dlist_s, *pdlist_s;

//�������ݽṹ
struct __dnode_s 
{
 pdnode_s prev;
 pdnode_s next;
};

//����ڵ�ṹ
struct __dlist_s 
{
 pdnode_s head;
 pdnode_s tail;
};

#define DNODE_INIT(dnode) {dnode.prev = NULL;dnode.next = NULL;}
#define DLIST_INIT(dlist) {dlist.head = NULL;dlist.tail = NULL;}
#define FIND(type,e) ((w_uint32_t)&(((type*)0)->e))
#define DLIST_OBJ(ptr,type,e) (void*)(((char*)(ptr))-FIND(type,e))
// ��ȡ����ͷ���ڵ�
static __INLINE__ 
pdnode_s dlist_head(pdlist_s dlist) 
{
    return dlist->head;
}

// ��ȡ����β���ڵ�
static __INLINE__ 
pdnode_s dlist_tail(pdlist_s dlist) 
{
    return dlist->tail;
}

// ��ȡ�����ڵ����һ���ڵ�
static __INLINE__ 
pdnode_s dnode_next(pdnode_s node) 
{
    return node->next;
}

// ��ȡ�����ڵ����һ���ڵ�
static __INLINE__
pdnode_s dnode_prev(pdnode_s node)
{
    return node->prev;
}

// ������ͷ������һ���ڵ�
void dlist_insert_head(pdlist_s dlist,pdnode_s node);

//���� ��β������һ���ڵ�
void dlist_insert_tail(pdlist_s dlist,pdnode_s node);

// ��ָ���ڵ�����һ���ڵ�
void dlist_insert(pdlist_s dlist,pdnode_s lpAfter,pdnode_s node);

// ������ͷ������һ���ڵ�
pdnode_s dlist_remove_head(pdlist_s dlist);

// ������β������һ���ڵ�
pdnode_s dlist_remove_tail(pdlist_s dlist);

// ��������ɾ�������ڵ�
pdnode_s dlist_remove(pdlist_s dlist,pdnode_s node);

// ��� �����Ƿ�Ϊ��
w_bool_t dlist_is_empty(pdlist_s dlist);

// ��ȡ�����еĽڵ���
w_int32_t dlist_get_count(pdlist_s dlist);
// �ϲ���������
pdlist_s dlist_combine(pdlist_s dlist1,pdlist_s dlist2);

#endif//__dlist_s_H__
