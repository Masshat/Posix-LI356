#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define _POSIX_SOURCE 1


int creeFils(int nbFils){
  int * tabFils;
  tabFils = calloc (nbFils, sizeof(int));
  int * tabPere;
  tabPere = calloc (nbFils, sizeof(int));
  pid_t pid;
  int i=0, j,status;
  
  printf("\n");
  
  int auxFunction(int num){
    if(num == nbFils){
      for(j=0; j<nbFils; j++){ 
	printf("%d) Pere (%d) -> Fils (%d)\n", j, tabPere[j], tabFils[j]); 
      } 
      return 1;
    }
    if((pid  = fork())==0){
      tabFils[num]=getpid();
      tabPere[num]=getppid();
      auxFunction(num+1);
      exit(2);
    }
    else{
      pid = wait(&status);
      if (WIFEXITED (status)) {
      }
      else{
	return EXIT_FAILURE;
      }
    }
    return EXIT_SUCCESS;
  }
  auxFunction(i);
  return EXIT_SUCCESS;
}


int main ()
{
  int nombre;

  printf("Entrer un nombre : \n");
  scanf ("%d",&nombre);

  creeFils(nombre);
  
  return EXIT_SUCCESS;
}
