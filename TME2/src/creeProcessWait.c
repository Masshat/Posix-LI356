#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int waitFilsQuelconque (int n)
{
  pid_t pid_fils;
  pid_t tab[n];
  int num, fils=0, status;

  printf("Attente d'un fils\n");

  for (num=0; num<n; num++)
    if ((tab[num] = fork()) == 0)
      {
	printf("Fils - (pid:%d) -\n",getpid());
	fils=1;
	break;
      }
  
  if (fils==0)
    {
      do
	{
	  pid_fils = wait(&status);
	  if (WIFEXITED(status) && pid_fils != -1)
	    printf("Pere - %d\n",pid_fils);
	}while (pid_fils != tab[1]);
    }
  
  return EXIT_SUCCESS;
}

int waitTousFils (int n)
{
  pid_t fils;
  int status, i=0;

  printf("\nWait de tous les fils\n");
  
  /*Creation des processus fils*/
  for(;i<n;i++)
    if(fork() == 0)
      {
	printf("Fils n°%d (pid : %d)\n",i,getpid());
	exit (1);
      }
  
  for (i=0; i < n ;i++)
    {
      fils = wait(&status);
      printf("- Fils n°%d terminé -\n",i);
    } 
  
  if(WIFEXITED(status))
    printf("Pere (fils tous termines,status : %d)\n",WEXITSTATUS(status));
  
  return EXIT_SUCCESS;
}

int waitDernierFils (int n)
{
  pid_t fils;
  int status, i=0;
  
  printf("\nWait du dernier fils\n");
  
  /*Creation des processus fils*/
  for(;i<n;i++)
    if(fork() == 0)
      {
	printf("Fils (pid : %d)\n", getpid());
	exit (1);
      }
  
  for (i=0; i < n ;i++)
    fils = wait(&status);
      
  if(WIFEXITED(status))
    printf("Pere (fils %d termine, status : %d)\n", fils,WEXITSTATUS(status));
  
  return EXIT_SUCCESS;
}

int main()
{
  int numFils;
  int choix;
  numFils = 3;

  system("clear");
  printf("Quels tests voulez-vous faire ?\n\t 1 - waitFilsQuelconque\n\t 2 - waitTousFils\n\t 3 - waitDernierFils\n\t 4 - Quitter\n");
  scanf("%d",&choix);

  switch (choix)
    {
    case 1:
      {
	waitFilsQuelconque(numFils);
      }
      break;
    case 2:
      {
	waitTousFils(numFils);
      }
      break;
    case 3:
      {
	waitDernierFils(numFils);
      }
      break;
    case 4:
      break;
    default:
      printf("ERREUR : vous devez entrer un nombre valide\n");
      break;
    }

  return EXIT_SUCCESS;
}
