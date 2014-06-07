#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define _POSIX_SOURCE 1

void handler(int sig)
{
  fprintf(stderr,"fils terminé (signal : %d)\n",sig);
}

void mywait ()
{
  sigset_t mask;
  
  sigfillset (&mask);
  sigdelset (&mask,SIGCHLD);
  sigsuspend (&mask); 
}

int main()
{
  sigset_t mask;
  struct sigaction action;
  pid_t pid;
  
  sigemptyset (&mask);

  action.sa_flags = 0;
  action.sa_handler = handler;
  action.sa_mask = mask;

  sigaddset (&mask,SIGCHLD);
  sigaction(SIGCHLD,&action,NULL);
  sigprocmask(SIG_SETMASK,&mask,NULL);

  pid = fork();
  
  if (pid == -1)
    {
      perror("fork");
      return EXIT_FAILURE;
    }
  else if (pid == 0)
    {
      exit(0);
    }
  else
    {
      sleep(1);
      mywait();
    }
  
  return EXIT_SUCCESS;
}
