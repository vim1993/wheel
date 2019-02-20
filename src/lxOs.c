#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "type.h"
#include "lxOs.h"
#include "lxlog.h"

void * lxOSMalloc(size_t st)
{
    if(st <= 0)
    {
        return NULL;
    }

    void * mOSMalloPtr = malloc(st);
    if(!mOSMalloPtr)
    {
        return NULL;
    }

    LOG_INFO_PRINT("[%s][%d]!malloc:%p\n", __func__, __LINE__, mOSMalloPtr);

    return mOSMalloPtr;
}

void lxOSFree(void * pFree)
{
    if(pFree)
    {
        LOG_INFO_PRINT("[%s][%d]!pFree:%p\n", __func__, __LINE__, pFree);

        free(pFree);
        pFree = NULL;
    }

    return;
}

/*this module designed for mutex */
typedef struct mutex_lock_manager {
    pthread_mutex_t mMutex;
    mutex_lock mlock;
}mutex_lock_manager;

static void pthread_resource_lock(mutex_lock * _mutex)
{
    mutex_lock_manager * lockManager = GET_STRUCT_HEAD_PTR(mutex_lock_manager, _mutex, mlock);
    if(lockManager != NULL)
    {
        pthread_mutex_lock(&lockManager->mMutex);
    }

    return;
}

static void pthread_resource_unlock(mutex_lock * _mutex)
{
    mutex_lock_manager * lockManager = GET_STRUCT_HEAD_PTR(mutex_lock_manager, _mutex, mlock);
    if(lockManager != NULL)
    {
        pthread_mutex_unlock(&lockManager->mMutex);
    }

    return;
}

mutex_lock * pthread_resource_lock_new(void)
{
    int i32Ret = 0;
    mutex_lock_manager * lockManager = lxOSMalloc(sizeof(mutex_lock_manager));
    if(!lockManager)
    {
        return NULL;
    }

    i32Ret = pthread_mutex_init(&lockManager->mMutex, NULL);
    if(i32Ret != 0)
    {
        lxOSFree(lockManager);
        return NULL;
    }

    lockManager->mlock.pthread_resource_lock = pthread_resource_lock;
    lockManager->mlock.pthread_resource_unlock = pthread_resource_unlock;

    return &lockManager->mlock;
}

void pthread_resource_lock_delete(mutex_lock * _mutex)
{
    mutex_lock_manager * lockManager = GET_STRUCT_HEAD_PTR(mutex_lock_manager, _mutex, mlock);
    if(lockManager != NULL)
    {
        pthread_mutex_destroy(&lockManager->mMutex);
        lxOSFree(lockManager);
        _mutex = NULL;
    }

    return;
}

typedef struct semaphore_manager {
    sem_t mSem_t;
    semaphore_t mSem;
}semaphore_manager;

static void semaphore_post(semaphore_t * _mutex)
{
    semaphore_manager * semManager = GET_STRUCT_HEAD_PTR(semaphore_manager, _mutex, mSem);
    if(semManager)
    {
        sem_post(&semManager->mSem_t);
    }

    return;
}

static void semaphore_wait(semaphore_t * _mutex)
{
    semaphore_manager * semManager = GET_STRUCT_HEAD_PTR(semaphore_manager, _mutex, mSem);
    if(semManager)
    {
        sem_wait(&semManager->mSem_t);
    }

    return;
}

static int semaphore_wait_timeout(semaphore_t * _mutex, unsigned char timeoutS)
{
    struct timespec timeout = {0};

    semaphore_manager * semManager = GET_STRUCT_HEAD_PTR(semaphore_manager, _mutex, mSem);
    if(semManager)
    {
        clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += timeoutS;
        return sem_timedwait(&semManager->mSem_t, &timeout);
    }

    return RET_FAILED;
}

semaphore_t * semaphore_t_new(void)
{
    semaphore_manager * semManager = lxOSMalloc(sizeof(semaphore_manager));
    if(!semManager)
    {
        return NULL;
    }

    semManager->mSem.semaphore_post = semaphore_post;
    semManager->mSem.semaphore_wait = semaphore_wait;
    semManager->mSem.semaphore_wait_timeout = semaphore_wait_timeout;

    if(sem_init(&semManager->mSem_t, 0, 0) != 0)
    {
        lxOSFree(semManager);
        return NULL;
    }

    return &semManager->mSem;
}

