/* Imię nazwisko: Mateusz Basiak
 * Numer indeksu: 300487
 *
 * Oświadczam, że:
 *  - rozwiązanie zadania jest mojego autorstwa,
 *  - jego kodu źródłowego dnie będę udostępniać innym studentom,
 *  - a w szczególności nie będę go publikować w sieci Internet.
 *
 * Q: Jak proces wykrywa, że drugi koniec potoku został zamknięty?
 * A: W momencie zamknięcia potoku zostaje wysłany sygnał EOF.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Cztery rury - dwie dwukierunkowe rury między procesami 1 i 2 oraz 2 i 3.
int fd[2], fd2[2], fd3[2], fd4[2];

static int func_1()
{
    static int words = 0;
    char ch;
    int pointer,a=0;
    a++;
    char word[256];

    close(fd[0]);
    close(fd3[1]);
    while(read(0, &ch, 1) > 0) {
        pointer=0;
        for(int i=0; i<=255; i++) word[i]=0;

        //Dopóki wczytujesz białe znaki - wczytuj dalej.
        while(isspace(ch)) {
            if(read(0, &ch, 1) <= 0) {
                pointer = -1;
                break;
            }
        }
        if(pointer==-1) break;

        //Dopóki nie napotkasz białego znaku lub końca pliku - wczytuj dalej słowo.
        while(!isspace(ch)) {
            word[pointer] = ch;
            pointer++;
            if(read(0, &ch, 1) <= 0) {
                pointer = -1;
                break;
            }
        }
        words++;
        a=write(fd[1], word, 256);
        if(pointer==-1) break;

        //Czekanie na sygnał od func_2()
        while(read(fd3[0],&ch,1)==0) a=0;
    }
    
    for(int i=0; i<=255; i++) word[i]='1';
    a=write(fd[1], word, 256);
    sleep(1);
    fprintf(stderr, "words = %d\n", words);
    close(fd[1]);
    close(fd3[0]);
    return EXIT_SUCCESS;
}

static int func_2()
{
    static int removed = 0;
    int pointer, pointer2,a=0;
    a++;
    char word[256], ch='c';

    close(fd[1]);
    close(fd2[0]);
    close(fd3[0]);
    close(fd4[1]);
    while(read(fd[0], &word, 256) > 0)
    {
        if(word[255]=='1') break;
        pointer=0;
        pointer2=0;

        //Chodzenie dwoma wskaźnikami po słowie i usuwalnie znaków,
        //które nie są znakami alfanumerycznymi.
        while(pointer<255 && word[pointer]!=0) {
            if(isalnum(word[pointer]) != 0) {
                word[pointer2] = word[pointer];
                pointer2++;
            }
            else removed++;
            pointer++;
        }

        //Czyszczenie pozostałej części słowa.
        while(pointer2<255) {
            word[pointer2]=0;
            pointer2++;
        }
        a=write(fd2[1], word, 256);

        //Czekanie na sygnał od func_3()
        while(read(fd4[0],&ch,1)==0) a=0;
        a=write(fd3[1], &ch, 1);
    }

    for(int i=0; i<=255; i++) word[i]='1';
    a=write(fd2[1], word, 256);
    sleep(1);
    fprintf(stderr, "removed = %d\n", removed);
    close(fd2[1]);
    close(fd[0]);
    close(fd3[1]);
    close(fd4[0]);
    return EXIT_SUCCESS;
}

static int func_3()
{
    static int chars = 0;
    int pointer;
    int isprinted;
    int a=0;
    a++;
    char word[256], ch='c';

    close(fd2[1]);
    close(fd4[0]);
    while(read(fd2[0], &word, 256) > 0)
    {
        if(word[255]=='1') break;
        pointer=0;
        isprinted=0;

        //Wypisz słowo.
        while(pointer<255 && word[pointer]!=0) {
            a=write(2, &word[pointer], 1);
            pointer++;
            chars++;
            isprinted=1;
        }
        if(isprinted) a=write(2, "\n", 1);
        a=write(fd4[1], &ch, 1);
    }

    sleep(1);
    fprintf(stderr, "chars = %d\n", chars);
    close(fd2[0]);
    close(fd4[1]);
    return EXIT_SUCCESS;
}

static int parent(pid_t child_pid, pid_t child_pid2, pid_t child_pid3)
{
    int success=1;
    siginfo_t sig;
    waitid(P_PID, child_pid, &sig, WEXITED);
    if(sig.si_code != 1)
    {
        kill(child_pid, SIGKILL);
        printf("Coś poszło nie tak - dziecko nie zakończyło się poprawnie");
        success=0;
    }
    waitid(P_PID, child_pid2, &sig, WEXITED);
    if(sig.si_code != 1)
    {
        kill(child_pid2, SIGKILL);
        printf("Coś poszło nie tak - dziecko nie zakończyło się poprawnie");
        success=0;
    }
    waitid(P_PID, child_pid3, &sig, WEXITED);
    if(sig.si_code != 1)
    {
        kill(child_pid3, SIGKILL);
        printf("Coś poszło nie tak - dziecko nie zakończyło się poprawnie");
        success=0;
    }
    return success;
}

int main()
{
    int a;
    a=pipe(fd);
    a=pipe(fd3);
    pid_t child_pid = fork();
    a++;
    if(child_pid==0) func_1();
    else
    {
        a=pipe(fd2);
        a=pipe(fd4);
        pid_t child_pid2 = fork();
        if(child_pid2==0) func_2();
        else
        {
            pid_t child_pid3 = fork();
            if(child_pid3==0) func_3();
            else {                                //parent
                parent(child_pid, child_pid2, child_pid3);
            }
        }
    }
    return EXIT_SUCCESS;
}