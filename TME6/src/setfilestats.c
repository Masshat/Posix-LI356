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
  /*PREMIER PARAMETRE : operation (e->effacer, r->renommer et c->changer les droits)*/
  /*DEUXIEME PARAMETRE : nom du fichier a modifier*/
  /*TROISIEME PARAMETRE : nom du nouveau fichier (renommage) ou nouveau droits (r->read-only ou w->read-write)*/
  
  char act[2]={'\0','\0'};
  char * dir=(char *)malloc(sizeof(BUFSIZ));
  char * opt=(char *)malloc(sizeof(BUFSIZ));
  struct stat buf;
  
  if(argc < 2)
    {
      fprintf(stderr,"usage : \n\t1er : e(ffacer),r(ename),c(hmod)\n\t2eme : nom de fichier\n\t3eme(options) : ** si r : nouveau nom ** sinon c : r (pour read-only) ou w (pour read-write)\n");
      return 0;
    }
  else if(argc >= 3)
    {
      act[0]=argv[1][0];
      strcpy(dir,argv[2]);
      if(argc == 4)
	{
	  if((act[0]=='r') || (act[0]=='c'))
	    {
	      strcpy(opt,argv[3]);
	    }
	  else
	    {
	      fprintf(stderr,"Mauvais Arguments");
	      return 0;
	    }
	}
    }
  if(stat(dir,&buf) == -1)
    {
      perror("ERREUR stat");
      return EXIT_FAILURE;
    }
  if(S_ISREG(buf.st_mode) == 0)
    {
      fprintf(stderr,"Besoin d'un fichier");
      return 0;
    }
  switch(act[0])
    {
    case 'e':
      fprintf(stderr,"Supression de %s\n",dir);
      unlink(dir);
      break;
    case 'r':
      fprintf(stderr,"Renommage de %s en %s\n",dir,opt);
      rename(dir,opt);
      break;
    case 'c':
      switch(opt[0])
	{
	case 'r':
	  fprintf(stderr,"Changement des droits de %s en lecture seule (pour le groupe)\n",dir);
	  chmod(dir,S_IRUSR|S_IWUSR|S_IRGRP);
	  break;
	case 'w':
	  fprintf(stderr,"Changement des droits de %s en lecture et ecriture (pour le groupe)\n",dir);
	  chmod(dir,(buf.st_mode |S_IWGRP | S_IRGRP));
	  break;
	default :
	  fprintf(stderr,"ERREUR mode");
	  return EXIT_FAILURE;
	}
      break;
    default :
      fprintf(stderr,"ERREUR action %c\n",act[0]);
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
