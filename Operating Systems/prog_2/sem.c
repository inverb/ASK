/*
*   Mateusz Basiak, 22.12.2018
*   Zadanie 1
*/

#include <assert.h>
#include "sem.h"


void sem_init(sem_t *sem, unsigned int value)
{
    pthread_mutexattr_t mutex_attribute;
    pthread_mutexattr_init(&mutex_attribute);
    pthread_mutexattr_settype(&mutex_attribute, PTHREAD_MUTEX_ERRORCHECK_NP);

    pthread_mutex_init(&sem->mutex, &mutex_attribute);
    pthread_cond_init(&sem->number, NULL);
    sem->cnt = value;
    return;
}

void sem_wait(sem_t *sem)
{
    assert(pthread_mutex_lock(&sem->mutex) == 0);

    while(sem->cnt < 0) {
        pthread_cond_wait(&sem->number, &sem->mutex);
    }
    sem->cnt--;

    assert(pthread_mutex_unlock(&sem->mutex) == 0);
}

void sem_post(sem_t *sem)
{
    assert(pthread_mutex_lock(&sem->mutex) == 0);

    sem->cnt++;
    if (sem->cnt == 0)
        pthread_cond_signal(&sem->number);

    assert(pthread_mutex_unlock(&sem->mutex) == 0);
}

void sem_getvalue(sem_t *sem, int *sval)
{
    *sval = sem->cnt;
}


//Dodane do interfejsu semafora, gdyż broadcast potrzebne było w zadaniu drugim.
void sem_post_broadcast(sem_t *sem)
{
    assert(pthread_mutex_lock(&sem->mutex) == 0);

    sem->cnt++;
    if (sem->cnt == 0)
        pthread_cond_broadcast(&sem->number);

    assert(pthread_mutex_unlock(&sem->mutex) == 0);
}
