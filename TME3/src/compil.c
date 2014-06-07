#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define db printf("FILE : %s - LINE : %d\n",__FILE__,__LINE__)

int main(int argc, char ** argv)
{
  if(argc==1)
    return 0;
  
  int i=1;
  int status[argc-1];
  char *arg_vect[argc+3];
  char *arg_vect2[6];

  arg_vect2[0]="gcc";
  arg_vect2[1]="-c";
  arg_vect2[2]="-Wall";
  arg_vect2[3]="-I.";
  arg_vect2[5]="-o";
  arg_vect2[7]=NULL;
  
  while(i!=argc)
    {
      arg_vect2[4]=argv[i];
      arg_vect2[6]=(char *)calloc(strlen(argv[i])+1,sizeof(char));
      strcpy(arg_vect2[6],argv[i]);
      arg_vect2[6][strlen(argv[i])-1]='o';
      
      if(fork()==0)
	{
	  /*execv("/usr/bin/gcc",arg_vect2);*/
	  fprintf(stderr,"%s %s %s %s %s %s %s\n",
		  arg_vect2[0],
		  arg_vect2[1],
		  arg_vect2[2],
		  arg_vect2[3],
		  arg_vect2[4],
		  arg_vect2[5],
	          arg_vect2[6]);
	  execvp("gcc",arg_vect2);
	  perror("execv1");
	  exit(-1);
	}
      
      free(arg_vect2[6]);
      i++;
    }
  
  i=1;
  
  while(i!=argc)
    {
      wait(&(status[i-1]));
      
      if(!WIFEXITED(status[i-1]))
	{
	  printf("wrong\n");
	  return 0;
	}
      
      argv[i][strlen(argv[i])-1]='o';
      arg_vect[i-1+3]=argv[i];
      i++;
    }
  
  arg_vect[i-1+3]=NULL;
  arg_vect[0]="gcc";
  arg_vect[1]="-o";
  arg_vect[2]="affiche";
  
  i=3;
  fprintf(stderr,"%s",arg_vect[0]);
  while(arg_vect[i] != NULL)
    {
      fprintf(stderr," %s",arg_vect[i]);
      i++;
    }
  fprintf(stderr," %s %s\n",arg_vect[1],arg_vect[2]);
  execv("/usr/bin/gcc",arg_vect);
  perror("execv2");
    
  return EXIT_SUCCESS;
}
