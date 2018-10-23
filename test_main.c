#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lxOs.h"
#include "lxMsgQue.h"
#include "type.h"

msgque_obj * pMsgQue = NULL;

void task_routime(void * param)
{
    void * data = NULL;
    while(1)
    {
        data = pMsgQue->pop_front(pMsgQue);
        printf("data:%d\n", *((int *)data));
        pMsgQue->release_buffer(pMsgQue, data);
    }

    return;
}

int main(int argc, char *argv[])
{

    pthread_t tt;
    int i = 1;
    pMsgQue = NEW(msgque_obj);
    pthread_create(&tt, NULL, &task_routime, NULL);

    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));

    pthread_join(tt, NULL);
    DELETE(msgque_obj, pMsgQue);

    return 0;
}
