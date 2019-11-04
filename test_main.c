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

    utils_ecb_unit_test(argc, argv);
    lxmemory_unit_test();
    lxlist_unit_test_main();
    lx_sequence_stack_unit_test();
    //lxstack_unit_test();

    lxque_obj * que = New(lxque_obj);
    que->push_back(que, "hello world", 11);
    DELETE(lxque_obj, que);

    msgque_obj * msgque = New(msgque_obj);
    msgque->push_back(msgque, "hello world", 11);
    DELETE(msgque_obj, msgque);

    lxlist_unit_test_main(argc, argv);

    timer_obj * timer = New(timer_obj);
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
