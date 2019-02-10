/* 
*   Mateusz Basiak, 23.12.2018
*   Zadanie 1
*   Testowanie semafora
*   Główna część kodu jest w plikach sem.c oraz sem.h.
*/

#include <unistd.h>
#include <pthread.h>
#include "sem.h"

sem_t sem;

pthread_t thr[11];

void* function()
{
    sem_wait(&sem);
    usleep(10);
    sem_post(&sem);
    pthread_exit(0);
}

int main()
{
    sem_init(&sem, 1);
    for(int i=1; i<=10; i++) {
        pthread_create(&thr[i], NULL, function, NULL);
    }
    for(int i=1; i<=10; i++) pthread_join(thr[i], NULL);
    return 0;
}