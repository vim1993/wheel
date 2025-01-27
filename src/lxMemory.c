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
    lxMemeryBlock_t * Mb = NULL, *PrevMb = NULL, *NextMb = NULL, * breakValue = NULL;
    lxMemeryBlock_Head_t * lxMHead = NULL;
    lxMemeryBlock_Tail_t * lxMTail = NULL;

    Mb = lxMb;

    while(blockSize < size)
    {
        PrevMb = NULL;
        lxMTail = NULL;
        if(Mb != basePtr)
        {
            lxMTail = (lxMemeryBlock_Tail_t *)((char *)Mb - sizeof(lxMemeryBlock_Tail_t));
            LOG_INFO_PRINT("[%p]lxMTail->tag [%d]\n", lxMTail, lxMTail->tag);
        }

        lxMHead = (lxMemeryBlock_Head_t *)((char *)Mb + sizeof(lxMemeryBlock_t) + Mb->mBlockHead.blockSize + sizeof(lxMemeryBlock_Tail_t));
        LOG_INFO_PRINT("lxMHead[%p][%p]\n", ((char *)basePtr + MEMERY_POOL_SIZE), lxMHead);
        if((char *)lxMHead == ((char *)basePtr + MEMERY_POOL_SIZE))
        {
            lxMHead = NULL;
        }
        else
        {
            LOG_INFO_PRINT("lxMHead->tag [%d]\n", lxMHead->tag);
        }

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
            blockSize = PrevMb->mBlockHead.blockSize;
            LOG_INFO_PRINT("Resource blockSize[%d][%d]\n", PrevMb->mBlockHead.blockSize, blockSize);
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
            blockSize = PrevMb->mBlockHead.blockSize;
            LOG_INFO_PRINT("Resource blockSize[%d][%d]\n", blockSize, PrevMb->mBlockHead.blockSize);
        }
        else if(lxMTail != NULL && lxMTail->tag == MEMERYBLOCK_STATUS_FREE)
        {
            PrevMb = lxMTail->uplink;
            LXLIST_DEL_NODE(mLXLOBJ, &Mb->mBlockHead.lxnode);
            PrevMb->mBlockTail = Mb->mBlockTail;
            PrevMb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            PrevMb->mBlockTail->uplink = PrevMb;

            PrevMb->mBlockHead.blockSize = (char *)PrevMb->mBlockTail - (char *)PrevMb->mDataSpace;

            blockSize = PrevMb->mBlockHead.blockSize;
            LOG_INFO_PRINT("Resource blockSize[%d][%d]\n", blockSize, PrevMb->mBlockHead.blockSize);
        }
        else
        {
            blockSize = 0;
        }

        if(PrevMb == NULL)
        {
            LOG_INFO_PRINT("PrevMb == NULL\n");
            PrevMb = (lxMemeryBlock_t *)GET_STRUCT_HEAD_PTR(lxMemeryBlock_Head_t, Mb->mBlockHead.lxnode.Next, lxnode);
            if(breakValue == PrevMb)
            {
                return NULL;
            }
            breakValue = PrevMb;
        }
        Mb = PrevMb;
    }

    LOG_INFO_PRINT("[%p][%p][%p]\n", Mb, Mb->mBlockHead.lxnode.Next, Mb->mBlockHead.lxnode.Prev);
    memset(Mb->mDataSpace, 0x00, Mb->mBlockHead.blockSize);

    return Mb;
}

