/* 
*   Mateusz Basiak, 23.12.2018
*   Zadanie 6
*/

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

sem_t* mutex;
sem_t* kucharz;
sem_t* strawa;
sem_t* cook_asleep;

//Zmienne z treści zadania
int  m = 100, n = 200;

void wilder()
{
    int food;
    while(1) {
        sem_wait(mutex);

        sem_getvalue(strawa, &food);

        //Pusty kociołek
        if(food == 0) {
            //Sprawdzenie, czy kucharz jeszcze śpi.
            sem_getvalue(cook_asleep, &food);
            if(food == 1) sem_post(kucharz);

            sem_post(mutex);
            sem_wait(strawa);
        }
        else {
            sem_wait(strawa);
            sem_post(mutex);
        }

        //Idzie spać
        usleep(rand()%1000);
    }
}

void cook()
{
    while(1) {
        sem_wait(kucharz);
        sem_wait(cook_asleep);
        sem_wait(mutex);

        //Gotowanie zajmuje chwilę.
        usleep(rand()%500);
        for(int i=1; i<=m; i++) sem_post(strawa);

        sem_post(cook_asleep);
        sem_post(mutex);
    }
}


int main()
{
    mutex=sem_open("/mutex", O_CREAT, 0770, 1);
    kucharz=sem_open("/kucharz", O_CREAT, 0770, 0);
    strawa=sem_open("/strawa", O_CREAT, 0770, 0);
    cook_asleep=sem_open("/asleep", O_CREAT, 0770, 1);

    //Tworzenie kucharza
    pid_t child_pid = fork();
    if(child_pid == 0) {
        cook();
        return 0;
    }

    //Tworzenie dzikusów
    for(int i=1; i<=n; i++) {
        pid_t child_pid = fork();
        if(child_pid == 0) {
            wilder();
            return 0;
        }
    }
    return 0;
}