void semaphore_t_delete(semaphore_t * _mutex)
{
    semaphore_manager * semManager = GET_STRUCT_HEAD_PTR(semaphore_manager, _mutex, mSem);
    if(semManager)
    {
        sem_destroy(&semManager->mSem_t);
        lxOSFree(semManager);
        _mutex = NULL;
    }

    return;
}

typedef enum timer_status_e {
    TIMER_START,
    TIEMR_PAUSE,
    TIEMR_STOP,
    TIEMR_RELEASE,
    TIEMR_IDEL
}timer_status_e;

typedef struct timer_manager_obj {
    unsigned int timerus;

    pthread_t m_pid;
    timer_status_e timer_status;
    timer_notify_p pNotify;
    timer_obj m_obj;
}timer_manager_obj;

static void * timer_proc(void * param)
{
    timer_status_e timerStatus = TIEMR_IDEL;
    timer_manager_obj * timerManagerObj = (timer_manager_obj *)param;

    while(1) {
        timerStatus = timerManagerObj->timer_status;
        if(timerStatus == TIMER_START) {
            timerManagerObj->pNotify();
        } else if(timerStatus == TIEMR_RELEASE) {
            break;
        }

        usleep(timerManagerObj->timerus * 1000);
    }

    return NULL;
}

static BOOLTYPE start_timer(struct timer_obj *pThis, unsigned int timerus, timer_notify_p notify)
{
    if(notify == NULL || timerus == 0) {
        return BOOL_FALSE;
    }

    timer_manager_obj * timerManagerObj = GET_STRUCT_HEAD_PTR(timer_manager_obj, pThis, m_obj);
    if(timerManagerObj == NULL) {
        return BOOL_FALSE;
    }

    timerManagerObj->timer_status = TIMER_START;
    timerManagerObj->timerus = timerus;
    timerManagerObj->pNotify = notify;

    if(pthread_create(&timerManagerObj->m_pid, NULL, timer_proc, (void *)timerManagerObj) != 0) {
        timerManagerObj->timer_status = TIEMR_RELEASE;
        return BOOL_FALSE;
    }

    return BOOL_TRUE;
}

static BOOLTYPE pause_timer(struct timer_obj *pThis)
{
    timer_manager_obj * timerManagerObj = GET_STRUCT_HEAD_PTR(timer_manager_obj, pThis, m_obj);
    if(timerManagerObj == NULL) {
        return BOOL_FALSE;
    }

    timerManagerObj->timer_status = TIEMR_PAUSE;

    return BOOL_TRUE;
}

static BOOLTYPE release_timer(struct timer_obj *pThis)
{
    timer_manager_obj * timerManagerObj = GET_STRUCT_HEAD_PTR(timer_manager_obj, pThis, m_obj);
    if(timerManagerObj == NULL) {
        return BOOL_FALSE;
    }

    timerManagerObj->timer_status = TIEMR_RELEASE;

    return BOOL_TRUE;
}

timer_obj * timer_obj_new(void)
{
    timer_manager_obj * timerManagerObj = (timer_manager_obj *)lxOSMalloc(sizeof(timer_manager_obj));
    if(timerManagerObj == NULL) {
        return NULL;
    }

    timerManagerObj->timerus = 0;
    timerManagerObj->timer_status = TIEMR_IDEL;

    timerManagerObj->m_obj.pause_timer = pause_timer;
    timerManagerObj->m_obj.release_timer = release_timer;
    timerManagerObj->m_obj.start_timer = start_timer;

    return &timerManagerObj->m_obj;
}

void timer_obj_delete(struct timer_obj *pThis)
{
    timer_manager_obj * timerManagerObj = GET_STRUCT_HEAD_PTR(timer_manager_obj, pThis, m_obj);
    if(timerManagerObj == NULL) {
        return;
    }

    timerManagerObj->timer_status = TIEMR_RELEASE;
    pthread_join(timerManagerObj->m_pid, NULL);

    lxOSFree(timerManagerObj);

    return;
}
