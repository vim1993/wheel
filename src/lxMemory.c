/*
 *author:zheng.yi
 *email:vim1993@163.com
 *version:V1.0
 */
#include <stdio.h>
#include <stdlib.h>
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
    mutex_lock * mlock;
    lxlist_Obj * mLXLOBJ;
    lx_memery_Obj mLXMOBJ;

}lxMemeryContext;

static size_t getPageSize(void)
{
    #ifdef UNIT_TEST
    return 128;
    #else
    return 512;
    #endif
}

static lxMemeryBlock_t * resources_recovery(const lxlist_Obj * mLXLOBJ, struct lxlist_node * lxnode, const void * basePtr, lxMemeryBlock_t * lxMb, size_t size)
{
    if(!mLXLOBJ || !lxnode || !lxMb)
    {
        return NULL;
    }

    size_t blockSize = 0;
    lxMemeryBlock_t * Mb = NULL, *PrevMb = NULL, *NextMb = NULL;
    lxMemeryBlock_Head_t * lxMHead = NULL;
    lxMemeryBlock_Tail_t * lxMTail = NULL;

    Mb = lxMb;

    while(blockSize < size)
    {
        lxMTail = NULL;
        if(Mb != basePtr)
        {
            lxMTail = (char *)Mb - sizeof(lxMemeryBlock_Tail_t);
        }

        lxMHead = (char *)Mb + sizeof(lxMemeryBlock_t) + Mb->mBlockHead.blockSize + sizeof(lxMemeryBlock_Tail_t);
        LOG_ERROR_PRINT("lxMHead[%p][%p][%p]\n", ((char *)basePtr + MEMERY_POOL_SIZE), lxMHead, ((lxMemeryBlock_t *)lxMHead)->mBlockTail);
        if((char *)((lxMemeryBlock_t *)lxMHead)->mBlockTail + sizeof(lxMemeryBlock_Tail_t) > ((char *)basePtr + MEMERY_POOL_SIZE))
        {
            lxMHead = NULL;
        }

        LOG_ERROR_PRINT("[%d][%d]\n", lxMHead->tag, lxMTail->tag);

        if(lxMTail != NULL && lxMHead != NULL && lxMTail->tag == MEMERYBLOCK_STATUS_FREE && lxMHead->tag == MEMERYBLOCK_STATUS_FREE)
        {
            PrevMb = lxMTail->uplink;
            NextMb = (lxMemeryBlock_t *)lxMHead;

            PrevMb->mBlockTail = NextMb->mBlockTail;

            LXLIST_DEL_NODE(mLXLOBJ, &Mb->mBlockHead.lxnode);
            LXLIST_DEL_NODE(mLXLOBJ, &NextMb->mBlockHead.lxnode);
            PrevMb->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;
            PrevMb->mBlockHead.blockSize = (char *)PrevMb->mBlockTail - (char *)PrevMb->mDataSpace;
            PrevMb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            PrevMb->mBlockTail->uplink = PrevMb;
            blockSize += PrevMb->mBlockHead.blockSize;
            LOG_ERROR_PRINT("Resource blockSize[%d]\n", PrevMb->mBlockHead.blockSize);
        }
        else if(lxMHead != NULL && lxMHead->tag == MEMERYBLOCK_STATUS_FREE)
        {
            NextMb = (lxMemeryBlock_t *)lxMHead;
            PrevMb = Mb;
            LXLIST_DEL_NODE(mLXLOBJ, &NextMb->mBlockHead.lxnode);
            PrevMb->mBlockTail = NextMb->mBlockTail;
            PrevMb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            PrevMb->mBlockTail->uplink = PrevMb;
            PrevMb->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;
            PrevMb->mBlockHead.blockSize = (char *)PrevMb->mBlockTail - (char *)PrevMb->mDataSpace;
            blockSize += PrevMb->mBlockHead.blockSize;
            LOG_ERROR_PRINT("Resource blockSize[%d]\n", blockSize);

        }
        else if(lxMTail != NULL && lxMTail->tag == MEMERYBLOCK_STATUS_FREE)
        {
            PrevMb = lxMTail->uplink;
            LXLIST_DEL_NODE(mLXLOBJ, &Mb->mBlockHead.lxnode);
            PrevMb->mBlockTail = Mb->mBlockTail;
            PrevMb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            PrevMb->mBlockTail->uplink = PrevMb;

            PrevMb->mBlockHead.blockSize = (char *)PrevMb->mBlockTail - (char *)PrevMb->mDataSpace;

            blockSize += PrevMb->mBlockHead.blockSize;
            LOG_ERROR_PRINT("Resource blockSize[%d]\n", blockSize);
        }
        else
        {
            blockSize = 0;
        }

        Mb = PrevMb;
    }

    LOG_ERROR_PRINT("[%p][%p][%p]\n", Mb, Mb->mBlockHead.lxnode.Next, Mb->mBlockHead.lxnode.Prev);
    memset(Mb->mDataSpace, 0x00, Mb->mBlockHead.blockSize);

    return Mb;
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
    PMUTEX_LOCK(lxMCtx->mlock);
    lxnode = &lxMCtx->FreeSpace->mBlockHead.lxnode;
    lxfirstnode = lxnode;
    while(lxnode)
    {
        lxMHead = GET_STRUCT_HEAD_PTR(lxMemeryBlock_Head_t, lxnode, lxnode);
        Mb = GET_STRUCT_HEAD_PTR(lxMemeryBlock_t, lxMHead, mBlockHead);
        if(!Mb)
        {
            PMUTEX_UNLOCK(lxMCtx->mlock);
            return NULL;
        }

        LOG_ERROR_PRINT("blockSize:[%d], Head:[%p]\n", lxMHead->blockSize, lxMHead);

        if(lxMHead->blockSize >= size)
        {
            if(lxMHead->blockSize - size > getPageSize())
            {
                NewFreeMB = (char *)Mb + sizeof(lxMemeryBlock_t) + size + sizeof(lxMemeryBlock_Tail_t);

                NewFreeMB->mBlockHead.blockSize = (char *)Mb->mBlockTail - (char *)NewFreeMB - sizeof(lxMemeryBlock_t);
                NewFreeMB->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;

                NewFreeMB->mBlockTail = Mb->mBlockTail;

                NewFreeMB->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
                NewFreeMB->mBlockTail->uplink = NewFreeMB;
                NewFreeMB->mDataSpace = (char *)NewFreeMB + sizeof(lxMemeryBlock_t);

                LXLIST_ADD_HEAD(lxMCtx->mLXLOBJ, &NewFreeMB->mBlockHead.lxnode, lxnode);
                lxMCtx->FreeSpace = NewFreeMB;
                Mb->mBlockHead.blockSize = size;
                LOG_ERROR_PRINT("[%p][%p]\n", NewFreeMB->mBlockTail, NewFreeMB);
            }
            LXLIST_DEL_NODE(lxMCtx->mLXLOBJ, lxnode);
            break;
        }
        lxnode = lxnode->Next;
        if(lxfirstnode == lxnode)
        {
            Mb = resources_recovery(lxMCtx->mLXLOBJ, lxnode, lxMCtx->basePtr, Mb, size);
            lxnode = &Mb->mBlockHead.lxnode;
        }
    }

    Mb->mBlockHead.malloc_size = size;
    Mb->mBlockHead.tag = MEMERYBLOCK_STATUS_BUSY;

    Mb->mBlockTail = (char *)Mb + sizeof(lxMemeryBlock_t) + Mb->mBlockHead.blockSize;
    Mb->mBlockTail->tag = MEMERYBLOCK_STATUS_BUSY;
    Mb->mBlockTail->uplink = Mb;
    Mb->mDataSpace = (char *)Mb + sizeof(lxMemeryBlock_t);

    LOG_ERROR_PRINT("[%p][%p]\n", Mb, Mb->mBlockTail);
    PMUTEX_UNLOCK(lxMCtx->mlock);
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
            PMUTEX_LOCK(lxMCtx->mlock);
            Mb->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;
            Mb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            memset(Mb->mDataSpace, 0x00, Mb->mBlockHead.malloc_size);
            Mb->mBlockHead.malloc_size = 0;
            LOG_ERROR_PRINT("freeAddr:[%p], blockSize:[%d], [%p]\n", basePtr, Mb->mBlockHead.blockSize, Mb);
            if(lxMCtx->FreeSpace->mBlockHead.lxnode.Next == NULL && lxMCtx->FreeSpace->mBlockHead.lxnode.Prev == NULL)
            {
                lxMCtx->FreeSpace = Mb;
                LXLIST_INIT(lxMCtx->mLXLOBJ, &lxMCtx->FreeSpace->mBlockHead.lxnode);
            }
            else
            {
                LXLIST_ADD_TAIL(lxMCtx->mLXLOBJ, &Mb->mBlockHead.lxnode, &lxMCtx->FreeSpace->mBlockHead.lxnode); //add to free list
            }
            PMUTEX_UNLOCK(lxMCtx->mlock);
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
    LOG_ERROR_PRINT("blockSize:[%d], malloc_size:[%d]\n", Mb->mBlockHead.blockSize, Mb->mBlockHead.malloc_size);

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

    lxMCtx->mlock = pthread_resource_lock_new();
    if(!lxMCtx->mlock)
    {
        lxOSFree(lxMCtx->basePtr);
        lxOSFree(lxMCtx);
        lxlist_Obj_delete(lxMCtx->mLXLOBJ);
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
    LOG_ERROR_PRINT("[%p][%p]\n", lxMCtx->FreeSpace->mBlockTail, (char *)lxMCtx->FreeSpace + MEMERY_POOL_SIZE);
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

#ifdef UNIT_TEST
u32int lxmemory_unit_test(void)
{
    lx_memery_Obj * this = NEW(lx_memery_Obj);
    if(!this)
    {
        LOG_ERROR_PRINT("create lx_memery_Obj failed\n");
        return -1;
    }


    void * data = MALLOC(this, 200);
    void * data1 = MALLOC(this, 200);
    void * data2 = MALLOC(this, 400);
    FREE(this, data);
    FREE(this, data2);
    FREE(this, data1);
    void * data4 = MALLOC(this, 600);
    DELETE(lx_memery_Obj, this);
}

#endif
