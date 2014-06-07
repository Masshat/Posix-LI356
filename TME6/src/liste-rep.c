#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

DIR *pt_dir;
struct dirent *dir_ent;

int main(int argc, char ** argv)
{
  mode_t mode;
  char *dir;
  dir=(char *)malloc(sizeof(BUFSIZ));
  mode=0;
  
  if(argc <= 2)
    {
      if(argc == 2)
	strcpy(dir,argv[1]);
      else
	strcpy(dir,"./");
    }
  else
    {
      printf("Nombre d'arguments INCORRECT\n");
    }

  if( (pt_dir = opendir(dir)) == NULL)
    {
      if(errno == ENOENT)
	{
	  mkdir(dir,mode);
	  return EXIT_SUCCESS;
	}
      else
	return EXIT_FAILURE;
    }
  
  while( (dir_ent = readdir(pt_dir)) != NULL)
    printf("%s\n",dir_ent->d_name);
  
  closedir(pt_dir);
  return EXIT_SUCCESS;
}
