#ifndef __LX_MEMERY_H__
#define __LX_MEMERY_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "type.h"

#ifdef UNIT_TEST
#define MEMERY_POOL_SIZE (1024)
#else
#define MEMERY_POOL_SIZE (1024 * 1024 * 10)
#endif

typedef struct lx_memery_Obj {

    VOIDPTR (*lxmalloc)(struct lx_memery_Obj * this, size_t size);
    VOIDPTR (*lxrealloc)(struct lx_memery_Obj * this, VOIDPTR basePtr, size_t size);
    void (*lxfree)(struct lx_memery_Obj * this, VOIDPTR basePtr);

}lx_memery_Obj;

#define MALLOC(THIS, SIZE) ( \
    THIS == NULL ? NULL : (THIS)->lxmalloc(THIS, SIZE) \
    )

#define FREE(THIS, PTR) do{ \
    assert(THIS != NULL); \
    assert(PTR != NULL); \
    (THIS)->lxfree(THIS, PTR); \
}while(0)

#define REALLOC(THIS, PTR, SIZE) ( \
    assert(THIS != NULL); \
    assert(PTR != NULL); \
    THIS == NULL ? NULL : (THIS)->lxrealloc(THIS, PTR, SIZE) \
    )

lx_memery_Obj * lx_memery_Obj_new();
void lx_memery_Obj_delete(lx_memery_Obj * this);

#ifdef UNIT_TEST
u32int lxmemory_unit_test(void);
#endif

#endif