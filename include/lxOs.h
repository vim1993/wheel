#ifndef __LX_OS_H__
#define __LX_OS_H__

#include <pthread.h>

typedef struct mutex_lock {

    void (*pthread_resource_lock)(struct mutex_lock * _mutex); // mutex_lock
    void (*pthread_resource_unlock)(struct mutex_lock * _mutex); // mutex_unlock

}mutex_lock;

extern void pthread_resource_lock_delete(mutex_lock * _mutex);
extern mutex_lock * pthread_resource_lock_new(void);

typedef struct semaphore_t {

    void (*semaphore_post)(struct semaphore_t * _sem); // sem_post
    void (*semaphore_wait)(struct semaphore_t * _sem); // sem_wait
    int (*semaphore_wait_timeout)(struct semaphore_t * _sem, unsigned char timeoutS); // sem_wait

}semaphore_t;

extern void semaphore_t_delete(semaphore_t * _mutex);
extern semaphore_t * semaphore_t_new(void);

void * lxOSMalloc(size_t st);
void lxOSFree(void * pFree);

#endif