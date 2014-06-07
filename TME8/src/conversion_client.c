#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "converters.h"

#define _POSIX_SOURCE 1

int main(int argc,char ** argv)
{
  if(argc != 5)
    {
      printf("invalid arguments : <nom_tube_requete> <nom_tube_reponse> <devise> <\"montant\">\n");
      return 0;
    }
  
  printf("Conversion for %s: %f\n",argv[3],atof(argv[4]));
  
  int i = 0;
  int fdw=open(argv[1],O_WRONLY);
  int fdr=open(argv[2],O_RDONLY);
  int len;
  
  len=strlen(argv[3]);
  write(fdw,&len,sizeof(int));
  write(fdw,argv[3],len);
  len=strlen(argv[4]);
  write(fdw,&len,sizeof(int));
  write(fdw,argv[4],len);
  
  char * buff=(char *)calloc(20,sizeof(char));
  double dd;
  int n;
  
  while(i!=5)
    {
      if(read(fdr,&n,sizeof(int))==-1)
	{
	  perror("read");
	  exit(0);
	}
      if((n=read(fdr,buff,n))==-1)
	{
	  perror("read");
	  exit(0);
	}
      
      buff[n]='\0';
      printf("%s ",buff);
      
      if((n=read(fdr,&dd,sizeof(double)))==-1)
	{
	  perror("read");
	  exit(0);
	}
      printf("%.3f\n",dd);
      i++;
    }
  close(fdr);
  close(fdw);
  
  return EXIT_SUCCESS;
}
