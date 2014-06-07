#define _XOPEN_SOURCE 700

#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h> /*A INCLURE SINON NE COMPILE PAS*/

typedef struct _sharedmem
{
  char sp[BUFSIZ];
  sem_t smutexf;
  sem_t smutexp;
}shared;

shared * shm;

void sigint_handler(int sig)
{
  printf("sigint handler\n");
  munmap(shm,(sizeof(shared)));

  sem_destroy(&shm->smutexf);
  sem_destroy(&shm->smutexp);
  shm_unlink("myshm");
  exit(0);
}


int main(void)
{
  printf("Usage: Write anything (if you want to exit just press CTRL-C)\n");

  sem_unlink("momsemf");
  sem_unlink("momsemp");
  struct sigaction sigact;
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask,SIGINT);
  sigact.sa_mask=mask;
  sigact.sa_flags=0;
  sigact.sa_handler=sigint_handler;
  int fd=shm_open("myshm", O_RDWR|O_CREAT, 0600);
  ftruncate(fd,sizeof(shared));
  
  if((shm=mmap(NULL,sizeof(shared),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap");
      exit(0);
    }
  
  if(sem_init(&shm->smutexf,1,1)==-1)
    {
      perror("sem_init");
      sem_destroy(&shm->smutexf);
      exit(0);
    }
  
  if(sem_init(&shm->smutexp,1,0)==-1)
    {
      perror("sem_init");
      sem_destroy(&shm->smutexp);
      exit(0);
    }
  
  printf("fork\n");
  fflush(stdout);
  
  if(fork()==0)
    {
      int a;
      
      while(1)
	{
	  sem_wait(&shm->smutexf);
	  shm->sp[scanf("%s",shm->sp)+(strlen(shm->sp)>1024)?(1024):(strlen(shm->sp))-1]='\0';
	  sem_post(&shm->smutexp);
	}
    }
  else
    {
      sigaction(SIGINT,&sigact,NULL);
      int i = 0;
     
      while(1)
	{
	  i=0;
	  sem_wait(&shm->smutexp);
	  
	  while(i[shm->sp]!='\0')
	    {
	      printf("%c",toupper(i[shm->sp]));
	      i++;
	    }
	  
	  printf("\n");
	  sem_post(&shm->smutexf);
	}
    }
  
  return EXIT_SUCCESS;
}
