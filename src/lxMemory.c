/*
 *author:zheng.yi
 *email:vim1993@163.com
 *version:V1.0
 */
#include <string.h>

#include "lxMemory.h"
#include "lxOs.h"
#include "type.h"
#include "lxlog.h"
#include "lxList.h"

typedef enum {
    MEMERYBLOCK_STATUS_BUSY,
    MEMERYBLOCK_STATUS_FREE
}MEMERYBLOCK_STATUS_E;

typedef struct lxMemeryBlock_t lxMemeryBlock_t;

typedef struct lxMemeryBlock_Head_t {
    struct lxlist_node lxnode;
    size_t blockSize;
    size_t malloc_size;
    MEMERYBLOCK_STATUS_E  tag;
}lxMemeryBlock_Head_t;

typedef struct lxMemeryBlock_Tail_t {
    lxMemeryBlock_t * uplink;
    MEMERYBLOCK_STATUS_E tag;
}lxMemeryBlock_Tail_t;

struct lxMemeryBlock_t {
    lxMemeryBlock_Head_t mBlockHead;
    void * mDataSpace;
    lxMemeryBlock_Tail_t * mBlockTail;
};

typedef struct lxMemeryContext{

    void * basePtr;
    lxMemeryBlock_t * FreeSpace;
    lxlist_Obj * mLXLOBJ;
    lx_memery_Obj mLXMOBJ;

}lxMemeryContext;

static size_t getPageSize(void)
{
    return 128;
}

static void resources_recovery(const lxlist_Obj * mLXLOBJ, struct lxlist_node * lxnode, const void * basePtr, lxMemeryBlock_t * lxMb, size_t size)
{
    if(!mLXLOBJ || !lxnode || !lxMb)
    {
        return;
    }

    size_t blockSize = 0;
    lxMemeryBlock_t * Mb = NULL, *PrevMb = NULL, *NextMb = NULL;
    lxMemeryBlock_Head_t * lxMHead = NULL;
    lxMemeryBlock_Tail_t * lxMTail = NULL;

    Mb = lxMb;
    if(Mb == basePtr)
    {
        lxMHead = GET_STRUCT_HEAD_PTR(lxMemeryBlock_Head_t, lxMb->mBlockHead.lxnode.Next, lxnode);
        Mb = GET_STRUCT_HEAD_PTR(lxMemeryBlock_t, lxMHead, mBlockHead);
    }

    while(blockSize < size)
    {
        lxMTail = (char *)Mb - sizeof(lxMemeryBlock_Tail_t);
        if(lxMTail->tag == MEMERYBLOCK_STATUS_FREE)
        {
            PrevMb = lxMTail->uplink;
            LXLIST_DEL_NODE(mLXLOBJ, &Mb->mBlockHead.lxnode);
            PrevMb->mBlockTail = Mb->mBlockTail;
            PrevMb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            PrevMb->mBlockTail->uplink = PrevMb;

            PrevMb->mBlockHead.blockSize = PrevMb->mBlockHead.blockSize + sizeof(lxMemeryBlock_Tail_t) + sizeof(lxMemeryBlock_t) + Mb->mBlockHead.blockSize;
            LOG_INFO_PRINT("Resource blockSize[%d]\n", PrevMb->mBlockHead.blockSize);
            blockSize = PrevMb->mBlockHead.blockSize;
        }
        Mb = PrevMb;
    }

    return;
}

static VOIDPTR lxmalloc(struct lx_memery_Obj * this, size_t size)
{
    lxMemeryContext * lxMCtx = GET_STRUCT_HEAD_PTR(lxMemeryContext, this, mLXMOBJ);
    if(!lxMCtx)
    {
        return NULL;
    }

    size_t minSize = 0;
    lxMemeryBlock_t * Mb = NULL, * NewFreeMB = NULL;
    struct lxlist_node * lxnode = NULL, * lxfirstnode = NULL;
    lxMemeryBlock_Head_t * lxMHead = NULL;
    lxnode = &lxMCtx->FreeSpace->mBlockHead.lxnode;
    lxfirstnode = lxnode;
    while(lxnode)
    {
        lxMHead = GET_STRUCT_HEAD_PTR(lxMemeryBlock_Head_t, lxnode, lxnode);
        Mb = GET_STRUCT_HEAD_PTR(lxMemeryBlock_t, lxMHead, mBlockHead);
        if(!Mb)
        {
            return NULL;
        }

        LOG_INFO_PRINT("blockSize:[%d], Head:[%p]\n", lxMHead->blockSize, lxMHead);

        if(lxMHead->blockSize >= size)
        {
            if(lxMHead->blockSize - size > getPageSize())
            {
                NewFreeMB = (char *)Mb + sizeof(lxMemeryBlock_t) + size + sizeof(lxMemeryBlock_Tail_t);

                NewFreeMB->mBlockHead.blockSize = (char *)Mb->mBlockTail - (char *)NewFreeMB - sizeof(lxMemeryBlock_Head_t);
                NewFreeMB->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;

                NewFreeMB->mBlockTail = Mb->mBlockTail;
                NewFreeMB->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
                NewFreeMB->mBlockTail->uplink = NewFreeMB;
                NewFreeMB->mDataSpace = (char *)NewFreeMB + sizeof(lxMemeryBlock_t);

                LXLIST_ADD_HEAD(lxMCtx->mLXLOBJ, &NewFreeMB->mBlockHead.lxnode, lxnode);
                lxMCtx->FreeSpace = NewFreeMB;
                Mb->mBlockHead.blockSize = size;
            }
            LXLIST_DEL_NODE(lxMCtx->mLXLOBJ, lxnode);
            break;
        }

        lxnode = lxnode->Next;
        if(lxfirstnode == lxnode)
        {
            resources_recovery(lxMCtx->mLXLOBJ, lxnode, lxMCtx->basePtr, Mb, size);
        }
    }

    Mb->mBlockHead.malloc_size = size;
    Mb->mBlockHead.tag = MEMERYBLOCK_STATUS_BUSY;

    Mb->mBlockTail = (char *)Mb + sizeof(lxMemeryBlock_t) + size;
    Mb->mBlockTail->tag = MEMERYBLOCK_STATUS_BUSY;
    Mb->mBlockTail->uplink = Mb;
    Mb->mDataSpace = (char *)Mb + sizeof(lxMemeryBlock_t);

    LOG_INFO_PRINT("bs:[%d],MS:[%d], addr:[%p]\n", Mb->mBlockHead.blockSize, Mb->mBlockHead.malloc_size, Mb->mDataSpace);

    return Mb->mDataSpace;
}

