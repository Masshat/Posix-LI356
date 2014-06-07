#define _POSIX_SOURCE 1

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "converters.h"

int fdr,fdw,fdwr;
char * argv1,*argv2;
void sigint_handler(/*int sig*/)
{
  printf("sigint reçu\n");
  close(fdr);
  close(fdw);
  close(fdwr);
  unlink(argv1);
  unlink(argv2);
  exit(0);
}

int main(int argc,char ** argv)
{
  if(argc != 3)
    {
      printf("invalid arguments");
      return EXIT_FAILURE;
    }
  
  argv1=argv[1];
  argv2=argv[2];
  
  struct sigaction act;
  sigset_t mask;
  
  act.sa_handler=sigint_handler;
  act.sa_flags=0;
  act.sa_mask=mask;
  
  sigaction(SIGINT,&act,NULL);
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
  
  while(1)
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
	  write(fdw,&len,sizeof(int));
	  write(fdw,buf,strlen(buf));
	  d=convert(devise,buf,atof(montant));
	  write(fdw,&d,sizeof(double));
	  i++;
	}
      i=0;
    }
  
  return EXIT_SUCCESS;
}
