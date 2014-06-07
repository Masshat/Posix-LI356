/*Programme basé sur le squelette de spy-incomplete.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define _POSIX_SOURCE 1

#define BUFSZ 1024
#define WDSZ 24
#define NBUF 4

int cut_down(char *buf, char **cmd) 
{
  char* s = strtok(buf, " \n");
  int i = 0;
  
  while (s != NULL) 
    {
      cmd[i] = (char*) malloc(sizeof(char) * WDSZ);
      strcpy(cmd[i], s);
      s = strtok(NULL, " \n");
      i++;
    }
  cmd[i] = NULL;
  
  return i;
}


void sig_handler(int sig)
{
  fprintf(stderr,"Signal recu :  %d\n",sig);
}


int main()
{
  int or = 1, command_size, i,nbf=0;
  char buf[BUFSZ];
  char *command[WDSZ];
  pid_t pid[NBUF];
  int status;
  sigset_t maskf;
  struct sigaction sigact;

  sigemptyset(&maskf);

  sigact.sa_flags = 0;
  sigact.sa_handler = sig_handler;
  sigact.sa_mask = maskf;
  sigaction(SIGUSR1,&sigact,NULL);
  sigaddset(&maskf,SIGUSR1);
  sigprocmask(SIG_SETMASK,&maskf,NULL);

 
  while (or> 0)/*Boucle infinie*/
    {
      for (i = 0; i < BUFSZ ; i++) {buf[i] = '\0';}
      
      if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1) {perror("read");}
      else if (or > 0)
	{
	  command_size = cut_down(buf, (char**)command);
	  
	  if((pid[nbf]=fork())==0)
	    {
	      sigfillset(&maskf);
	      sigdelset(&maskf,SIGUSR1);
	      sigsuspend(&maskf);
	      
	      /*Si la commande entrée a exactement un argument*/
	      if(command_size==2)
		{
		  execlp(command[0],command[0],command[1],NULL);
		  perror("execlp");
		  exit(0);
		}
	      /*Si la commande entrée a plus de un argument*/
	      else
		{
		  execvp(command[0],command);
		  perror("execvp");
		  exit(0);
		}
	    }
	  else if(pid[nbf]==-1) {fprintf(stderr,"erreur fork");} /*Si erreur au moment du fork*/
	  else
	    {
	      if(nbf<NBUF-1)
		{
		  printf("nbf++");
		  nbf++;
		}
	      else
		{
		  nbf=0;
		  while(nbf<NBUF)
		    {
		      kill(pid[nbf],SIGUSR1);
		      nbf++;
		    }
		  nbf=0;
		  /*Attente que tous les fils se soient terminés*/
		  while(nbf<NBUF)
		    {
		      waitpid(pid[nbf],&status,0);
		      nbf++;
		    }
		  nbf=0;
		  while(nbf<command_size)
		    {
		      free(command[nbf]);
		      nbf++;
		    }
		  nbf=0;
		}
	    }
	}
    }

  return EXIT_SUCCESS;
}
