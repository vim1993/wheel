#include "type.h"
#include "lxlog.h"
#include "lxOs.h"
#include "lxMemoryPool.h"

typedef struct lxMemery_Context_s {



    lx_memerypool_obj lxPublicObj;

}lxMemery_Context_s;

static void lxMpInit(struct lx_memerypool_obj *this, size_t ElementSize)
{
    return;
}

static VOIDPTR lxMpMalloc(struct lx_memerypool_obj *this, size_t len)
{

    return NULL;
}

static VOIDPTR lxMpRealloc(struct lx_memerypool_obj *this, void * baseptr, size_t len)
{

    return NULL;
}

static void lxMpFree(struct lx_memerypool_obj *this, void *baseptr)
{

    return;
}

lx_memerypool_obj * lx_memerypool_obj_new(void)
{
    lxMemery_Context_s * lxMemeryCtx = (lxMemery_Context_s *)lxOSMalloc(sizeof(lxMemery_Context_s));
    if(!lxMemeryCtx)
    {
        return NULL;
    }

    lxMemeryCtx->lxPublicObj.lxMpInit = lxMpInit;
    lxMemeryCtx->lxPublicObj.lxMpMalloc = lxMpMalloc;
    lxMemeryCtx->lxPublicObj.lxMpRealloc = lxMpRealloc;
    lxMemeryCtx->lxPublicObj.lxMpFree = lxMpFree;

    return &lxMemeryCtx->lxPublicObj;
}

void lx_memerypool_obj_delete(struct lx_memerypool_obj * this)
{

    return;
}

