#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "lxlog.h"
#include "lxQue.h"
#include "lxMsgQue.h"
#include "lxMemory.h"
#include "lxOs.h"
#include "lxevent.h"

typedef struct lxevent_context {


    lxevent_obj eventObj;
}lxevent_context;