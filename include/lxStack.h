#ifndef __LX_STACK_H__
#define __LX_STACK_H__

#include "type.h"

typedef struct lxstack_obj {

    BOOLTYPE (*push_stack)(struct lxstack_obj * this, const void * data, size_t len);
    size_t   (*pop_stack)(struct lxstack_obj * this, void * outdata, size_t len);
    BOOLTYPE (*isEmpty)(struct lxstack_obj * this);

}lxstack_obj;

lxstack_obj * lxstack_obj_new(void);
void lxstack_obj_delete(lxstack_obj * this);

int lxstack_unit_test(void);

#endif