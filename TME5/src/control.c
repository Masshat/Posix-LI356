#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

#define NFIN 5

int sig;
sigjmp_buf env;

void sig_hand (int sig)
{
  siglongjmp (env,sig);
}

int main()
{
  int cpt;
  struct sigaction sigact;
  
  cpt = 0;
  sigact.sa_handler = sig_hand;
  sigaction (SIGINT,&sigact,NULL);
  
  alarm (5);

  while (cpt != NFIN)
    {
      sig = sigsetjmp(env,1);
      
      if (sig == SIGINT)
	{
	  cpt++;
	  printf("****************\nSignal SIGINT reçu\n******************\n\n");
	}
      else
	{
	  pause();
	}
    }
  
  return EXIT_SUCCESS;
}
