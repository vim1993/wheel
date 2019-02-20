#ifndef __LX_EVENT_H__
#define __LX_EVENT_H__

#include "lxeventType.h"

typedef struct lxevent_obj {

    void (*event_loop)(struct lxevent_obj * pthis);
    void (*event_release)(struct lxevent_obj * pthis);

}lxevent_obj;

//struct lxevent_obj * lxevent_obj_new(unsigned int port, event_callback eventcb);
void lxevent_obj_delete(struct lxevent_obj * pthis);

#endif