#ifndef __LX_STACK_H__
#define __LX_STACK_H__

#include "type.h"

typedef struct lxstack_obj {

    BOOLTYPE (*push_stack)(struct lxstack_obj * pthis, const void * data, size_t len);
    size_t   (*pop_stack)(struct lxstack_obj * pthis, void * outdata, size_t len);
    BOOLTYPE (*isEmpty)(struct lxstack_obj * pthis);

    //pthis interfaces are for the sequential stack, and the chain stack returns false
    BOOLTYPE (*isFull)(struct lxstack_obj * pthis);

    //pthis interfaces are for the sequential stack, and the chain stack returns true
    BOOLTYPE (*init)(struct lxstack_obj * pthis, u32int ElmentNum);

}lxstack_obj;

lxstack_obj * lxstack_obj_new(void);
void lxstack_obj_delete(lxstack_obj * pthis);

//pthis is for sequence stack
typedef lxstack_obj lxstack_sequence;
lxstack_sequence * lxstack_sequence_new(void);
void lxstack_sequence_delete(lxstack_sequence * pthis);

int lxstack_unit_test(void);

#endif