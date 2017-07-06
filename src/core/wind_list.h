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
#include "wind_types.h"
//#include "wind_thread.h"
//#include "wind_message.h"
//#include "wind_sem.h"

#ifdef __cplusplus
extern "C"{
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
    bool_t used;
    bool_t minus;
    s32_t key;
    void *obj;
    pnode_s next;
    pnode_s prev;
};

struct __wind_list
{
    pnode_s head;
    pnode_s tail;
    int cnt;
};


pnode_s wind_node_malloc(core_type type);
err_t wind_node_free(pnode_s node);

err_t wind_list_init(plist_s list);
err_t wind_list_insert(plist_s list,pnode_s node);
err_t wind_list_inserttoend(plist_s list,pnode_s node);
err_t wind_list_inserttohead(plist_s list,pnode_s node);
err_t wind_list_insert_with_minus(plist_s list,pnode_s node);
pnode_s wind_list_remove(plist_s list,pnode_s node);
pnode_s wind_list_search(plist_s list,void *obj);

void printlist(plist_s list);
#ifdef __cplusplus
}
#endif

#endif
