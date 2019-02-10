/* Imię nazwisko: Mateusz Basiak
 * Numer indeksu: 300487
 *
 * Oświadczam, że:
 *  - rozwiązanie zadania jest mojego autorstwa,
 *  - jego kodu źródłowego dnie będę udostępniać innym studentom,
 *  - a w szczególności nie będę go publikować w sieci Internet.
 *
 * Q: Czemu nie musisz synchronizować dostępu do zmiennych współdzielonych?
 * A: Ponieważ fakt, że funkcje działają równolegle to iluzja - ich działanie
 * się przeplata, ale nigdy żadne dwie nie pracują w tym samym czasie.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ucontext.h>

static ucontext_t uctx_main, uctx_func_1, uctx_func_2, uctx_func_3;
char func_1_stack[32768];
char func_2_stack[32768];
char func_3_stack[32768];
int fin;
char word[256];
int read();
int write();

static void func_1()
{
    static int words = 0;
    char ch;
    int pointer;

    while(read(0, &ch, 1) > 0) {
        pointer=0;
        for(int i=0; i<=254; i++) word[i]=0;

        //Dopóki wczytujesz białe znaki - wczytuj dalej.
        while(isspace(ch)) {
            if(read(0, &ch, 1) <= 0)
            {
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
        swapcontext(&uctx_func_1, &uctx_func_2);
        if(pointer==-1) break;
    }
    fin=1; //zmienna oznaczająca zakończenie wejścia
    fprintf(stderr, "words = %d\n", words);
}

static void func_2()
{
    static int removed = 0;
    int pointer, pointer2;

    while(1) {
        if(fin==1) break;
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
        swapcontext(&uctx_func_2, &uctx_func_3);
    }

    fprintf(stderr, "removed = %d\n", removed);
}

static void func_3()
{
    static int chars = 0;
    int pointer;
    int isprinted;
    while(1) {
        if(fin==1) break;
        pointer=0;
        isprinted=0;

        //Wypisz słowo.
        while(pointer<255 && word[pointer]!=0) {
            write(2, &word[pointer], 1);
            pointer++;
            chars++;
            isprinted=1;
        }
        
        if(isprinted) write(2, "\n", 1);
        swapcontext(&uctx_func_3, &uctx_func_1);
    }
    fprintf(stderr, "chars = %d\n", chars);
}

int main()
{
    //Tworzenie kontekstu dla func_1
    getcontext(&uctx_func_1);
    uctx_func_1.uc_stack.ss_sp = func_1_stack;
    uctx_func_1.uc_stack.ss_size = sizeof(func_1_stack);
    uctx_func_1.uc_link = &uctx_func_2;
    makecontext(&uctx_func_1, func_1, 0);

    //Tworzenie kontekstu dla func_2
    getcontext(&uctx_func_2);
    uctx_func_2.uc_stack.ss_sp = func_2_stack;
    uctx_func_2.uc_stack.ss_size = sizeof(func_2_stack);
    uctx_func_2.uc_link = &uctx_func_3;
    makecontext(&uctx_func_2, func_2, 0);

    //Tworzenie kontekstu dla func_3
    getcontext(&uctx_func_3);
    uctx_func_3.uc_stack.ss_sp = func_3_stack;
    uctx_func_3.uc_stack.ss_size = sizeof(func_3_stack);
    uctx_func_3.uc_link = &uctx_main;
    makecontext(&uctx_func_3, func_3, 0);

    swapcontext(&uctx_main, &uctx_func_1);
    return EXIT_SUCCESS;
}
