#ifndef __LX_QUE_H__
#define __LX_QUE_H__

#include "type.h"

typedef struct lxque_obj {

    status_e  (*push_back)(struct lxque_obj *this, const void * data, size_t len);
    size_t    (*pop_front)(struct lxque_obj *this, void * outbuffer, size_t bufferlen);
    BOOLTYPE  (*isEmpty)(struct lxque_obj *this);

}lxque_obj;

lxque_obj * lxque_obj_new(void);
void lxque_obj_delete(lxque_obj * this);

#endif

