#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define _POSIX_SOURCE 1

void handler ()
{
  return;
}

void calcul_1 ( ) 
{
  int i;
  for (i = 0; i < 1E8; i++);
  fprintf(stdout,"FIN CALCUL 1\n");
}

void calcul_2 () 
{
  int i;
  fprintf(stdout,"DEBUT CALCUL 2\n");
  for (i = 0; i < 1E8; i++);
}

int main () 
{
  int i=0;
  pid_t pid_fils[2];
  
  struct sigaction sigact;
  sigset_t mask;
  
  sigfillset (&mask);
  
  sigact.sa_flags = 0;
  sigact.sa_handler = handler;
  sigact.sa_mask = mask;

  sigaction (SIGUSR1,&sigact,NULL);
  sigprocmask (SIG_SETMASK,&mask,NULL);
  
  while ((i<2) && ((pid_fils[i] = fork())!=0))
    i++;
  
  if (i == 0)
    {
      printf("Cas : i == 0 (FILS 1)\n");
      calcul_1 ();
      
      kill(getppid(),SIGUSR1);
      sigfillset (&mask);
      sigdelset (&mask,SIGUSR1);
      sigsuspend (&mask);
      
      calcul_2 ();
      printf ("fin processus FILS 1\n");
    }
  else if (i == 1)
    {
      printf("Cas : i == 1 (FILS 2)\n");
      calcul_1 ();
      
      kill(getppid(),SIGUSR2);
      sigfillset (&mask);
      sigdelset (&mask,SIGUSR1);
      sigsuspend (&mask);
      
      calcul_2 ();
      printf ("fin processus FILS 2\n");
    }
  else
    {
      printf("Cas : else (PERE)\n");
      calcul_1 ();

      sigfillset (&mask);
      sigdelset (&mask,SIGUSR1);
      sigsuspend (&mask);
      sigaction (SIGUSR2, &sigact, NULL);
      sigprocmask (SIG_SETMASK, &mask, NULL);
      sigfillset (&mask);
      sigdelset (&mask,SIGUSR2);
      sigsuspend (&mask);
      kill (pid_fils[0],SIGUSR1);
      kill (pid_fils[1],SIGUSR1);

      calcul_2();
      printf ("fin processus PERE\n");
    }
  
  /*printf ("fin processus %d \n",i);*/             
  
  return EXIT_SUCCESS;   
}
