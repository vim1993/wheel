#ifndef __LX_S_LIST_H__
#define __LX_S_LIST_H__

typedef struct lx_s_list_OBJ {

    BOOLTYPE    (*init)(struct lx_s_list_OBJ * this, u32int ElmentNum, u32int ElmentSize);
    void        (*push_back)(struct lx_s_list_OBJ * this, const void * data); //add data
    void        (*pop_front)(struct lx_s_list_OBJ * this); //remove head

    VOIDPTR     (*get_first_node)(struct lx_s_list_OBJ * this); //get list head
    VOIDPTR     (*get_next_node)(struct lx_s_list_OBJ * this, VOIDPTR node); //return node->Next;
    VOIDPTR     (*get_node_data)(struct lx_s_list_OBJ * this, VOIDPTR node); //get node->Data;

    VOIDPTR     (*earse_node)(struct lx_s_list_OBJ * this, VOIDPTR node); //earse current node, return node->Next;

    u32int      (*get_list_size)(struct lx_s_list_OBJ * this); //get list size, is full

}lx_s_list_OBJ;

#endif