#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void reset(char * buffer)
{
  int i = 0;
  while(i!=100)
    {
      buffer[i]='\0';
      i++;
    }
}

int main(int argc, char *argv[])
{
  FILE *fd;
  char readc1='a';
  int i;
  char * buffer=(char *)malloc(sizeof(char)*100);
  
  /*Re-initialisation du buffer*/
  reset(buffer);
  
  if(argc < 3 || argc > 3)
    {
      fprintf(stderr,"Il faut 2 arguments, et pas %d !!\n",argc-1);
      fprintf(stderr,"Usage : ./bin/FINDWORD nom_fichier mot_rechercher\n");
      return EXIT_FAILURE;
    }
    
  if ( (fd = fopen(argv[1],"r")) == NULL)
    {
      perror("open");
      return EXIT_FAILURE;
    }

  i=0;
  while(readc1 != EOF)
    {
      i=0;
      while((readc1=fgetc(fd))!=' ' && readc1!='\n' && readc1!=EOF)
	{
	  buffer[i]=readc1;
	  i++;
	}
      buffer[i]='\0';
      if(strncmp(buffer,argv[2],strlen(argv[2]))==0 && readc1 != EOF)
	{
	  printf("%ld : %s\n",ftell(fd)-strlen(buffer)-1, buffer);
	  reset(buffer);
	}
    }
  
  /*Fermeture*/
  fclose(fd);
  
  return EXIT_SUCCESS;
}
