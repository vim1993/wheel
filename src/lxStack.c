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

static BOOLTYPE push_stack(struct lxstack_obj * this, const void * data, size_t len)
{
    if(!data || len <= 0)
    {
        LOG_ERROR_PRINT("param error ,please check\n");
        return BOOL_FALSE;
    }

    lxStackNode * plxStackNode = NULL;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, this, mSatckObj);
    if(plxStackCtx)
    {
        plxStackNode = lxstack_node_init(data, len);
        if(!plxStackNode)
        {
            LOG_DEBUG_PRINT("plxStackNode init failed\n");
            return BOOL_FALSE;
        }

        plxStackNode->next = plxStackCtx->Head;
        plxStackCtx->Head = plxStackNode;

        return BOOL_TRUE;
    }

    return BOOL_FALSE;
}

static size_t  pop_stack(struct lxstack_obj * this, void * outdata, size_t len)
{
    if(!outdata || len <= 0)
    {
        LOG_ERROR_PRINT("param error ,please check\n");
        return 0;
    }

    size_t datalen = 0;
    lxStackNode * plxStackNode = NULL;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, this, mSatckObj);
    if(plxStackCtx)
    {
        if(plxStackCtx->Head == NULL)
        {
            LOG_DEBUG_PRINT("stack is empty\n");
            return 0;
        }

        plxStackNode = plxStackCtx->Head->next;
        datalen = len > plxStackCtx->Head->datalen ? plxStackCtx->Head->datalen : len;
        memset(outdata, 0x00, len);
        memcpy(outdata, plxStackCtx->Head->data, datalen);
        lxstack_node_deinit(plxStackCtx->Head);
        plxStackCtx->Head = plxStackNode;
    }

    return datalen;
}

static BOOLTYPE isEmpty(struct lxstack_obj * this)
{
    BOOLTYPE boolRet = BOOL_FALSE;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, this, mSatckObj);
    if(plxStackCtx)
    {
        if(plxStackCtx->Head == NULL)
        {
            boolRet = BOOL_TRUE;
        }
    }

    return boolRet;
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

    return &plxStackCtx->mSatckObj;
}

void lxstack_obj_delete(lxstack_obj * this)
{
    lxStackNode * plxStackNode = NULL;
    lxStackContext * plxStackCtx = GET_STRUCT_HEAD_PTR(lxStackContext, this, mSatckObj);
    if(plxStackCtx)
    {
        while(plxStackCtx->Head)
        {
            plxStackNode = plxStackCtx->Head->next;
            lxOSFree(plxStackCtx->Head->data);
            lxOSFree(plxStackCtx->Head);

            plxStackCtx->Head = plxStackNode;
        }

        lxOSFree(plxStackCtx);
    }

    return;
}

int lxstack_unit_test(void)
{
    int index = 0;

    char buffer[][16] = {
        "hello world",
        "nihao",
        "rtrr"
    };
    char outbuff[3][16] = {0};

    lxstack_obj * lxStackOBJ = lxstack_obj_new();
    if(!lxStackOBJ)
    {
        LOG_ERROR_PRINT("new lxStackOBJ failed\n");
        return -1;
    }
    LOG_DEBUG_PRINT("is empty:%d\n", lxStackOBJ->isEmpty(lxStackOBJ));
    for(index = 0; index < 3; index++)
    {
        lxStackOBJ->push_stack(lxStackOBJ, buffer[index], strlen(buffer[index]));
    }

    for(index = 0; index < 3; index++)
    {
        lxStackOBJ->pop_stack(lxStackOBJ, outbuff[index], 16);
        LOG_DEBUG_PRINT("[%d][%s]\n", index, outbuff[index]);
    }

    lxstack_obj_delete(lxStackOBJ);

    return 0;
}

