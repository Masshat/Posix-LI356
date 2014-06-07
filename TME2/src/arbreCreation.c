#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int arg, char* argv[])
{
  int i, j, p, status;

  for (i=0; i<3 ; i++)
    if ((p=fork ()) == 0)
      {
	printf ("i=%d\n", i);
	j=0;
	while (j<i && ((p=fork()) == 0))
	  j++;
	
	if (p==0)
	  printf("j=%d\n",j);
	exit(j);
      } /* if */ 

  for (i=0; i<11; i++)
    if (wait(&status) == -1)
      {
	perror("wait");
	exit (2);
      }

  return EXIT_SUCCESS;
}
