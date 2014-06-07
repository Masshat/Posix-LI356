#define _POSIX_SOURCE 1

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "converters.h"

#define NB_CONVERTERS 5

int fdr,fdw,fdwr;
char * argv1,*argv2;
int nbsigusr=0;

void sigint_handler(int sig)
{
  if(sig==SIGINT)
    {
      printf("sigint reçu\n");
      close(fdr);
      close(fdw);
      close(fdwr);
      unlink(argv1);
      unlink(argv2);
      exit(0);
    }
  else if(sig == SIGUSR1)
    {
      printf("sigusr1 reçu\n");
      nbsigusr++;
    }
}

int main(int argc,char ** argv)
{
  if(argc != 3)
    {
      printf("invalid arguments");
      return 0;
    }

  argv1=argv[1];
  argv2=argv[2];
  
  int status;
  int tube[5][2];
  int tuber[5][2];
  int pid;
  struct sigaction act;
  sigset_t mask;
  
  sigaddset(&mask,SIGUSR1);
  act.sa_handler=sigint_handler;
  act.sa_flags=0;
  act.sa_mask=mask;
  sigaction(SIGINT,&act,NULL);
  sigaction(SIGUSR1,&act,NULL);
  mkfifo(argv[1],S_IRUSR|S_IWUSR);
  mkfifo(argv[2],S_IRUSR|S_IWUSR);

  fdr=open(argv[1],O_RDONLY);
  fdw=open(argv[2],O_WRONLY);
  fdwr=open(argv[1],O_WRONLY);
  
  int len;
  char * devise=(char *)calloc(20,sizeof(char));
  char * montant=(char *)calloc(20,sizeof(char));
  char * buf=(char *)calloc(20,sizeof(char));
  int i = 0;
  double d;
  
  while(i!=NB_CONVERTERS)
    {
      if(pipe(tube[i])==-1)
	{
	  perror("pipe");
	  return -1;
	}
      
      if(pipe(tuber[i])==-1)
	{
	  perror("pipe");
	  return -1;
	}
      
      if((pid=fork())==0)
	{
	  pid+=i;
	  break;
	}
      i++;
    }
  
  while(1)
    {
      i=0;
      
      if(pid>5)
	{
	  read(fdr,&len,sizeof(int));
	  read(fdr,devise,len);
	  read(fdr,&len,sizeof(int));
	  read(fdr,montant,len);
	  printf("requete reçue %s %s\n",devise,montant);

	  while(i!=5)
	    {

	      buf=determine_currency(i);
	      len=strlen(buf);
	      write(tube[i][1],&len,sizeof(int));
	      write(tube[i][1],buf,len);
	      len=strlen(devise);
	      write(tube[i][1],&len,sizeof(int));
	      write(tube[i][1],devise,len);
	      d=atof(montant);
	      write(tube[i][1],&d,sizeof(double));
	      /*d=convert(devise,buf,atof(montant));
		write(tube[i][1],&d,sizeof(double));*/
	      i++;
	    }
	  i=0;
	  
	  while(nbsigusr!=NB_CONVERTERS)
	    pause();
	  
	  printf("nbsigusr reçu\n");
	  
	  while(i!=NB_CONVERTERS)
	    {
	      read(tuber[i][0],&len,sizeof(int));
	      write(fdw,&len,sizeof(int));
	      read(tuber[i][0],devise,len);
	      write(fdw,devise,len);
	      read(tuber[i][0],&d,sizeof(double));
	      write(fdw,&d,sizeof(double));
	      i++;
	    }
	  i=0;
	}
      else
	{
	  printf("%d\n",pid);
	  read(tube[pid][0],&len,sizeof(int));
	  read(tube[pid][0],buf,len);
	  read(tube[pid][0],&len,sizeof(int));
	  read(tube[pid][0],devise,len);
	  read(tube[pid][0],&d,sizeof(double));
	  printf("*********%s ,%s, %f********** \n",devise,buf,d);
	  d=convert(devise,buf,d);
	  printf("kill(getppid(),SIGUSR1)\n");
	  kill(getppid(),SIGUSR1);
	  len=strlen(buf);
	  write(tuber[pid][1],&len,sizeof(int));
	  write(tuber[pid][1],buf,len);
	  write(tuber[pid][1],&d,sizeof(double));
	}
    }
  
  return EXIT_SUCCESS;
}
