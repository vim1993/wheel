#include "type.h"
#include "lxlog.h"
#include "lx_s_list.h"

typedef struct alloc_t {
    lx_s_list_alloc alloc;
    lx_s_list_free  free;
}alloc_t;

typedef struct lx_list_node {
    struct lx_list_node * Next;
    struct lx_list_node * Prev;
    void * data;
}lx_list_node;

typedef struct lx_s_list_context {

    u32int         ElmentsNum;
    u32int         ElmentsSize;
    u32int         ListNodeNum;
    lx_list_node * listHead;
    alloc_t        mAlloc;
    lx_s_list_OBJ  mlxOBJ;

}lx_s_list_context;

