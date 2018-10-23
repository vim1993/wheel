#include "type.h"
#include "lxOs.h"
#include "lxMsgQue.h"

typedef struct MsgQueNode_s {
    void * data;
    struct MsgQueNode_s * next;
}MsgQueNode_s;

typedef struct MsgQue_Content_s {
    MsgQueNode_s * head;
    MsgQueNode_s * tail;
    semaphore_t  * semfifo;
    msgque_obj msgObj;
}MsgQue_Content_s;

static MsgQueNode_s * msgque_node_create(void)
{
    MsgQueNode_s * node = (MsgQueNode_s *)lxOSMalloc(sizeof(MsgQueNode_s));
    if(node)
    {
        node->next = NULL;
        node->data = NULL;
    }

    return node;
}

void release_buffer(struct msgque_obj *this, const void * data)
{
    lxOSFree(data);

    return;
}

status_e push_back(struct msgque_obj *this, const void * data, size_t datalen)
{
    MsgQueNode_s * pDataNode = NULL;
    MsgQue_Content_s * pQueCtx = GET_STRUCT_HEAD_PTR(MsgQue_Content_s, this, msgObj);
    if(!pQueCtx)
    {
        return STATUS_E_FALSE;
    }

    pQueCtx->tail->data = lxOSMalloc(datalen);
    memcpy(pQueCtx->tail->data, data, datalen);
    pDataNode = msgque_node_create();
    pQueCtx->tail->next = pDataNode;
    pQueCtx->tail = pDataNode;

    pQueCtx->semfifo->semaphore_post(pQueCtx->semfifo);

    return STATUS_E_TRUE;
}

VOIDPTR pop_front(struct msgque_obj *this)
{
    void * data = NULL;
    MsgQue_Content_s * pQueCtx = GET_STRUCT_HEAD_PTR(MsgQue_Content_s, this, msgObj);
    if(!pQueCtx)
    {
        return NULL;
    }

    pQueCtx->semfifo->semaphore_wait(pQueCtx->semfifo);
    MsgQueNode_s * node= pQueCtx->head;
    data = pQueCtx->head->data;
    pQueCtx->head = pQueCtx->head->next;
    lxOSFree(node);
    return data;
}

msgque_obj * msgque_obj_new(void)
{
    MsgQue_Content_s * pQueCtx = (MsgQue_Content_s *)lxOSMalloc(sizeof(MsgQue_Content_s));
    if(!pQueCtx)
    {
        return NULL;
    }

    pQueCtx->semfifo = semaphore_t_new();
    if(!pQueCtx->semfifo)
    {
        lxOSFree(pQueCtx);
        return NULL;
    }

    pQueCtx->msgObj.pop_front = pop_front;
    pQueCtx->msgObj.push_back = push_back;
    pQueCtx->msgObj.release_buffer = release_buffer;

    pQueCtx->head = msgque_node_create();
    if(!pQueCtx->head)
    {
        semaphore_t_delete(pQueCtx->semfifo);
        lxOSFree(pQueCtx);
        return NULL;
    }
    pQueCtx->tail = pQueCtx->head;

    return &pQueCtx->msgObj;
}

void msgque_obj_delete(msgque_obj * this)
{
    MsgQueNode_s * pNode = NULL, *pTmpNode = NULL;
    MsgQue_Content_s * pQueCtx = GET_STRUCT_HEAD_PTR(MsgQue_Content_s, this, msgObj);
    if(pQueCtx)
    {
        pNode = pQueCtx->head;
        while(pNode != NULL)
        {
            pTmpNode = pNode->next;
            lxOSFree(pNode->data);
            lxOSFree(pNode);

            pNode = pTmpNode;
        }

        semaphore_t_delete(pQueCtx->semfifo);
        lxOSFree(pQueCtx);
    }

    return;
}

