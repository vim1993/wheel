#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lxOs.h"
#include "lxMsgQue.h"
#include "lxQue.h"
#include "lxStack.h"
#include "type.h"
#include "lxlog.h"
#include "lxMemory.h"

void timer_notify(void * param)
{
    LOG_ERROR_PRINT("timer event...\n");
}


void init(LOG_LEVEL_E level)
{
    Log_Set_PrintLevel(level);
}

int main(int argc, char *argv[])
{
    unsigned int cnt = 0;
    init(LOG_LEVEL_INFO);

    timer_obj * timer = NEW(timer_obj);
    if(timer == NULL) {
        return -1;
    }

    timer->start_timer(timer, 100, timer_notify, NULL);

    while(1) {
        usleep(100 * 1000);
        cnt++;
        if(cnt == 10) {
            timer->release_timer(timer);
            break;
        }
    }

    DELETE(timer_obj, timer);

    return 0;
}
