#ifndef __LX_LIST_H__
#define __LX_LIST_H__

#include <assert.h>
#include "type.h"

typedef enum ADD_TYPE_E {
    NEXT_ADD_E,
    PREV_ADD_E
}ADD_TYPE_E;

//通用双向链表, 屏蔽数据域
typedef struct lxlist_node {
    struct lxlist_node * Next;
    struct lxlist_node * Prev;
}lxlist_node;

typedef struct lxlist_Obj {

    #ifdef LX_DEBUG
    void (*lxlist_show)(struct lxlist_node * baseNode);
    #endif

    void (*lxlist_init)(struct lxlist_node * InitNode);
    void (*lxlist_add)(struct lxlist_node * newNode, struct lxlist_node * PreNode, struct lxlist_node * NextNode);
    void (*lxlist_rm)(struct lxlist_node * rmNode, struct lxlist_node * PreNode, struct lxlist_node * NextNode);

}lxlist_Obj;

#ifdef LX_DEBUG
#define LXLIST_SHOW(THIS, NODEPTR) do { \
        assert(NODEPTR != NULL); \
        assert(THIS != NULL); \
        (THIS)->lxlist_show(NODEPTR); \
    }while(0)
#endif

#define LXLIST_INIT(THIS, NODEPtr) do{ \
        assert(NODEPtr != NULL); \
        assert(THIS != NULL); \
        (THIS)->lxlist_init(NODEPtr); \
    }while(0)

#define LXLIST_ADD_HEAD(THIS, NEWNODEPtr, BASENODEPtr) do{ \
        assert(THIS != NULL); \
        assert(NEWNODEPtr != NULL); \
        assert(BASENODEPtr != NULL); \
        (THIS)->lxlist_add(NEWNODEPtr, (BASENODEPtr)->Prev, BASENODEPtr); \
    }while(0)

#define LXLIST_ADD_TAIL(THIS, NEWNODEPtr, BASENODEPtr) do{ \
        assert(THIS != NULL); \
        assert(NEWNODEPtr != NULL); \
        assert(BASENODEPtr != NULL); \
        (THIS)->lxlist_add(NEWNODEPtr, BASENODEPtr, (BASENODEPtr)->Next); \
    }while(0)

#define LXLIST_DEL_NODE(THIS, DELNODE) do { \
        assert(THIS); \
        assert(DELNODE); \
        (THIS)->lxlist_rm(DELNODE, (DELNODE)->Prev, (DELNODE)->Next); \
    }while(0)

lxlist_Obj * lxlist_Obj_new(void);
void lxlist_Obj_delete(lxlist_Obj * this);

#ifdef UNIT_TEST
BOOLTYPE lxlist_unit_test_main(int argc, char *argv[]);
#endif

#endif