#include "Ficindex.h"

/*                                                                                           
                  .-:/++o+/:.                                          .:/+o++/:-.                  
            .-/+osssyyyyyyso/:.                                      .:/osyyyyyyssso+/-.            
          .:/oyyyyyyyyyyyyyyso/:-               `..`               -:/osyyyyyyyyyyyyyyo/:.          
            .:/oyyyyyyyyyyyyyyso/:-`       `-/+oyyyyo+/-`        -//ssyyyyyyyyyyyyyyo/:-            
              .:/+syyyyyyyyyyyyyys//-`   `ooyyyyyyosohyyoo    `-//ssyyyyyyyyyyyyys+/:.              
                `:/+syyyyyyyyyyyyyys//-`  //syyooo+ooyys:/  `-//syyyyyyyyyyyyyys+/:`                
                  `:/+syyyyyyyyyyyysso/+::+sshy/``sssyhss+:-//ossyyyyyyyyyyyys+/:`                  
                    `-:+syyyyysssooo++///:osyhhyo+yyyhyyo/.----::/+oosyyyyys+:-`                    
                      `-:+ssooo+//:.``    :/:./+osso+/.:/:       ```..-/+s+:-`                      
                      `./+o++:.`                 ``           `          `..`                       
                    `:/oo+-`           `-:/:.             .::/.`             `                      
                  .:+o+-`           `:oo+:.`           -+so/-`                                      
                `:+o/. `.-:+.     -oys/:`           ./yyo:.`                  :s/.                  
               ./+/` .:/hdds:   :sdy/-`           .ohho:.                   -sdh:.                  
              -//.    `sddd:. -sdd+/:--.`       `+hds//---.               .ohdd+:                   
             .:.      :ddd+:`+hddy+/:--syo.    -yddho+::-+hs:           `+hdddh:`                   
            `-       `hddy:.sdddy/-`   .ddh`  /hddh+:.`   sdd:        `/hyyddd/-                    
            .        +ddd/-sdddo:`      ddd/`:dddh/-      odds.      :yy+-hddy:                     
                    .ddds:/ddds:`      -ddd/.hddd/-       ydds-    -sho:.oddd:.                     
                    sddh:.yddd:.       sddy::dddo:       :ddd/.  .oho/. .dddo:                      
                   :ddd+- hdds-       :ddh/-/ddd:.      `yddo: .+hs/-.`.sddd/:...```   .            
                  `hddy:` sdd+-      -hdh/- :ddh-      `sddo:./hdsoyyyhhdddyso++//-/  -`            
                  +ddd:.  :dd+.     :hdy/-  `ydh.     .sdh+:/yddhhyyo+/hddh:`````   .:.             
                 -ddho:    /hh-  `-ohy+:`    .yd/` `.+yyo:.:yso/:-.```/ddd/-      .//-              
                 +o+/-`     ./++/+++:.`       `:++/+++/-.  .-.`      `hdds:     `/+/.               
                  ``           ````               ````               /hs+:.   ./o+:`                
                                                                     `.``  `-+o+:.                  
                     -``                                                `-+oo++.                    
                   .+++o/-`                                        ``./++o+ss+++.                   
                 `/+/oyyyys+/-..```            `    `         ``.://+ooossyyyyo/+/`                 
                -o/+yyyyyyyyyysoo//:----:++/+ooo--.-oo+//++///++o++sssyyyyyyyyyy+/o-                
              `/+/syyyyyyyyyyyys+/+..../syyyyhy.-:-:.yyyshyy:-...+/+syyyyyyyyyyyys/+/`              
             .o/+yyyyyyyyyyyys+/+-      :+/:/+s+/--/os+/://-      -+/+syyyyyyyyyyyy+/o.             
            -o/syyyyyyyyyyyyo/+:               -+ss+-               :+/oyyyyyyyyyyyys/o-            
           :+:ssssssyyyyyys//+`                                      `+//syyyyyyssssss:+:           
           .`.`````.syyyyo/o-                                          -+/oyyyys.`````.`.           
                    /yyy+/+`                                            `+/+syy/                    
                    `ss/+:                                                :+/ss`                    
                     -:o.                                                  .o:.                     
                      :.                                                    .:                      
                                                                                              

*/
int tab[16] = {-64,-64,-64,-64,-64,-64,-64,-64,-64,-64,-64,-64,-64,-64,-64,-64};

