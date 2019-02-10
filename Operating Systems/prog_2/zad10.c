/* 
*   Mateusz Basiak, 23.12.2018
*   Zadanie 10
*/

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_cond_t next_client, your_cut_is_over, wake_up_barber, wait_for_client;
pthread_t thr[1009];

int empty_seats = 100, clients;

void enter_barber_shop()
{
    pthread_mutex_lock(&mutex);
    if(empty_seats != 0)
    {
        //Udało się usiąść.
        empty_seats--;
        clients++;
        pthread_cond_signal(&wake_up_barber);
        pthread_cond_wait(&next_client, &mutex);
        

        //Nadeszła kolej tego klienta.
        empty_seats++;
        clients--;
        pthread_cond_signal(&wait_for_client);
        pthread_cond_wait(&your_cut_is_over, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void* client()
{
    while(1) {
        usleep(rand()%500);
        enter_barber_shop();
    }
    pthread_exit(0);
}

void* barber()
{
    pthread_mutex_lock(&mutex);
    while(1) {
        if(clients) {
            pthread_cond_signal(&next_client);
            pthread_cond_wait(&wait_for_client, &mutex);

            //Strzyżenie
            pthread_mutex_unlock(&mutex);
            usleep(rand()%500);

            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&your_cut_is_over);
        }
        else {
            pthread_cond_wait(&wake_up_barber, &mutex);
        }
    }
    pthread_exit(0);
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&next_client, NULL);
    pthread_cond_init(&your_cut_is_over, NULL);
    pthread_cond_init(&wake_up_barber, NULL);
    pthread_cond_init(&wait_for_client, NULL);

    pthread_create(&thr[0], NULL, barber, NULL);
    for(int i=1; i<=1000; i++) {
        pthread_create(&thr[i], NULL, client, NULL);
    }
    for(int i=0; i<=1000; i++) pthread_join(thr[i], NULL);
    return 0;
}