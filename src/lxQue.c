#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "lxOs.h"
#include "lxQue.h"
#include "lxlog.h"

typedef struct lxQue_Node_s {
    struct lxQue_Node_s * next;
    void * data;
    size_t datalen;
}lxQue_Node_s;

typedef struct lxQue_Content_s {
    lxQue_Node_s * Head;
    lxQue_Node_s * Tail;
    lxque_obj mlxQueObj;
}lxQue_Content_s;

static lxQue_Node_s * lxque_node_init(void)
{
    lxQue_Node_s * node = (lxQue_Node_s *)lxOSMalloc(sizeof(lxQue_Node_s));
    if(node)
    {
        node->next = NULL;
        node->data = NULL;
        node->datalen = 0;
    }

    return node;
}

static status_e push_back(struct lxque_obj * pthis, const void * data, size_t len)
{
    if(len <= 0 || !data)
    {
        LOG_DEBUG_PRINT("param error, please check\n");
        return STATUS_E_FALSE;
    }

    lxQue_Content_s * lxQueCtx = GET_STRUCT_HEAD_PTR(lxQue_Content_s, pthis, mlxQueObj);
    if(lxQueCtx)
    {
        lxQueCtx->Tail->data = lxOSMalloc(len);
        if(!lxQueCtx->Tail->data)
        {
            LOG_DEBUG_PRINT("malloc memery failed\n");
            return STATUS_E_MALLOC_FALSE;
        }

        memcpy(lxQueCtx->Tail->data, data, len);
        lxQueCtx->Tail->datalen = len;
        lxQueCtx->Tail->next = lxque_node_init();
        lxQueCtx->Tail = lxQueCtx->Tail->next;
    }

    return STATUS_E_TRUE;
}

static size_t pop_front(struct lxque_obj *pthis, void * outbuffer, size_t bufferlen)
{
    if(bufferlen <= 0 || !outbuffer)
    {
        LOG_DEBUG_PRINT("param error, please check\n");
        return 0;
    }

    size_t len = 0;
    lxQue_Node_s * pNode = NULL;
    lxQue_Content_s * lxQueCtx = GET_STRUCT_HEAD_PTR(lxQue_Content_s, pthis, mlxQueObj);
    if(lxQueCtx)
    {
        if(lxQueCtx->Head == lxQueCtx->Tail)
        {
            return 0;
        }

        len = bufferlen > lxQueCtx->Head->datalen ? lxQueCtx->Head->datalen : bufferlen;
        memset(outbuffer, 0x00, bufferlen);
        memcpy(outbuffer, lxQueCtx->Head->data, len);
        pNode = lxQueCtx->Head->next;
        lxOSFree(lxQueCtx->Head->data);
        lxOSFree(lxQueCtx->Head);
        lxQueCtx->Head = pNode;
    }

    return len;
}

static BOOLTYPE isEmpty(struct lxque_obj *pthis)
{
    BOOLTYPE boolRet = BOOL_FALSE;
    lxQue_Content_s * lxQueCtx = GET_STRUCT_HEAD_PTR(lxQue_Content_s, pthis, mlxQueObj);
    if(lxQueCtx)
    {
        if(lxQueCtx->Head == lxQueCtx->Tail)
        {
            boolRet =  BOOL_TRUE;
        }
    }

    return boolRet;
}


lxque_obj * lxque_obj_new(void)
{
    lxQue_Content_s * lxQueCtx = (lxQue_Content_s *)lxOSMalloc(sizeof(lxQue_Content_s));
    if(!lxQueCtx)
    {
        return NULL;
    }

    lxQueCtx->mlxQueObj.isEmpty = isEmpty;
    lxQueCtx->mlxQueObj.pop_front = pop_front;
    lxQueCtx->mlxQueObj.push_back = push_back;

    lxQueCtx->Head = lxque_node_init();
    if(!lxQueCtx->Head)
    {
        lxOSFree(lxQueCtx);
        return NULL;
    }

    lxQueCtx->Tail = lxQueCtx->Head;

    return &lxQueCtx->mlxQueObj;
}

void lxque_obj_delete(lxque_obj * pthis)
{
    lxQue_Node_s * pNode = NULL;
    lxQue_Content_s * lxQueCtx = GET_STRUCT_HEAD_PTR(lxQue_Content_s, pthis, mlxQueObj);
    if(lxQueCtx)
    {
        while(lxQueCtx->Head)
        {
            pNode = lxQueCtx->Head->next;
            lxOSFree(lxQueCtx->Head->data);
            lxOSFree(lxQueCtx->Head);
            lxQueCtx->Head = pNode;
        }

        lxOSFree(lxQueCtx);
    }
    return;
}