/*=======================*/
/*Ouvre un fichier indexe*/
/*=======================*/
int indopen(const char *path, int flags, int ind, mode_t mode)
{
  int buf,desc, exist;
  struct stat stat_info;
  exist=0;
  
  exist = !(access(path,F_OK) == -1);
  stat(path,&stat_info);
  exist = exist ? !(S_ISDIR(stat_info.st_mode)) : 0;

  if ( (desc = open(path,flags,mode)) == -1)
    {
      perror("Erreur ouverture : open\n");
      return -1;
    }

  if ( (!exist && (flags & O_CREAT)) || ( (exist && (flags & O_TRUNC))) )
    {
      tab[desc] = ind;
      if (write(desc,&ind,sizeof(int)) == -1)
	{
	  perror("Erreur ouverture : write\n");
	  return -1;
	}
    }
  else 
    {
      read(desc,&buf,sizeof(int));
      tab[desc] = buf;
    }

  return desc;
}

/*=========================*/
/* Ferme un fichier indexe */
/*=========================*/
int indclose(int fd)
{
  if (tab[fd] == -64)
    {
      perror("Descripteur de fichier n'existe pas \n");
      return -1;
    }
  /*fermeture de fd grace a la fonction fclose*/
  return close(fd);
}

/*========================================*/
/* Lit des donnees dans un fichier indexe */
/*========================================*/
ssize_t indread(int fd, void *buf, size_t nbytes)
{
  if (tab[fd] == -64)
    {
      perror("Descripteur de fichier n'existe pas \n");
      return -1;
    }
  if (nbytes < tab[fd])
    {
      perror("indread");
      return -1;
    }
  return read(fd,buf,nbytes);
}

/*==========================================*/
/* Ecrit des donnees dans un fichier indexe */
/*==========================================*/
ssize_t indwrite(int fd, const void *buf, size_t nbytes)
{
  if (tab[fd] == -64)
    {
      perror("Descripteur de fichier n'existe pas \n");
      return -1;
    }
  
  if (tab[fd] > nbytes)
    {
      perror("indwrite");
      return -1;
    }
  
  return write(fd,buf,nbytes);
}

/*===========================================*/
/* Deplace le curseur dans un fichier indexe */
/*===========================================*/
off_t indlseek(int fd, off_t offset, int whence)
{
  off_t position;
  if (tab[fd] == -64)
    {
      perror("Descripteur de fichier n'existe pas \n");
      return -1;
    }
  
  position = offset*tab[fd];

  if (whence == SEEK_SET)
    {
      return lseek (fd,position+sizeof(int),whence);
    }
  return lseek (fd,position,whence);
}

/*====================================================*/
/* Recherche un enregistrement dans un fichier indexe */
/*====================================================*/
void *indsearch(int fd, int (*cmp)(void *, void *), void *key)
{
  int error;
  void *tocmp;
  
  tocmp = malloc (tab[fd]);
  error = 0;

  if (tocmp == NULL)
    {
      perror("indsearch allocation\n");
      return -1;
    }
  
  while ( (error=read(fd,tocmp,tab[fd])) != 0 && error != -1)
    {
      /*comparaison octet par octet*/
      if (cmp(tocmp,key) == 1)
	{
	  return tocmp;
	}
    }
  free(tocmp);
  return NULL;
}

/*==================================*/
/* Change l'indexation d'un fichier */
/*==================================*/
int indxchg(char *oldfic, char *newfic, unsigned int newind)
{ 
  int oldesc, newdesc, err=0, oldind, taille;                                  
  void *mot;                                                                   
  struct stat etat;                                                            
                                                                               
  if(access(oldfic,F_OK)==-1)
    { 
      printf("%s: n'esiste pas \n",oldfic);                                      
      return -1;                                                                 
    }     
  
  stat(oldfic,&etat);                                                          
  
  if((oldesc=indopen(oldfic, O_RDONLY, 0,etat.st_mode)) ==-1)
    {
      perror("erreur sur indopen\n");                                            
      return -1;                                                                 
    }     
  
  oldind=tab[oldesc];                                                           
  
  if((newdesc=indopen(newfic, O_CREAT|O_WRONLY|O_TRUNC, newind,etat.st_mode)) == -1)
    {                                                                           
      perror("erreur sur indopen\n");                                            
      return -1;                                                                 
    }     
  
  taille = ( (newind>oldind) ? newind : oldind );                            
  mot=(void *) malloc(taille);                                               
  
  while (((err=indread(oldesc,mot,oldind)) !=0) && err!=-1) {indwrite(newdesc,mot,newind);}
  
  indclose(newdesc);                                                         
  indclose(oldesc);                                                          
  
  return EXIT_SUCCESS;                                                                    
}                                                          
