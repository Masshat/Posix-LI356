#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define _POSIX_SOURCE 1

sigset_t sig;
sigset_t sigpendings;
int sip;

int main()
{
  sigemptyset(&sig);
  sigaddset(&sig,SIGINT);
  sigaddset(&sig,SIGQUIT);
  sigprocmask(SIG_SETMASK,&sig,NULL);
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
