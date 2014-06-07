#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void creeNProcess_it (int n)
{
  /*Si fork() > 0 : pere sinon c est le fils*/
  pid_t pid_fils;
  int nombre = 0;  
  
  while (nombre < n)
    {
      pid_fils = fork();

      if (pid_fils == -1)
	{
	  perror("creaProcess_it");
	  exit (1);
	}
      else if (pid_fils == 0)
	{
	  printf("Fils n° %d : %d\n",nombre+1,getpid());
	  exit (0);
	}
      else if (pid_fils > 0)
	{
	  printf("Pere : %d\n",getpid());
	  nombre++;
	}
    }
}

int creeNProcess_rec (int n)
{
  pid_t pid_fils;
  
  if (n == 0) return 0;
  
  if ((pid_fils = fork()) == 0)
    {
      return 1;
    }
  return creeNProcess_rec(--n);
  
  
}

int main ()
{
  int nombreFils;
  int rec;
  nombreFils = 3;
  
  printf("Version Iterative\n");
  creeNProcess_it(nombreFils);
  
  sleep (5);
  printf("\nVersion Recursive\n");
  rec = creeNProcess_rec(nombreFils);
  
  if (rec)
    {
      printf("Fils (pid : %d)\n",getpid());
    }
  else
    {
      printf("Pere (nombre de fils : %d)\n",nombreFils);
    }
  
  return EXIT_SUCCESS;
}