static VOIDPTR lxmalloc(struct lx_memery_Obj * pthis, size_t size)
{
    if(!pthis || size <= 0)
    {
        return NULL;
    }

    lxMemeryContext * lxMCtx = GET_STRUCT_HEAD_PTR(lxMemeryContext, pthis, mLXMOBJ);
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

        LOG_INFO_PRINT("blockSize:[%d], Head:[%p]\n", lxMHead->blockSize, lxMHead);

        if(lxMHead->blockSize >= size)
        {
            if(lxMHead->blockSize - size > getPageSize())
            {
                NewFreeMB = (lxMemeryBlock_t *)((char *)Mb + sizeof(lxMemeryBlock_t) + size + sizeof(lxMemeryBlock_Tail_t));

                NewFreeMB->mBlockHead.blockSize = (char *)Mb->mBlockTail - (char *)NewFreeMB - sizeof(lxMemeryBlock_t);
                NewFreeMB->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;

                NewFreeMB->mBlockTail = Mb->mBlockTail;

                NewFreeMB->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
                NewFreeMB->mBlockTail->uplink = NewFreeMB;
                NewFreeMB->mDataSpace = (char *)NewFreeMB + sizeof(lxMemeryBlock_t);

                LXLIST_ADD_HEAD(lxMCtx->mLXLOBJ, &NewFreeMB->mBlockHead.lxnode, lxnode);
                lxMCtx->FreeSpace = NewFreeMB;
                Mb->mBlockHead.blockSize = size;
                LOG_INFO_PRINT("[%p][%p]\n", NewFreeMB->mBlockTail, NewFreeMB);
            }
            LXLIST_DEL_NODE(lxMCtx->mLXLOBJ, lxnode);
            break;
        }
        lxnode = lxnode->Next;
        if(lxfirstnode == lxnode)
        {
            Mb = resources_recovery(lxMCtx->mLXLOBJ, lxnode, lxMCtx->basePtr, Mb, size);
            if(Mb == NULL)
            {
                LOG_INFO_PRINT("not find suitable memery block\n");
                PMUTEX_UNLOCK(lxMCtx->mlock);
                return NULL;
            }
            lxnode = &Mb->mBlockHead.lxnode;
        }
    }

    Mb->mBlockHead.malloc_size = size;
    Mb->mBlockHead.tag = MEMERYBLOCK_STATUS_BUSY;

    Mb->mBlockTail = (lxMemeryBlock_Tail_t *)((char *)Mb + sizeof(lxMemeryBlock_t) + Mb->mBlockHead.blockSize);
    Mb->mBlockTail->tag = MEMERYBLOCK_STATUS_BUSY;
    Mb->mBlockTail->uplink = Mb;
    Mb->mDataSpace = (char *)Mb + sizeof(lxMemeryBlock_t);

    LOG_INFO_PRINT("[%p][%p][%d]\n", Mb, Mb->mBlockTail, Mb->mBlockHead.malloc_size);
    PMUTEX_UNLOCK(lxMCtx->mlock);
    return Mb->mDataSpace;
}

static void lxfree(struct lx_memery_Obj * pthis, VOIDPTR basePtr)
{
    if(!pthis || !basePtr)
    {
        return;
    }

    lxMemeryContext * lxMCtx = GET_STRUCT_HEAD_PTR(lxMemeryContext, pthis, mLXMOBJ);
    if(lxMCtx)
    {
        lxMemeryBlock_t * Mb = (lxMemeryBlock_t *)((char *)basePtr - sizeof(lxMemeryBlock_t));
        if(Mb)
        {
            PMUTEX_LOCK(lxMCtx->mlock);
            Mb->mBlockHead.tag = MEMERYBLOCK_STATUS_FREE;
            Mb->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
            memset(Mb->mDataSpace, 0x00, Mb->mBlockHead.malloc_size);
            Mb->mBlockHead.malloc_size = 0;
            LOG_INFO_PRINT("freeAddr:[%p], blockSize:[%d], [%p]\n", basePtr, Mb->mBlockHead.blockSize, Mb);
            if(lxMCtx->FreeSpace->mBlockHead.lxnode.Next == NULL && lxMCtx->FreeSpace->mBlockHead.lxnode.Prev == NULL)
            {
                lxMCtx->FreeSpace = Mb;
                LXLIST_INIT(lxMCtx->mLXLOBJ, &lxMCtx->FreeSpace->mBlockHead.lxnode);
            }
            else
            {
                LXLIST_ADD_TAIL(lxMCtx->mLXLOBJ, &Mb->mBlockHead.lxnode, &lxMCtx->FreeSpace->mBlockHead.lxnode); //add to free list
                lxMCtx->FreeSpace = Mb;
            }
            PMUTEX_UNLOCK(lxMCtx->mlock);
        }
    }

    return;
}

