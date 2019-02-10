/* Imię nazwisko: Mateusz Basiak
 * Numer indeksu: 300487
 *
 * Oświadczam, że:
 *  - rozwiązanie zadania jest mojego autorstwa,
 *  - jego kodu źródłowego dnie będę udostępniać innym studentom,
 *  - a w szczególności nie będę go publikować w sieci Internet.
 *
 * Q: Zdefiniuj proces "sierotę".
 * A: Sierota to proces, który nadal jest w trakcie wykonywania, ale jego rodzic zakończył już swoje wykonywanie.
 *
 * Q: Co się stanie, jeśli główny proces nie podejmie się roli żniwiarza?
 * A: Przygarnia go proces init(1) (na niektórych systemach systemach Unixowych systemd - dziecko init(1)).
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

void parent(pid_t child_pid, int gid)           //Rodzic
{
    siginfo_t sig;
    waitid(P_PID, child_pid, &sig, WEXITED);    //Dziecko umiera, przygarniamy wnuka.
    if(sig.si_code !=1) {
        printf("Something got wrong!");
    }
    int a=getpid();
    char s[6];
    snprintf(s, 6, "%d", a);
    printf("Lista dzieci procesu %d:\n", getpid());
    char *new_args[] = {"/bin/ps", "--ppid", s, NULL};
    execv(new_args[0], new_args);
    killpg(gid, SIGKILL);
    return;
}

int main(void)
{
    int id,gid;
    id=getpid();
    gid=id;                                     //Done by setpgid() by default.
    setpgid(id, gid);
    printf("PID ojca: %d\n", id);
    prctl(PR_SET_CHILD_SUBREAPER, 1);
    pid_t child_pid = fork();

    if (child_pid == 0)                         //dziecko
    {
        int id1=getpid();
        setpgid(id1,gid);
        printf("PID dziecka: %d\n", id1);
        pid_t child_pid1 = fork();
        if(child_pid1 == 0)                     //wnuk
        {
            int id2=getpid();
            setpgid(id2,gid);
            printf("PID wnuka: %d\n", id2);
            pause();
        }
    }
    else {                                      //rodzic
        parent(child_pid, gid);
    }

    return EXIT_SUCCESS;
}
