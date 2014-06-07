#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "converters.h"

#define _POSIX_SOURCE 1

int main (int argc, char *argv[])
{
  int i;
  double val;
  char target[4], src[4];

  if (argc != 3)
    {
      printf("############################\n");
      printf("Donner deux arguments\n");
      printf("liste : EUR GBP USD JPY CNY \n");
      printf("Ex : ./CONVERT USD 100.0 \n");
      printf("############################\n");
      exit (1);
    }

  strcpy(src,argv[1]);
  val = atoi(argv[2]);

  printf("############################\n");
  printf("Conversion for : %s %f\n",src,val);
  printf("############################\n\n");

  for (i=0; i < 5; i++)
    {
      strcpy(target, determine_currency(i));
      
      if (fork() == 0)
	{
	  printf("- %s %f\n",target, convert(src, target, val));
	  exit (1);
	}
    }

  while (wait(0) != -1);
  printf("\n");

  return EXIT_SUCCESS;
}