static VOIDPTR lxrealloc(struct lx_memery_Obj * pthis, VOIDPTR basePtr, size_t size)
{
    if(!basePtr || size <= 0)
    {
        return basePtr;
    }

    size_t oldsize = 0;
    lxMemeryBlock_t * Mb = (lxMemeryBlock_t *)((char *)basePtr - sizeof(lxMemeryBlock_t));
    LOG_INFO_PRINT("blockSize:[%d], malloc_size:[%d]\n", Mb->mBlockHead.blockSize, Mb->mBlockHead.malloc_size);

    oldsize = Mb->mBlockHead.malloc_size;

    if(Mb->mBlockHead.blockSize >= size)
    {
        if(Mb->mBlockHead.malloc_size < size)
        {
            Mb->mBlockHead.malloc_size = size;
        }
        return basePtr;
    }

    void * ptr = lxmalloc(pthis, size);
    if(ptr)
    {
        memcpy(ptr, basePtr, oldsize);
        lxfree(pthis, basePtr);
    }

    return ptr;
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

    lxMCtx->FreeSpace->mBlockTail = (lxMemeryBlock_Tail_t *)((char *)lxMCtx->FreeSpace + MEMERY_POOL_SIZE - sizeof(lxMemeryBlock_Tail_t));
    LOG_INFO_PRINT("[%p][%p]\n", lxMCtx->FreeSpace->mBlockTail, (char *)lxMCtx->FreeSpace + MEMERY_POOL_SIZE);
    lxMCtx->FreeSpace->mBlockTail->tag = MEMERYBLOCK_STATUS_FREE;
    lxMCtx->FreeSpace->mBlockTail->uplink = lxMCtx->FreeSpace;
    lxMCtx->FreeSpace->mDataSpace = (char *)lxMCtx->FreeSpace + sizeof(lxMemeryBlock_t);

    return &lxMCtx->mLXMOBJ;
}

void lx_memery_Obj_delete(lx_memery_Obj * pthis)
{
    lxMemeryContext * lxMCtx = GET_STRUCT_HEAD_PTR(lxMemeryContext, pthis, mLXMOBJ);
    if(lxMCtx)
    {
        lxOSFree(lxMCtx->basePtr);
        lxlist_Obj_delete(lxMCtx->mLXLOBJ);
        pthread_resource_lock_delete(lxMCtx->mlock);
        lxOSFree(lxMCtx);
    }

    return;
}

#ifdef UNIT_TEST
u32int lxmemory_unit_test(void)
{
    lx_memery_Obj * pthis = New(lx_memery_Obj);
    if(!pthis)
    {
        LOG_ERROR_PRINT("create lx_memery_Obj failed\n");
        return -1;
    }

    #if 1
    void * data2 = MALLOC(pthis, 200);

    if(data2 != NULL)
    {
        strcpy(data2, "hello world\n");
        LOG_DEBUG_PRINT("data2:%p, %s\n", data2, data2);
    }
    data2 = REALLOC(pthis, data2, 500);
    LOG_DEBUG_PRINT("data2:%p, %s\n", data2, data2);
    FREE(pthis, data2);
    #else
    void * data1 = MALLOC(pthis, 200);
    strcpy(data1, "hello world\n");
    LOG_DEBUG_PRINT("data2:%p, %s\n", data1, data1);
    FREE(pthis, data1);
    void * data2 = MALLOC(pthis, 200);
    strcpy(data2, "ssssss\n");
    LOG_DEBUG_PRINT("data2:%p, %s\n", data2, data2);
    FREE(pthis, data2);
    #endif
    DELETE(lx_memery_Obj, pthis);
}

#endif
