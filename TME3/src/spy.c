/**** spy.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define _POSIX_SOURCE 1

#define BUFSZ 1024
#define WDSZ 24


int cut_down(char *buf, char **cmd) {
  char* s = strtok(buf, " \n");
  int i = 0;
  while (s != NULL) {
    cmd[i] = (char*) malloc(sizeof(char) * WDSZ);
    strcpy(cmd[i], s);
    s = strtok(NULL, " \n");
    i++;
  }
  cmd[i] = NULL;
  return i;
}


int main()
{

  int or = 1, pid, command_size, i, status, freecpt=0;
  char buf[BUFSZ];
  char *command[WDSZ];

  while (or > 0) 
    {
      for (i = 0; i < BUFSZ ; i++)
	buf[i] = '\0';
      if ((or = read(STDIN_FILENO, buf, BUFSZ)) == -1)
	perror("read");
      else if (or > 0) {
	command_size = cut_down(buf, (char**)command);
	/* A COMPLETER : 
	      -> LANCEMENT DE LA COMMANDE PAR UN FILS
	      -> ATTENTE ET LIBERATION DE LA MEMOIRE PAR LE PERE */

	if((pid=fork())==0)
	  {
	    if(command_size==2)
	      {
		execlp(command[0],command[0],command[1],NULL);
		perror("execlp");
		exit(0);
	      }
	    else
	      {
		execvp(command[0],command);
		perror("execvp");
		exit(0);
	      }
	  }
	else
	  {
	    wait(&status);
	    
	    while(freecpt!=command_size)
	      {
		free(command[command_size]);
		freecpt++;
	      }
	    freecpt=0;
	  }
	
      }
    }
  
  return EXIT_SUCCESS;
}
