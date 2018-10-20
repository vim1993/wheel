#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "type.h"
#include "lxOs.h"

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

    return mOSMalloPtr;
}

void lxOSFree(void * pFree)
{
    if(pFree)
    {
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

semaphore_t * semaphore_t_new(void)
{
    semaphore_manager * semManager = lxOSMalloc(sizeof(semaphore_manager));
    if(!semManager)
    {
        return NULL;
    }

    semManager->mSem.semaphore_post = semaphore_post;
    semManager->mSem.semaphore_wait = semaphore_wait;
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

