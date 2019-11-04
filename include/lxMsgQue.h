#ifndef __LX_MSGQUE_H__
#define __LX_MSGQUE_H__

#include "type.h"

typedef struct msgque_obj {

    status_e  (*push_back)(struct msgque_obj *pthis, const void * data, size_t datalen);
    VOIDPTR   (*pop_front)(struct msgque_obj *pthis);
    VOIDPTR   (*pop_front_timeout)(struct msgque_obj *pthis, unsigned char timeoutS);
    void      (*release_buffer)(struct msgque_obj *pthis, void * data);
}msgque_obj;

msgque_obj * msgque_obj_new(void);
void msgque_obj_delete(msgque_obj * pthis);

#endif