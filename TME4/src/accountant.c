#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define _POSIX_SOURCE 1
#define MAX_INTR 5

sigset_t sig;
sigset_t sigpendings;
struct sigaction sigact[32];

int *compteur;
int cpt2=0;

void sig_handler (int sig)
{
  int i;
  cpt2++; /*Compteur TOTAL*/
  compteur[sig]++; /*Compteur Signal*/
  
  if (compteur[SIGINT] >= MAX_INTR)
    {
      i=1;
      
      while (i != 32)
	{
	  printf("Signal n°%d : %d\n",i,compteur[i]);
	  i++;
	}

      printf("\n#== TOTAL : %d ==#\n",cpt2++);
      exit (0);
    }
}
  

int main()
{
  int nb;
  compteur = (int *) calloc(32,sizeof(int));
  
  sigemptyset(&sig);
  sigprocmask(SIG_SETMASK,&sig,NULL);
  
  nb=1;
  
  while (nb != 32)
    {
      sigact[nb].sa_mask = sig;
      sigact[nb].sa_flags = 0;
      sigact[nb].sa_handler = sig_handler;
      sigaction(nb,&sigact[nb],NULL);
      nb++;
    }

  while (1) {sigsuspend(&sig);}
 
  return EXIT_SUCCESS;
}
