#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lxOs.h"
#include "lxMsgQue.h"
#include "type.h"

msgque_obj * pMsgQue = NULL;
unsigned int exit_pthread = 0;

void showtime(void)
{
    time_t timep;
    time(&timep);
    printf("%s\n",asctime(gmtime(&timep)));
}


void task_routime(void * param)
{
    void * data = NULL;
    while(1)
    {
        #if 0
        data = pMsgQue->pop_front(pMsgQue);
        #else
        data = pMsgQue->pop_front_timeout(pMsgQue, 2);
        if(!data)
        {
            showtime();
            continue;
        }
        #endif
        printf("data:%d\n", *((int *)data));
        pMsgQue->release_buffer(pMsgQue, data);
        if(exit_pthread)
        {
            break;
        }
    }

    printf("[%s][%d]! exit\n", __func__, __LINE__);
    return;
}

int main(int argc, char *argv[])
{

    pthread_t tt;
    int i = 1;
    pMsgQue = NEW(msgque_obj);
    pthread_create(&tt, NULL, &task_routime, NULL);

    #if 0
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));
    i++;
    pMsgQue->push_back(pMsgQue, &i, sizeof(int));

    exit_pthread = 1;
    #endif

    pthread_join(tt, NULL);
    DELETE(msgque_obj, pMsgQue);

    return 0;
}
