/* Imię nazwisko: Mateusz Basiak
 * Numer indeksu: 300487
 *
 * Oświadczam, że:
 *  - rozwiązanie zadania jest mojego autorstwa,
 *  - jego kodu źródłowego dnie będę udostępniać innym studentom,
 *  - a w szczególności nie będę go publikować w sieci Internet.
 *
 * Q: Czemu procedura printf nie jest wielobieżna, a snprintf jest?
 * A: Printf używa globalnego bufora, więc przy przerwaniu działania funkcji, inne programy działające
 * w trakcie mogą do tego bufora coś zapisać i nie mamy nad tym kontroli.
 * Snprintf trzyma osobne bufory dla poszczególnych wątków i procesów, więc jest wielobieżna.
 */

#define _GNU_SOURCE
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <sys/mman.h>

static void handle(int sig, siginfo_t *siginfo, void *context)
{
  ucontext_t *p_context = (ucontext_t *)context;
  char str[1024];
  int shift = 0;

  //Opis błędu
  shift = snprintf(str,1024, "Adres powodujący błąd odwołania: %p\n", siginfo->si_addr);
  if(siginfo->si_code == 1) shift += snprintf(str+shift,1024-shift, "Typ błędu: \t \t \t MAPERR\n");
  else if(siginfo->si_code == 2) shift += snprintf(str+shift,1024-shift, "Typ błędu: \t \t \t ACCERR\n");
  shift += snprintf(str+shift,1024-shift, "Adres wierzchołka stosu: \t %lld\n", p_context->uc_mcontext.gregs[REG_RSP]);
  shift += snprintf(str+shift,1024-shift, "Linia powoduąca błąd: \t \t %lld\n", p_context->uc_mcontext.gregs[REG_RIP]);

  shift = write(1,str,strlen(str));

  //Backtrace błędu
  snprintf(str,1024, "Backtrace błędu:\n");
  shift = write(1,str,strlen(str));

  void *buffer[100];
  char **strings;

  int n = backtrace(buffer, 100);
  shift = 0;
  for(int i=0; i<1024; i++) str[i]=0;

  strings = backtrace_symbols(buffer, n);
  for (int i = 0; i < n; i++) {
    shift += snprintf(str+shift, 1024-shift, "%s\n", strings[i]);
  }
  shift = write(1, str, strlen(str));
  
  exit(EXIT_SUCCESS);
}

void maperr()
{
  int* pointer =mmap(0, 8, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  munmap(pointer, 8);
  *pointer = 42;
  return;
}

void accerr()
{
  int *pointer=mmap(0, 8, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
  *pointer = 42;
  return;
}

int main(int argc, char *argv[]) 
{
  //Ustawianie signal handlera
  struct sigaction action;
  action.sa_sigaction = &handle;
  action.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &action, NULL);

  //Generowanie właściwego błędu
  char* s = "--maperr";
  char* s1 = "--accerr";

  if(argc == 2 && strcmp(argv[1], s)==0){
    maperr();
  }
  if(argc == 2 && strcmp(argv[1], s1)==0){
    accerr();
  }

  return EXIT_SUCCESS;
}
