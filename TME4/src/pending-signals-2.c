#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define _POSIX_SOURCE 1

sigset_t sig;
sigset_t sigpendings;
struct sigaction sigact;

int sip;
int cpt=0;

void sig_int(/*int sig*/)
{
  printf("%d",cpt++);
}

int main()
{
  sigemptyset(&sig);
  sigaddset(&sig,SIGINT);
  sigaddset(&sig,SIGQUIT);
  sigprocmask(SIG_SETMASK,&sig,NULL);
  sigact.sa_mask=sig;
  sigact.sa_flags=0;
  sigact.sa_handler=sig_int;
  sigaction(SIGINT,&sigact,NULL);
  sleep(5);
  sigdelset(&sig,SIGINT);
  sigprocmask(SIG_SETMASK,&sig,NULL);
  sigpending(&sigpendings);
  
  int i = 2;
  
  while(i!=28)
    {
      if((sip=sigismember(&sigpendings,i))==-1) {printf("error sigismember\n");}
      else if(sip==1) {printf("sig no %d is pending\n",i);}
      else {printf("%d, not\n",i);}
      i++;
    }
  
  return EXIT_SUCCESS;
}
