#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>



int main(int argc,char ** argv)
{
  char v[2];
  char *path=(char *)malloc(sizeof(char)*100);
  
  if(argc<2)
    {
      strcpy(v,"5");
      strcpy(path,argv[0]);
    }
  else
    {
      if(!strcmp(argv[1],"0"))
	return 0;
      sprintf(v,"%d",atoi(argv[1])-1);
      strcpy(path,argv[2]);
    }
  
  printf("\nPID : %d\n",getpid());
  printf("n°%s : %s %s \n",v,argv[0],path);
  execlp(argv[0],argv[0],v,path,NULL);
  perror("execlp");
  
  return EXIT_SUCCESS;
}
