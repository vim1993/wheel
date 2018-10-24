#ifndef __LX_MEMERY_POOL_H__
#define __LX_MEMERY_POOL_H__

#include "type.h"

typedef struct lx_memerypool_obj {

    void    (*lxMpInit)(struct lx_memerypool_obj *this, size_t ElementSize);

    VOIDPTR (*lxMpMalloc)(struct lx_memerypool_obj *this, size_t len);
    VOIDPTR (*lxMpRealloc)(struct lx_memerypool_obj *this, void * baseptr, size_t len);
    void    (*lxMpFree)(struct lx_memerypool_obj *this, void *baseptr);

}lx_memerypool_obj;

lx_memerypool_obj * lx_memerypool_obj_new(void);
void lx_memerypool_obj_delete(struct lx_memerypool_obj * this);

#endif