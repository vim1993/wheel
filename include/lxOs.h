#ifndef __LX_OS_H__
#define __LX_OS_H__

#include <pthread.h>
#include <assert.h>

typedef struct mutex_lock {

    void (*pthread_resource_lock)(struct mutex_lock * _mutex); // mutex_lock
    void (*pthread_resource_unlock)(struct mutex_lock * _mutex); // mutex_unlock

}mutex_lock;

#define PMUTEX_LOCK(THIS) do { \
        assert(THIS != NULL); \
        (THIS)->pthread_resource_lock(THIS); \
    }while(0)

#define PMUTEX_UNLOCK(THIS) do { \
        assert(THIS != NULL); \
        (THIS)->pthread_resource_unlock(THIS); \
    }while(0)

extern void pthread_resource_lock_delete(mutex_lock * _mutex);
extern mutex_lock * pthread_resource_lock_new(void);

typedef struct semaphore_t {

    void (*semaphore_post)(struct semaphore_t * _sem); // sem_post
    void (*semaphore_wait)(struct semaphore_t * _sem); // sem_wait
    int (*semaphore_wait_timeout)(struct semaphore_t * _sem, unsigned char timeoutS); // sem_wait

}semaphore_t;

#define SEMAPHORE_POST(THIS) do{ \
        assert(THIS != NULL); \
        (THIS)->semaphore_post(THIS); \
    }while(0)

#define SEMAPHORE_WAIT(THIS) do{ \
        assert(THIS != NULL); \
        (THIS)->semaphore_wait(THIS); \
    }while(0)

#define SEMAPHORE_WAIT_TIMEOUT(THIS, TS) do{ \
        assert(THIS != NULL); \
        (THIS)->semaphore_wait_timeout(THIS, TS); \
    }while(0)

extern void semaphore_t_delete(semaphore_t * _mutex);
extern semaphore_t * semaphore_t_new(void);

void * lxOSMalloc(size_t st);
void lxOSFree(void * pFree);

#endif