static void lxfree(struct lx_memery_Obj * this, VOIDPTR basePtr)
{

    lxMemeryContext * lxMCtx = GET_STRUCT_HEAD_PTR(lxMemeryContext, this, mLXMOBJ);
    if(lxMCtx)
    {
        lxMemeryBlock_t * Mb = (char *)basePtr - sizeof(lxMemeryBlock_t);

        if(Mb)
        {
            Mb->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;
            Mb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            memset(Mb->mDataSpace, 0x00, Mb->mBlockHead.malloc_size);
            Mb->mBlockHead.malloc_size = 0;
            LOG_INFO_PRINT("freeAddr:[%p], blockSize:[%d]\n", basePtr, Mb->mBlockHead.blockSize);
            if(lxMCtx->FreeSpace->mBlockHead.lxnode.Next == NULL && lxMCtx->FreeSpace->mBlockHead.lxnode.Prev == NULL)
            {
                lxMCtx->FreeSpace = Mb;
                LXLIST_INIT(lxMCtx->mLXLOBJ, &lxMCtx->FreeSpace->mBlockHead.lxnode);
            }
            else
            {
                LXLIST_ADD_TAIL(lxMCtx->mLXLOBJ, &Mb->mBlockHead.lxnode, &lxMCtx->FreeSpace->mBlockHead.lxnode); //add to free list
            }
        }
    }

    return;
}

static VOIDPTR lxrealloc(struct lx_memery_Obj * this, VOIDPTR basePtr, size_t size)
{
    if(!basePtr || size <= 0)
    {
        return basePtr;
    }

    lxMemeryBlock_t * Mb = (char *)basePtr - sizeof(lxMemeryBlock_t);
    LOG_INFO_PRINT("blockSize:[%d], malloc_size:[%d]\n", Mb->mBlockHead.blockSize, Mb->mBlockHead.malloc_size);
    if(Mb->mBlockHead.blockSize >= size)
    {
        if(Mb->mBlockHead.malloc_size < size)
        {
            Mb->mBlockHead.malloc_size = size;
        }

        return basePtr;
    }

    lxfree(this, basePtr);

    return lxmalloc(this, size);
}


lx_memery_Obj * lx_memery_Obj_new()
{
    lxMemeryContext * lxMCtx = lxOSMalloc(sizeof(lxMemeryContext));
    if(!lxMCtx)
    {
        LOG_ERROR_PRINT("init lxMemeryContext manager faild\n");
        return NULL;
    }

    lxMCtx->basePtr = lxOSMalloc(MEMERY_POOL_SIZE);
    if(!lxMCtx->basePtr)
    {
        lxOSFree(lxMCtx);
        return NULL;
    }

    lxMCtx->mLXLOBJ = lxlist_Obj_new();
    if(!lxMCtx->mLXLOBJ)
    {
        lxOSFree(lxMCtx->basePtr);
        lxOSFree(lxMCtx);
        return NULL;
    }
    lxMCtx->mLXMOBJ.lxfree = lxfree;
    lxMCtx->mLXMOBJ.lxmalloc = lxmalloc;
    lxMCtx->mLXMOBJ.lxrealloc = lxrealloc;

    lxMCtx->FreeSpace = lxMCtx->basePtr;
    lxMCtx->FreeSpace->mBlockHead.blockSize = MEMERY_POOL_SIZE - sizeof(lxMemeryBlock_t) - sizeof(lxMemeryBlock_Tail_t);
    lxMCtx->FreeSpace->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;
    LXLIST_INIT(lxMCtx->mLXLOBJ, &lxMCtx->FreeSpace->mBlockHead.lxnode);
    lxMCtx->FreeSpace->mBlockTail = (char *)lxMCtx->FreeSpace + MEMERY_POOL_SIZE - sizeof(lxMemeryBlock_Tail_t);
    lxMCtx->FreeSpace->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
    lxMCtx->FreeSpace->mBlockTail->uplink = lxMCtx->FreeSpace;
    lxMCtx->FreeSpace->mDataSpace = (char *)lxMCtx->FreeSpace + sizeof(lxMemeryBlock_t);

    return &lxMCtx->mLXMOBJ;
}

void lx_memery_Obj_delete(lx_memery_Obj * this)
{
    lxMemeryContext * lxMCtx = GET_STRUCT_HEAD_PTR(lxMemeryContext, this, mLXMOBJ);
    if(lxMCtx)
    {
        lxOSFree(lxMCtx->basePtr);
        lxlist_Obj_delete(lxMCtx->mLXLOBJ);
        lxOSFree(lxMCtx);
    }

    return;
}
