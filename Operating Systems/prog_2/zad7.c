/*
*   Mateusz Basiak, 23.12.2018
*   Zadanie 7
*/

#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex, appenders_mutex, deleters_mutex;
pthread_cond_t nodeleter, noreaders;

pthread_t thr[109];
long long number = 999666123, n = 10000, a=1;
int tab[100009], deleter, readers;

//Generator liczb pseudolosowych
long long random(long long length)
{
    a++;
    a *= number;
    if(length > 1) a %= length;
    else a %= 100000;
    return a;
}

int search(int a)
{
    pthread_mutex_lock(&mutex);
    while(deleter)
        pthread_cond_wait(&nodeleter, &mutex);
    readers++;
    pthread_mutex_unlock(&mutex);

    int is_in_set=0;
    for(int i=0; i<=n; i++) {
        if(tab[i]==a){
            is_in_set=1;
            break;
        }
    }

    pthread_mutex_lock(&mutex);
    readers--;
    if(readers == 0)
        pthread_cond_signal(&noreaders);
    pthread_mutex_unlock(&mutex);

    return is_in_set;
}

void add(int a)
{
    pthread_mutex_lock(&appenders_mutex);
    
    if(n<100000) {
        tab[n+1]=a;
        n++;
    }

    pthread_mutex_unlock(&appenders_mutex);
    return;
}

void remove(int a)
{   
    pthread_mutex_lock(&deleters_mutex);
    pthread_mutex_lock(&appenders_mutex);
    pthread_mutex_lock(&mutex);
    while(readers > 0)
        pthread_cond_wait(&noreaders, &mutex);
    deleter=1;
    pthread_mutex_unlock(&mutex);

    for(int i=0; i<n; i++)
    {
        if(tab[i]==a)
        {
            for(int j=i; j<n; j++) tab[j]=tab[j+1];
            tab[n]=0;
            n--;
            break;
        }
    }

    pthread_mutex_lock(&mutex);
    deleter=0;
    pthread_cond_broadcast(&nodeleter);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&appenders_mutex);
    pthread_mutex_unlock(&deleters_mutex);
    return;
}

void* reader()
{   
    while(1) {
        usleep(random(500));
        search(random(n));
    }
    pthread_exit(0);
}

void* writer()
{
    int x;
    while(1) {
        usleep(random(1000));
        if(random(3) == 0) {
            x = random(n);
            add(x);
        }
        else remove(random(n));
    }
    pthread_exit(0);
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&appenders_mutex, NULL);
    pthread_mutex_init(&deleters_mutex, NULL);
    pthread_cond_init(&nodeleter, NULL);
    pthread_cond_init(&noreaders, NULL);
    
    for(int i=0; i<=10000; i++) add(random(i));

    //Uruchamiam na 60 wątkach czytających i 40 wątkach piszących/usuwających.
    for(int i=1; i<=10; i++) {
        if(i%10 < 4) pthread_create(&thr[i], NULL, writer, NULL);  // 4/10
        else pthread_create(&thr[i], NULL, reader, NULL);          // 6/10
    }

    //Oczekiwanie na zakończenie wszystkich
    for(int i=1; i<=100; i++) pthread_join(thr[i], NULL);
    return 0;
}