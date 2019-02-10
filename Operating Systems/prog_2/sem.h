/*
*   Mateusz Basiak, 22.12.2018
*   Zadanie 1
*/

#include <pthread.h>
#include <assert.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t number;
    int cnt;
} sem_t;

void sem_init(sem_t *sem, unsigned int value);

void sem_wait(sem_t *sem);

void sem_post(sem_t *sem);

void sem_getvalue(sem_t *sem, int *sval);


void sem_post_broadcast(sem_t *sem);