/****************************************Copyright (c)**************************************************
**                                       ��  ��  ��  ��
**
**                                       yzfcer@163.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: wind_list.h / wind_list.c
**��   ��   ��: �ܽ���
**����޸�����: 2012.09.26
**��        ��: wind os���ں�������
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
#ifndef WIND_LIST_H__
#define WIND_LIST_H__
#include "wind_config.h"
#include "wind_type.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifndef __INLINE__
#define __INLINE__ 
#endif

typedef enum __core_type
{
    CORE_TYPE_NONE = 0,
    CORE_TYPE_PCB,//PCB
    CORE_TYPE_MSG,//��Ϣ
    CORE_TYPE_SEM,//�ź���
    CORE_TYPE_TTIMER,//tick��ʱ��
    CORE_TYPE_LOCK,//������
    CORE_TYPE_HEAP,//�ڴ��
    CORE_TYPE_MAX
} core_type;


typedef struct __wind_node node_s,*pnode_s;
typedef struct __wind_list list_s,*plist_s;

struct __wind_node
{
    core_type type;
    w_bool_t used;
    w_bool_t minus;
    w_int32_t key;
    void *obj;
    pnode_s next;
    pnode_s prev;
};

struct __wind_list
{
    pnode_s head;
    pnode_s tail;
    w_int32_t cnt;
};

pnode_s wind_node_malloc(core_type type);
w_err_t wind_node_free(pnode_s node);
void wind_node_bindobj(pnode_s node,core_type type,w_int32_t key,void *obj);

static __INLINE__ pnode_s list_head(plist_s list) {return list->head;}
static __INLINE__ pnode_s list_tail(plist_s list) {return list->tail;}
static __INLINE__ w_int32_t list_count(plist_s list) {return list->cnt;}
static __INLINE__ pnode_s next_node(pnode_s node) {return node->next;}
static __INLINE__ pnode_s prev_node(pnode_s node) {return node->prev;}

w_err_t wind_list_init(plist_s list);
w_err_t wind_list_insert(plist_s list,pnode_s node);
w_err_t wind_list_inserttoend(plist_s list,pnode_s node);
w_err_t wind_list_inserttohead(plist_s list,pnode_s node);
w_err_t wind_list_insert_with_minus(plist_s list,pnode_s node);
pnode_s wind_list_remove(plist_s list,pnode_s node);
pnode_s wind_list_search(plist_s list,void *obj);

void wind_list_print(plist_s list);
#ifdef __cplusplus
}
#endif

#endif
