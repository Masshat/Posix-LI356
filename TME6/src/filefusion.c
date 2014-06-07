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
  FILE *fd1;
  FILE *fd2;
  FILE *fd3;
  char readc1='a';
  char readc2='a';
  int i;
  char * buffer=(char *)malloc(sizeof(char)*100);
  
  /*Re-initialisation du buffer*/
  reset(buffer);
  
  if(argc < 4 || argc > 4)
    {
      fprintf(stderr,"Il faut 3 fichiers, et pas %d !!\n",argc-1);
      return EXIT_FAILURE;
    }
  
  /*Test d'ouverture du premier fichier*/
  if((fd1=fopen(argv[1],"r")) == NULL)
    {
      perror("open");
      return EXIT_FAILURE;
    }
  
  /*Test d'ouverture du deuxieme fichier*/
  if((fd2=fopen(argv[2],"r")) == NULL)
    {
      perror("open");
      return EXIT_FAILURE;
    }

  /*Test d'ouverture du troisieme fichier*/
  if((fd3=fopen(argv[3],"w+")) == NULL)
    {
      perror("open");
      return EXIT_FAILURE;
    }
  
  /*lecture du fichier 1 et 2*/
  while(readc1 != EOF && readc1 != EOF)
    {
      i=0;
      /*Lecture du fichier 1 jusqu'a trouver un espace*/
      while((readc1=fgetc(fd1))!=' ' && readc1!='\n' && readc1!=EOF)
	{
	  buffer[i]=readc1;
	  i++;
	}
      buffer[i]=' ';
      i++;
      /*Lecture du fichier 2 jusqu'a trouver un espace*/
      while((readc2=fgetc(fd2))!=' '&& readc2!='\n' && readc2!=EOF)
	{
	  buffer[i]=readc2;
	  i++;
	}
      buffer[i]=' ';
      /*Ecriture dans le fichier 3*/
      fprintf(fd3,"%s",buffer);
      reset(buffer);
    }
  
  /*Fermeture des fichiers*/
  fclose(fd1);
  fclose(fd2);
  fclose(fd3);
 
  return EXIT_SUCCESS;
}
