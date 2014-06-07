#define _XOPEN_SOURCE 700

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> /*A INCLURE SINON NE COMPILE PAS*/

#include "converter.h"

currency * shmc;

int main(int argc,char ** argv)
{
  int fd;
  int i = 0;
  
  /*Si le nombre d'arguments n'est pas egal a 4, on quitte le programme*/
  if(argc != 4)
    {
      printf("invalid arguments : <shm_client> <devise> <\"montant\">\n");
      return EXIT_FAILURE;
    }
    
  /*creation d'un segment de memoire partagee, si erreur on quitte le programme*/
  if((fd=shm_open(argv[1],O_RDWR,0600))==-1)
    {
      perror("shm_open");
      exit(0);
    }
  
  /*attachement d'un segment de memoire partagee (en Lecture et Ecriture), si erreur on quitte le programme*/
  if((shmc=mmap(NULL,sizeof(currency),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap");
      exit(0);
    }
  
  /*copie de la devise dans le segment de memoire partagee, changement en double le montant et mise de celui-ci dans le segment de memoire partagee*/
  strcpy(shmc->devise,argv[2]);
  shmc->montant=atof(argv[3]);
  
  /*affichage de la devise et du montant*/
  printf("%s %f\n",shmc->devise,shmc->montant);
  
  /*semaphore*/
  sem_post(&shmc->mutexf);
  printf("wait\n");
  
  /*semaphore*/
  sem_wait(&shmc->mutexp);
  
  printf("# %s : %f\n",shmc->devise,shmc->montant);
  
  /*on affiche les differentes valeurs (conversions)*/
  while(i!=5)
    {
      printf("%s : %f\n",determine_currency(i),shmc->rmontant[i]);
      i++;
    }
  
  /*destruction du segment de memoire partagee*/
  munmap(shmc,sizeof(currency));
  
  return EXIT_SUCCESS;
}
