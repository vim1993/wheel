#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lxOs.h"

#include "type.h"

unsigned char buffer[16] = {0};

void task_routime(void * param)
{
    semaphore_t * smt = (semaphore_t *)param;
    printf("start wait...\n");
    smt->semaphore_wait(smt);
    printf("end wait...\n");
    return;
}

int main(int argc, char *argv[])
{

    pthread_t tt;
    const semaphore_t * smt = NEW(semaphore_t);
    pthread_create(&tt, NULL, &task_routime, smt);
    snprintf(buffer, sizeof(buffer), "hello wolrd");
    printf("start post...\n");
    smt->semaphore_post(smt);
    printf("end post...\n");
    sleep(5);
    DELETE(semaphore_t, smt);
    pthread_join(tt, NULL);

    return 0;
}
