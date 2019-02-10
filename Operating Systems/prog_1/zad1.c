/* Imię nazwisko: Mateusz Basiak
 * Numer indeksu: 300487
 *
 * Oświadczam, że:
 *  - rozwiązanie zadania jest mojego autorstwa,
 *  - jego kodu źródłowego dnie będę udostępniać innym studentom,
 *  - a w szczególności nie będę go publikować w sieci Internet.
 *
 * Q: Zdefiniuj proces "zombie".
 * A: Proces "zombie" - proces, który zakończył wykonywanie, ale nadal jest widoczny w tablicy procesów,
 *    ponieważ dane o nim mogą być potrzebne jego rodzicowi.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{

    //Sprawdza, czy program dostał argument --bury, inne argumenty ignoruje
    char* s = "--bury";
    if(argc == 2 && strcmp(argv[1], s)==0)
    {
        struct sigaction sa;
        sa.sa_handler = SIG_IGN; //sig-ignore
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if (sigaction(SIGCHLD, &sa, 0) == -1) exit(0);
    }

    pid_t child_pid = fork();

    if (child_pid > 0)  //rodzic
    {
        sleep(1);
        int child = child_pid;
        char char_child[6];
        sprintf(char_child,"%d",child);
        char *new_args[] = {"/bin/ps", "-p", char_child, NULL};
        execv(new_args[0], new_args);
    }
    else exit(0);       //dziecko

    return EXIT_SUCCESS;
}
