#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "lxOs.h"
#include "lxlog.h"
#include "lxStack.h"

typedef struct lxStackNode {
    void * data;
    size_t datalen;
    struct lxStackNode * next;
}lxStackNode;

typedef struct lxStackContext {

    lxStackNode * Head;
    mutex_lock  * _lock;
    lxstack_obj mSatckObj;

}lxStackContext;

static void lxstack_node_deinit(lxStackNode * node)
{
    if(node)
    {
        lxOSFree(node->data);
        lxOSFree(node);
    }

    return;
}

static lxStackNode * lxstack_node_init(const void * data, size_t len)
{
    lxStackNode * node = (lxStackNode *)lxOSMalloc(sizeof(lxStackNode));
    if(node)
    {
        node->data = lxOSMalloc(len);
        if(!node->data)
        {
            lxOSFree(node);
            return NULL;
        }
        node->datalen = len;
        memcpy(node->data, data, len);
        node->next = NULL;
    }

    return node;
}

static BOOLTYPE push_stack(struct lxstack_obj * pthis, const void * data, size_t len)
{
    if(!data || len <= 0)
    {
        LOG_ERROR_PRINT("param error ,please check\n");
        return BOOL_FALSE;
    }

    lxStackNode * plxStackNode = NULL;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, pthis, mSatckObj);
    if(plxStackCtx)
    {
        plxStackNode = lxstack_node_init(data, len);
        if(!plxStackNode)
        {
            LOG_DEBUG_PRINT("plxStackNode init failed\n");
            return BOOL_FALSE;
        }
        plxStackCtx->_lock->pthread_resource_lock(plxStackCtx->_lock);
        plxStackNode->next = plxStackCtx->Head;
        plxStackCtx->Head = plxStackNode;
        plxStackCtx->_lock->pthread_resource_unlock(plxStackCtx->_lock);
        return BOOL_TRUE;
    }

    return BOOL_FALSE;
}

static size_t  pop_stack(struct lxstack_obj * pthis, void * outdata, size_t len)
{
    if(!outdata || len <= 0)
    {
        LOG_ERROR_PRINT("param error ,please check\n");
        return 0;
    }

    size_t datalen = 0;
    lxStackNode * plxStackNode = NULL;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, pthis, mSatckObj);
    if(plxStackCtx)
    {
        if(plxStackCtx->Head == NULL)
        {
            LOG_DEBUG_PRINT("stack is empty\n");
            return 0;
        }

        plxStackCtx->_lock->pthread_resource_lock(plxStackCtx->_lock);
        plxStackNode = plxStackCtx->Head->next;
        datalen = len > plxStackCtx->Head->datalen ? plxStackCtx->Head->datalen : len;
        memset(outdata, 0x00, len);
        memcpy(outdata, plxStackCtx->Head->data, datalen);
        lxstack_node_deinit(plxStackCtx->Head);
        plxStackCtx->Head = plxStackNode;
        plxStackCtx->_lock->pthread_resource_unlock(plxStackCtx->_lock);
    }

    return datalen;
}

static BOOLTYPE isEmpty(struct lxstack_obj * pthis)
{
    BOOLTYPE boolRet = BOOL_FALSE;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, pthis, mSatckObj);
    if(plxStackCtx)
    {
        if(plxStackCtx->Head == NULL)
        {
            boolRet = BOOL_TRUE;
        }
    }

    return boolRet;
}

//pthis interfaces are for the sequential stack, and the chain stack returns false
static BOOLTYPE isFull(struct lxstack_obj * pthis)
{
    return BOOL_FALSE;
}

//pthis interfaces are for the sequential stack, and the chain stack returns true
static BOOLTYPE init(struct lxstack_obj * pthis, u32int ElmentNum)
{
    return BOOL_TRUE;
}

lxstack_obj * lxstack_obj_new(void)
{
    lxStackContext * plxStackCtx = (lxStackContext *)lxOSMalloc(sizeof(lxStackContext));
    if(!plxStackCtx)
    {
        return NULL;
    }

    plxStackCtx->mSatckObj.isEmpty = isEmpty;
    plxStackCtx->mSatckObj.pop_stack = pop_stack;
    plxStackCtx->mSatckObj.push_stack = push_stack;
    plxStackCtx->mSatckObj.init = init;
    plxStackCtx->mSatckObj.isFull = isFull;
    plxStackCtx->_lock = pthread_resource_lock_new();
    if(!plxStackCtx->_lock)
    {
        lxOSFree(plxStackCtx);
        return NULL;
    }

    return &plxStackCtx->mSatckObj;
}

void lxstack_obj_delete(lxstack_obj * pthis)
{
    lxStackNode * plxStackNode = NULL;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, pthis, mSatckObj);

    if(plxStackCtx)
    {
        while(plxStackCtx->Head)
        {
            plxStackNode = plxStackCtx->Head->next;
            lxOSFree(plxStackCtx->Head->data);
            lxOSFree(plxStackCtx->Head);

            plxStackCtx->Head = plxStackNode;
        }

        pthread_resource_lock_delete(plxStackCtx->_lock);
        lxOSFree(plxStackCtx);
    }

    return;
}

#ifdef UNIT_TEST
void * lxstack_unit_test_proc(void * param)
{
    int index = 0;

    char outbuff[3][16] = {0};

    lxstack_obj * lxStackOBJ = (lxstack_obj *)param;
    if(!lxStackOBJ || lxStackOBJ->pop_stack == NULL)
    {
        LOG_ERROR_PRINT("new lxStackOBJ failed\n");
        return NULL;
    }
    //LOG_DEBUG_PRINT("is empty:%d\n", lxStackOBJ->isEmpty(lxStackOBJ));

    for(index = 0; index < 3; index++)
    {
        lxStackOBJ->pop_stack(lxStackOBJ, outbuff[index], 16);
        LOG_DEBUG_PRINT("pop [%d][%s]\n", index, outbuff[index]);
    }

    return NULL;
}


int lxstack_unit_test(void)
{
    int index = 0;
    pthread_t tt;
    char buffer[][16] = {
        "hello world",
        "nihao",
        "rtrr"
    };

    lxstack_obj * lxStackOBJ = lxstack_obj_new();
    if(!lxStackOBJ)
    {
        LOG_ERROR_PRINT("new lxStackOBJ failed\n");
        return -1;
    }

    pthread_create(&tt, NULL, lxstack_unit_test_proc, lxStackOBJ);
    //LOG_DEBUG_PRINT("is empty:%d\n", lxStackOBJ->isEmpty(lxStackOBJ));
    for(index = 0; index < 3; index++)
    {
        lxStackOBJ->push_stack(lxStackOBJ, buffer[index], strlen(buffer[index]));
        LOG_DEBUG_PRINT("push:%d,%s\n", index, buffer[index]);
    }

    pthread_join(tt, NULL);

    lxstack_obj_delete(lxStackOBJ);

    return 0;
}
#endif