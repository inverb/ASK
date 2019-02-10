/* 
*   Mateusz Basiak, 23.12.2018
*   Zadanie 2
*/

#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <execinfo.h>
#include <string.h>
#include <ucontext.h>
#include "sem.h"

pthread_mutex_t mutex;
sem_t sem;
pthread_t thr[109];

int forks[109], n=100;

//Obsługa błędu
static void handle(int sig, siginfo_t *siginfo, void *context)
{
    for(int i=1; i<=n; i++) pthread_cancel(thr[i]);
    pthread_exit(0);
}


void think()
{
    usleep(rand()%500);
    return;
}

void eat()
{
    usleep(rand()%500);
    return;
}

void take_forks(int i)
{
    pthread_mutex_lock(&mutex);
    
    int i_l=i%n, i_r = (i+1)%n;
    while(!(forks[i_l] && forks[i_r])) {
        pthread_mutex_unlock(&mutex);
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
    }
    forks[i_l]=0;
    forks[i_r]=0;

    pthread_mutex_unlock(&mutex);
    return;
}

void put_forks(int i)
{
    pthread_mutex_lock(&mutex);

    forks[i%n]=1;
    forks[(i+1)%n]=1;
    sem_post_broadcast(&sem);

    pthread_mutex_unlock(&mutex);
    return;
}

void* philosopher(void* i)
{
    int a = *(int*) i;
    a--;

    while(1) {
        think();
        take_forks(a);
        eat();
        put_forks(a);
    }
    pthread_exit(0);
}

int main()
{
    //Ustawianie signal handlera
    struct sigaction action;
    action.sa_sigaction = &handle;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &action, NULL);
    
    
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 1);
    
    // Wszystkie widelce są na stole.
    for(int i=0; i<n; i++) forks[i]=1;

    for(int i=1; i<=n; i++) {
        pthread_create(&thr[i], NULL, philosopher, &i);
    }
    for(int i=1; i<=n; i++) pthread_join(thr[i], NULL);
    return 0;
}