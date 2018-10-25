#ifdef UNIT_TEST
#include <string.h>
#endif

#include "type.h"
#include "lxOs.h"
#include "lxlog.h"
#include "lxStack.h"

typedef struct sequence_stack_node{
    void * data;    //压栈数据
    size_t datalen; //压栈数据大小
}sequence_stack_node;

typedef struct sequence_stack_Context {

    sequence_stack_node * node; //顺序栈
    i32int ElmentNum; //栈空间大小
    i32int ElmentPoint; //当前栈元素位置

    mutex_lock * lock; //互斥锁, pop,push加锁

    lxstack_sequence lxSeqStackOBJ; //顺序栈支持的操作函数指针集合
}sequence_stack_Context;


static BOOLTYPE push_stack(lxstack_sequence * this, const void * data, size_t len)
{
    if(!data || len <= 0)
    {
        return BOOL_FALSE;
    }

    i32int stackindex = -1;
    sequence_stack_Context * seqStackCtx = GET_STRUCT_HEAD_PTR(sequence_stack_Context, this, lxSeqStackOBJ);
    if(seqStackCtx)
    {
        if(seqStackCtx->node == NULL || seqStackCtx->ElmentPoint == (seqStackCtx->ElmentNum -1))
        {
            LOG_DEBUG_PRINT("sequence stack is full\n");
            return BOOL_FALSE;
        }

        seqStackCtx->lock->pthread_resource_lock(seqStackCtx->lock);

        stackindex = ++seqStackCtx->ElmentPoint;
        seqStackCtx->node[stackindex].data = lxOSMalloc(len);
        if(!seqStackCtx->node[stackindex].data)
        {
            seqStackCtx->ElmentPoint--;
            return BOOL_FALSE;
        }
        memcpy(seqStackCtx->node[stackindex].data, data, len);
        seqStackCtx->node[stackindex].datalen = len;

        seqStackCtx->lock->pthread_resource_unlock(seqStackCtx->lock);
    }

    return BOOL_TRUE;
}

static size_t   pop_stack(lxstack_sequence * this, void * outdata, size_t len)
{
    if(!outdata || len <= 0)
    {
        return 0;
    }

    size_t datalen = 0;
    sequence_stack_Context * seqStackCtx = GET_STRUCT_HEAD_PTR(sequence_stack_Context, this, lxSeqStackOBJ);
    if(seqStackCtx)
    {
        if(seqStackCtx->node == NULL || seqStackCtx->ElmentPoint == -1)
        {
            LOG_DEBUG_PRINT("sequence stack is empty\n");
            return 0;
        }

        seqStackCtx->lock->pthread_resource_lock(seqStackCtx->lock);
        datalen = seqStackCtx->node[seqStackCtx->ElmentPoint].datalen > len ? len : seqStackCtx->node[seqStackCtx->ElmentPoint].datalen;

        memcpy(outdata, seqStackCtx->node[seqStackCtx->ElmentPoint].data, datalen);
        lxOSFree(seqStackCtx->node[seqStackCtx->ElmentPoint].data);
        seqStackCtx->node[seqStackCtx->ElmentPoint].datalen = 0;
        seqStackCtx->ElmentPoint--;
        seqStackCtx->lock->pthread_resource_unlock(seqStackCtx->lock);
    }

    return datalen;
}

static BOOLTYPE isEmpty(lxstack_sequence * this)
{
    BOOLTYPE BoolRet = BOOL_FALSE;
    sequence_stack_Context * seqStackCtx = GET_STRUCT_HEAD_PTR(sequence_stack_Context, this, lxSeqStackOBJ);
    if(seqStackCtx)
    {
        if(seqStackCtx->node != NULL && seqStackCtx->ElmentPoint == -1)
        {
            BoolRet = BOOL_TRUE;
        }
    }

    return BoolRet;
}

    //this interfaces are for the sequential stack, and the chain stack returns false
static BOOLTYPE isFull(lxstack_sequence * this)
{
    BOOLTYPE BoolRet = BOOL_TRUE;
    sequence_stack_Context * seqStackCtx = GET_STRUCT_HEAD_PTR(sequence_stack_Context, this, lxSeqStackOBJ);
    if(seqStackCtx)
    {
        //LOG_DEBUG_PRINT("[%d][%d]!\n", seqStackCtx->ElmentPoint, seqStackCtx->ElmentNum);
        if(seqStackCtx->node != NULL && seqStackCtx->ElmentPoint < (seqStackCtx->ElmentNum - 1))
        {
            BoolRet = BOOL_FALSE;
        }
    }

    return BoolRet;
}

    //this interfaces are for the sequential stack, and the chain stack returns true
