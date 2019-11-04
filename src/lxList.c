#include "lxlog.h"
#include "lxOs.h"
#include "lxList.h"

#ifdef LX_DEBUG
static void lxlist_show(struct lxlist_node * baseNode)
{
    struct lxlist_node * node = baseNode;

    LOG_DEBUG_PRINT_EX();
    while(node)
    {
        LOG_DEBUG_PRINT("%p\n", node);
        node = node->Next;
        if(node == baseNode)
        {
            break;
        }
    }

    return;
}
#endif

static void lxlist_init(struct lxlist_node * InitNode)
{
    InitNode->Next = InitNode->Prev = InitNode;
}

static void lxlist_add(struct lxlist_node * newNode, struct lxlist_node * PreNode, struct lxlist_node * NextNode)
{
    assert(newNode != NULL);
    assert(PreNode != NULL);
    assert(NextNode != NULL);

    NextNode->Prev = newNode;
    newNode->Prev = PreNode;
    newNode->Next = NextNode;
    PreNode->Next = newNode;

    return;
}

static void lxlist_rm(struct lxlist_node * rmNode, struct lxlist_node * PreNode, struct lxlist_node * NextNode)
{
    PreNode->Next = NextNode;
    NextNode->Prev = PreNode;
    rmNode->Next = rmNode->Prev = NULL;
}

lxlist_Obj * lxlist_Obj_new(void)
{
    lxlist_Obj * mLxlistOBJ = (lxlist_Obj *)lxOSMalloc(sizeof(lxlist_Obj));
    if(!mLxlistOBJ)
    {
        return NULL;
    }

    mLxlistOBJ->lxlist_add = lxlist_add;
    mLxlistOBJ->lxlist_init = lxlist_init;
    mLxlistOBJ->lxlist_rm  = lxlist_rm;

    #ifdef LX_DEBUG
    mLxlistOBJ->lxlist_show = lxlist_show;
    #endif

    return mLxlistOBJ;
}

void lxlist_Obj_delete(lxlist_Obj * pthis)
{
    pthis->lxlist_add = NULL;
    pthis->lxlist_init = NULL;
    pthis->lxlist_rm  = NULL;
    lxOSFree(pthis);

    return;
}

#ifdef UNIT_TEST

struct lxlist_test {
    struct lxlist_node node;
    u32int data;
};

BOOLTYPE lxlist_unit_test_main(int argc, char *argv[])
{
    struct lxlist_test lxt1 = {0}, lxt2 = {0}, lxt3 = {0};
    lxt1.data = 1;
    lxt2.data = 3;
    lxt3.data = 6;

    lxlist_Obj * pThis = lxlist_Obj_new();
    if(!pThis)
    {
        return BOOL_FALSE;
    }

    LXLIST_INIT(pThis, &lxt1.node);
    LXLIST_ADD_HEAD(pThis, &lxt2.node, &lxt1.node);
    LXLIST_ADD_TAIL(pThis, &lxt3.node, &lxt1.node);

    struct lxlist_test * ptr = GET_STRUCT_HEAD_PTR(struct lxlist_test, (&lxt1.node)->Next, node);
    LOG_DEBUG_PRINT("[data]:%d\n", ptr->data);
    lxlist_Obj_delete(pThis);

    return BOOL_TRUE;
}
#endif