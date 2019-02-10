/* 
*   Mateusz Basiak, 23.12.2018
*   Zadanie 3
*/

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <execinfo.h>
#include <string.h>
#include <ucontext.h>
#include <semaphore.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

sem_t* mutex;
sem_t* count;
sem_t* forks[1009];

int children_ids[1009];

int n=10, id, gid;
char s;

//Obsługa błędu
static void handle(int sig, siginfo_t *siginfo, void *context)
{
    killpg(gid, SIGKILL);
    sem_unlink("count");
    sem_unlink("mutex");
    s='a';
    for(int i=1; i<=n; i++)
    {
        s++;
        sem_unlink(&s);
    }
    return;
}


void think()
{
    usleep(rand()%5);
    return;
}

void eat()
{
    usleep(rand()%5);
    return;
}

void take_forks(int i)
{
    sem_wait(mutex);
    
    int i_l=i%n+1, i_r = (i+1)%n+1, val, val2;
    sem_getvalue(forks[i_l], &val);
    sem_getvalue(forks[i_r], &val2);

    //Czekanie na widelce
    while(val == 0 || val2 == 0) {
        sem_post(mutex);
        if(val == 0) {
            sem_wait(forks[i_l]);
            sem_wait(mutex);
            sem_post(forks[i_l]);
        }
        if(val2 == 0) {
            sem_wait(forks[i_r]);
            sem_wait(mutex);
            sem_post(forks[i_r]);
        }
        sem_getvalue(forks[i_l], &val);
        sem_getvalue(forks[i_r], &val2);
    }

    //Zabranie widelców
    sem_wait(forks[i_l]);
    sem_wait(forks[i_r]);

    sem_post(mutex);
    return;
}

void put_forks(int i)
{
    sem_wait(mutex);

    sem_post(forks[i%n+1]);
    sem_post(forks[(i+1)%n+1]);
    sem_post(count);

    sem_post(mutex);
    return;
}

void philosopher(int i)
{
    while(1) {
        think();
        take_forks(i);
        eat();
        put_forks(i);
    }
    return;
}

int main()
{
    //Ustawianie signal handlera
    struct sigaction action;
    action.sa_sigaction = &handle;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &action, NULL);
    
    
    count=sem_open("/count", O_CREAT, 0770, 1);
    mutex=sem_open("/mutex", O_CREAT, 0770, 1);

    // Wszystkie widelce są na stole.
    s='a';
    for(int i=1; i<=n; i++) {
        s++;
        forks[i]=sem_open(&s, O_CREAT, 0770, 0);
        sem_post(forks[i]);
    }

    //Tworzenie procesów potomnych.
    id=getpid();
    gid=id;
    setpgid(id, gid);
    for(int i=1; i<=n; i++) {
        pid_t child_pid = fork();
        if(child_pid == 0) {
            setpgid(getpid(), gid);
            philosopher(i);
            return 0;
        }
        else children_ids[i] = child_pid;
    }

    //Oczekiwanie na zamknięcie potomków.
    siginfo_t sig;
    for(int i=1; i<=n; i++) waitid(P_PID, children_ids[i], &sig, WEXITED);
    return 0;
}