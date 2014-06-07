#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int rediriger_stdout(char * file)
{
  int fd,fstdout;
  
  if((fd=open(file,O_CREAT | O_WRONLY))==-1)
    {
      perror("open");
      return 0;
    }
  fstdout=dup(STDOUT_FILENO);
  dup2(fd,STDOUT_FILENO);
  
  return fstdout;
}

void restaurer_stdout(int fd)
{
  dup2(fd,STDOUT_FILENO);
}

int main ()
{
  int fd;
  
  printf ("avant la redirection \n") ;
  fd=rediriger_stdout ("fichier.out") ;
  
  printf ("après la redirection \n") ;
  restaurer_stdout (fd) ;
  
  printf ("après avoir restauré stdout \n") ;
  
  return EXIT_SUCCESS ;
}