static BOOLTYPE init(lxstack_sequence * this, u32int ElmentNum)
{
    BOOLTYPE BoolRet = BOOL_FALSE;
    sequence_stack_Context * seqStackCtx = GET_STRUCT_HEAD_PTR(sequence_stack_Context, this, lxSeqStackOBJ);
    if(seqStackCtx)
    {
        seqStackCtx->ElmentNum = ElmentNum;
        seqStackCtx->ElmentPoint = -1;
        seqStackCtx->node = (sequence_stack_node *)lxOSMalloc(sizeof(sequence_stack_node) * ElmentNum);
        if(seqStackCtx->node)
        {
            BoolRet = BOOL_TRUE;
        }
    }

    return BoolRet;
}


lxstack_sequence * lxstack_sequence_new(void)
{
    sequence_stack_Context * seqStackCtx = lxOSMalloc(sizeof(sequence_stack_Context));
    if(!seqStackCtx)
    {
        LOG_DEBUG_PRINT("create sequence_stack_Context failed\n");
        return NULL;
    }

    seqStackCtx->ElmentNum = 0;
    seqStackCtx->ElmentPoint = -1;
    seqStackCtx->node = NULL;
    seqStackCtx->lxSeqStackOBJ.init = init;
    seqStackCtx->lxSeqStackOBJ.isFull = isFull;
    seqStackCtx->lxSeqStackOBJ.isEmpty = isEmpty;
    seqStackCtx->lxSeqStackOBJ.pop_stack = pop_stack;
    seqStackCtx->lxSeqStackOBJ.push_stack = push_stack;

    seqStackCtx->lock = pthread_resource_lock_new();
    if(!seqStackCtx->lock)
    {
        lxOSFree(seqStackCtx);
        return NULL;
    }

    return &seqStackCtx->lxSeqStackOBJ;
}

void lxstack_sequence_delete(lxstack_sequence * this)
{
    sequence_stack_Context * seqStackCtx = GET_STRUCT_HEAD_PTR(sequence_stack_Context, this, lxSeqStackOBJ);
    if(seqStackCtx)
    {
        seqStackCtx->lock->pthread_resource_lock(seqStackCtx->lock);
        while((seqStackCtx->ElmentPoint + 1))
        {
            lxOSFree(seqStackCtx->node[seqStackCtx->ElmentPoint].data);
            seqStackCtx->node[seqStackCtx->ElmentPoint].datalen = 0;
            seqStackCtx->ElmentPoint--;
        }
        lxOSFree(seqStackCtx->node);
        seqStackCtx->lock->pthread_resource_unlock(seqStackCtx->lock);
        pthread_resource_lock_delete(seqStackCtx->lock);
        lxOSFree(seqStackCtx);
    }

    return;
}

#ifdef UNIT_TEST
int lx_sequence_stack_unit_test(void)
{
    int index = 0;
    char buffer[][16] = {
        "hello world",
        "nihao",
        "rtrr"
    };
    char outbuff[3][16] = {0};

    lxstack_sequence * lxSeqStackOBJ = lxstack_sequence_new();
    if(!lxSeqStackOBJ)
    {
        LOG_ERROR_PRINT("new lxStackOBJ failed\n");
        return -1;
    }
    lxSeqStackOBJ->init(lxSeqStackOBJ, 3);

    LOG_DEBUG_PRINT("is full:%d\n", lxSeqStackOBJ->isFull(lxSeqStackOBJ));
    LOG_DEBUG_PRINT("is empty:%d\n", lxSeqStackOBJ->isEmpty(lxSeqStackOBJ));
    for(index = 0; index < 3; index++)
    {
        lxSeqStackOBJ->push_stack(lxSeqStackOBJ, buffer[index], strlen(buffer[index]));
    }
    LOG_DEBUG_PRINT("is full:%d\n", lxSeqStackOBJ->isFull(lxSeqStackOBJ));
    for(index = 0; index < 3; index++)
    {
        lxSeqStackOBJ->pop_stack(lxSeqStackOBJ, outbuff[index], 16);
        LOG_DEBUG_PRINT("pop [%d][%s]\n", index, outbuff[index]);
    }
    LOG_DEBUG_PRINT("is full:%d\n", lxSeqStackOBJ->isFull(lxSeqStackOBJ));
    LOG_DEBUG_PRINT("is empty:%d\n", lxSeqStackOBJ->isEmpty(lxSeqStackOBJ));
    lxstack_sequence_delete(lxSeqStackOBJ);

    return 0;
}
#endif

