#define _POSIX_SOURCE 1

#include <string.h>
#include "converters.h"

int main(int argc,char ** argv)
{
  pid_t pid;
  int tube[2];

  if(pipe(tube)==-1)
    {
      perror("pipe");
      return -1;
    }
  
  if(argc != 3)
    {
      printf("invalid arguments");
      return EXIT_FAILURE;
    }
  
  printf("Conversion for %s: %f\n",argv[1],atof(argv[2]));
  int i = 0;
  
  if((pid=fork())==-1)
    {
      perror("fork");
      return EXIT_FAILURE;
    }

  if(pid==0)
    {
      sleep(1);
      char * buf=(char *)calloc(20,sizeof(char));
      double d;
      int len;
      
      while(i!=5)
	{
	  buf=determine_currency(i);
	  len=strlen(buf);
	  write(tube[1],&len,sizeof(int));
	  write(tube[1],buf,strlen(buf));
	  d=convert(argv[1],buf,atof(argv[2]));
	  write(tube[1],&d,sizeof(double));
	  i++;
	}
    }
  else
    {
      char * buff=(char *)calloc(20,sizeof(char));
      double dd;
      int n;
      
      while(i!=5)
	{
	  if(read(tube[0],&n,sizeof(int))==-1)
	    {
	      perror("read");
	      exit(0);
	    }
	  
	  if((n=read(tube[0],buff,n))==-1)
	    {
	      perror("read");
	      exit(0);
	    }
	  
	  buff[n]='\0';
	  printf("%s ",buff);
	  
	  if((n=read(tube[0],&dd,sizeof(double)))==-1)
	    {
	      perror("read");
	      exit(0);
	    }
	  printf("%.3f\n",dd);
	  i++;
	}
      close(tube[0]);
      close(tube[1]);
    }
  
  return EXIT_SUCCESS;
}
