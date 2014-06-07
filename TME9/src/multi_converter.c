#define _XOPEN_SOURCE 700

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h> /*A INCLURE SINON NE COMPILE PAS*/

#include "converter.h"

#define _POSIX_SOURCE 1
#define NB_CONVERTERS 5


currency * shm;
currency * shmc;
char * argv1;

void sigint_handler(int sig)
{
  printf("sigint hangler\n");
  
  fflush(stdout);
  sem_destroy(&shm->mutexf);
  sem_destroy(&shm->mutexp);
  sem_destroy(&shm->check);
  munmap(shm,sizeof(currency));
  shm_unlink("_myshm");
  
  sem_destroy(&shmc->mutexf);
  sem_destroy(&shmc->mutexp);
  munmap(shmc,sizeof(currency));
  shm_unlink(argv1);
  
  exit(0);
}

int main(int argc,char ** argv)
{
  struct sigaction sigact;
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask,SIGINT);
  sigact.sa_mask=mask;
  sigact.sa_flags=0;
  sigact.sa_handler=sigint_handler;
  
  
  argv1=(char *)malloc(sizeof(char)*20);
  strcpy(argv1,argv[1]);
  
  /*Si le nombre d'arguments est different de 2 alors on quitte le programme*/
  if(argc != 2)
    {
      printf("usage : <shm_client>");
      return EXIT_FAILURE;
    }
  
  char * buf=(char *)malloc(sizeof(char)*20);
  int fd;
  
  /*creation d'un segment de memoire partagee, si erreur on quitte le programme*/
  if((fd=shm_open("_myshm", O_RDWR|O_CREAT, 0600))==-1)
    {
      perror("shm_open");
      exit(0);
    }
  
  
  ftruncate(fd,sizeof(currency));
  int fdc;
  
  /*creation d'un segment de memoire partagee, si erreur on quitte le programme*/
  if((fdc=shm_open(argv1, O_RDWR|O_CREAT, 0600))==-1)
    {
      perror("shm_open");
      exit(0);
    }
  
  ftruncate(fdc,sizeof(currency));
  
  /*#== CLIENT ==#*/
  
  /*attachement d'un segment de memoire partagee (en Lecture et Ecriture), si erreur on quitte le programme*/
  if((shmc=mmap(NULL,sizeof(currency),PROT_READ|PROT_WRITE,MAP_SHARED,fdc,0))==MAP_FAILED)
    {
      perror("mmap");
      exit(0);
    }
  
  /*initialisation de de la semaphore, si erreur alors on quitte le programme*/
  if(sem_init(&shmc->mutexf,1,0)==-1)
    {
      perror("sem_init");
      sem_destroy(&shmc->mutexf);
      exit(0);
    }
  
  /*initialisation de de la semaphore, si erreur alors on quitte le programme*/
  if(sem_init(&shmc->mutexp,1,0)==-1)
    {
      perror("sem_init");
      sem_destroy(&shmc->mutexp);
      exit(0);
    }

  
  // !
  /*#== FILS ==#*/
  
  /*attachement d'un segment de memoire partagee (en Lecture et Ecriture), si erreur on quitte le programme*/
  if((shm=mmap(NULL,sizeof(currency),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap");
      exit(0);
    }
  
  /*initialisation de de la semaphore, si erreur alors on quitte le programme*/
  if(sem_init(&shm->mutexf,1,0)==-1)
    {
      perror("sem_init");
      sem_destroy(&shm->mutexf);
      exit(0);
    }
  
  /*initialisation de de la semaphore, si erreur alors on quitte le programme*/
  if(sem_init(&shm->mutexp,1,0)==-1)
    {
      perror("sem_init");
      sem_destroy(&shm->mutexp);
      exit(0);
    }
  
  /*initialisation de de la semaphore, si erreur alors on quitte le programme*/
  if(sem_init(&shm->check,1,0)==-1)
    {
      perror("sem_init");
      sem_destroy(&shm->mutexp);
      exit(0);
    }
  
  // !
  int i = 0;
  int ind=NB_CONVERTERS+1;
  printf("run\n");
  
  while(i!=NB_CONVERTERS)
    {
      if(fork()==0)
	{
	  ind=i;
	  buf=determine_currency(ind);
	  break;
	}
      i++;
    }
  
  i=0;
  
  if(ind>NB_CONVERTERS) // pere
    {
      printf("inwait\n");
      sigaction(SIGINT,&sigact,NULL);
      
      while(1)
	{
	  sem_wait(&shmc->mutexf);
	  printf("reveived %s %f\t",shmc->devise,shmc->montant);
	  strcpy(shm->devise,shmc->devise);
	  shm->montant=shmc->montant;
	  
	  while(i!=NB_CONVERTERS)
	    {
	      sem_post(&shm->mutexf);
	      i++;
	    }
	  
	  i=0;
	  	  
	  while(i!=NB_CONVERTERS)
	    {
	      sem_wait(&shm->mutexp);
	      i++;
	    }
	  
	  i=0;
	  
	  while(i!=NB_CONVERTERS)
	    {
	      sem_post(&shm->check);
	      i++;
	    }
	  
	  i=0;
	  
	  memcpy(shmc->rmontant,shm->rmontant,sizeof(double)*NB_CONVERTERS);
	  printf("post mutexp\n");
	  sem_post(&shmc->mutexp);
	}
    }
  else // fils
    {
      while(1)
	{
	  sem_wait(&shm->mutexf);
	  shm->rmontant[ind]=convert(shm->devise,buf,shm->montant);
	  /*sem_post(&shm->mutexf);
	    sem_wait(&shm->mutexp);*/
	  sem_post(&shm->mutexp);
	  sem_wait(&shm->check);
	}
    }
  
  return EXIT_SUCCESS;
}
