#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define _POSIX_SOURCE 1

void handler()
{
  return;
}

int mysleep (int sec)
{
  alarm (sec);
  int restant;
  struct sigaction sigact;
  struct sigaction sigactold;
  sigset_t mask;

  sigfillset(&mask);
  sigdelset(&mask, SIGALRM);
  
  sigact.sa_flags = 1;
  sigact.sa_handler = handler;
  sigact.sa_mask = mask;
  
  /*Redefinition du comportement de SIGALRM*/
  sigaction (SIGALRM,&sigact,&sigactold);
    
  sigsuspend (&mask);
  restant = alarm(0);
      
  sigaction (SIGALRM,&sigactold,&sigact);

  return restant;
}

int main()
{
  sigset_t mask;
  sigfillset(&mask);
  sigprocmask(SIG_SETMASK,&mask,NULL);

  printf ("********************************\nTemps restant : %d seconde(s)\n********************************\n",mysleep(30));

  return EXIT_SUCCESS;
}
