/**** decoder.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <decoder.h>

#define _POSIX_SOURCE 1

int magicsq[3][3] = {{4, -1, 8}, {-1, -1, -1}, {2, -1, 6}};

int check() {
  int i, j, sums[8];
  for (i = 0; i < 8; i++) 
    sums[i] = 0;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      sums[i] += magicsq[i][j];
      sums[i+3] += magicsq[j][i];
      if (i == j) sums[6] += magicsq[i][j];
      if ((i+j) == 2) sums[7] += magicsq[i][j];
    }
  }
  int result = 1;
  i = 1;
  while ((i < 8) && (result == 1)) {
    if (sums[0] != sums[i])
      result = 0;
    i++;
  }
  return result;
}

void display() {
  int i, j;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++)
      printf("  %d", magicsq[i][j]);
    printf("\n");
  }
  printf("\n");
}

int solve(int x)
{
  int sum ;                                                                    
  magicsq[0][1] = x ;                                                          
  sum = magicsq[0][0] + magicsq[0][1] + magicsq[0][2] ;                        
  /* b */                                                                      
  magicsq[1][0] = sum - (magicsq[0][0] + magicsq[2][0]) ;                      
  /* e */                                                                      
  magicsq[2][1] = sum - (magicsq[2][0] + magicsq[2][2]) ;                      
  /* d */                                                                      
  magicsq[1][2] = sum - (magicsq[0][2] + magicsq[2][2]) ;                      
  /* c */                                                                      
  magicsq[1][1] = sum - (magicsq[1][0] + magicsq[1][2]) ;                      
  
  if(check()) 
    {                                                                
      display() ;                                                                
      return 1 ;                                                                 
    }                                                                            
  
  return EXIT_SUCCESS;   
}

int main(int argc, char **argv)
{	
  int nbsolution = 0, status, i;
  pid_t pid;
  
  system("clear");
  printf("##########################\n      CARRE MAGIQUE       \n##########################\n");

  for (i=0; i < 9;i++)
    {
      pid = fork();
      if (!pid) return solve(i);
    }

  for (i=0; i < 9;i++)
    {
      wait(&status);
      nbsolution += WEXITSTATUS(status);
    }

  printf("Il y a %d solution(s)\n",nbsolution);

  return EXIT_SUCCESS;
}
