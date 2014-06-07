#include "myqueue.h"

int init_fd=0;

void printqueue(void)
{
  int i = shm->ind;
  fprintf(OUT_W,"shm->ind = %d\n",shm->ind);
  while(-1!=shm->cl_mq[i].next)
    {
      fprintf(OUT_W,"%2d %2d\n",shm->cl_mq[i].mq_msg->prio,shm->cl_mq[i].next);
      i=shm->cl_mq[i].next;
		
    }
  fprintf(OUT_W,"%2d %2d\n",shm->cl_mq[i].mq_msg->prio,shm->cl_mq[i].next);
}

mqd_t mq_open(const char * myqueue,int flags,int mod)
{
  int fd;
  int i = 0;
  int curr;
  
  if(0==init_fd)
    {	
      while(i!=MAX_FD)
	{
	  mqfd[i]=0;
	  i++;
	}
      init_fd=1;
    }
  
  i=0;
  
  while((mqfd[i]!=0) && (i<MAX_FD))
    i++;
  
  if(i==MAX_FD) return -1;
  curr=i;
	
  char *tmp=(char *)calloc(32,sizeof(char));
  
  if((fd=shm_open(myqueue,flags, mod))==-1)
    {
      perror("shm_open");
      return -1;
    }
  
  ftruncate(fd,sizeof(_shm_mq));
  
  if((shm=mmap(NULL,sizeof(_shm_mq),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
    {
      perror("mmap");
      return -1;
    }
  
  tmp=strcpy(tmp,myqueue);
  tmp=strcat(tmp,"_read\0");
  
  while(i!=32)
    {
      shm->mq.blocked_readers[i]=tmp[i];
      i++;
    }
  i=0;	

  tmp=strcpy(tmp,myqueue);
  tmp=strcat(tmp,"_write\0");
  
  while(i!=32)
    {
      shm->mq.blocked_writers[i]=tmp[i];
      i++;
    }
  i=0;	
	
  free(tmp);
	
  if((shm->mutex_read = sem_open(shm->mq.blocked_readers,O_CREAT|O_EXCL|O_RDWR,0666,0))==SEM_FAILED)
    perror("sem_open");
  
  if((shm->mutex_write = sem_open(shm->mq.blocked_writers,O_CREAT|O_EXCL|O_RDWR,0666,MAX_MSG))==SEM_FAILED)
    perror("sem_open");
  
  if(sem_init(&shm->mutex_access,1,1)==-1)
    perror("sem_init");
  
  if(sem_init(&shm->mutex_update_nb,1,1)==-1)
    perror("sem_init");
  
  shm->mq.mq_flags=((flags&O_NONBLOCK)?(1):(0));
  shm->mq.mq_maxmsg=MAX_MSG;
  shm->mq.mq_msgsize=MAX_SIZ_MSG;
  shm->mq.mq_curmsgs=0;

	
  shm->mq.nb_blocked_writers=0;	
  shm->mq.nb_blocked_readers=0;	
  shm->mq.subscriber=0;
  shm->mq.notification_sig=-1;
  mqfd[curr]=1;
  shm->ind=-1;
  shm->init=0;
  
  while(i!=MAX_MSG)
    {
      shm->cl_mq[i].next=-2;
      i++;
    }
  fprintf(OUT_W,"open success");
  
  return curr;
}

int mq_close(mqd_t fd)
{
  /*int i=0;*/
  if(mqfd[fd]!=1)
    return -1;
  
  shm->init = 0;
  sem_close(shm->mutex_read);
  sem_close(shm->mutex_write);
  mqfd[fd]=0;
  
  return EXIT_SUCCESS;
}

int mq_unlink(/*const char * s*/)
{
  /*char *tmp=(char *)calloc(32,sizeof(char));*/
  
  sem_destroy(&shm->mutex_access);
  sem_destroy(&shm->mutex_update_nb);
  sem_unlink(shm->mq.blocked_writers);
  sem_unlink(shm->mq.blocked_readers);
  munmap(shm,sizeof(_shm_mq));
  
  return EXIT_SUCCESS;
}

int mq_getattr(mqd_t fd,struct mq_attr * attr)
{
  if(mqfd[fd]!=1)
    return -1;
  
  *attr=shm->mq;
  
  return EXIT_SUCCESS;
}

int mq_setattr(mqd_t fd ,struct mq_attr * new,struct mq_attr * old)
{
  if(mqfd[fd]!=1)
    return -1;
  
  if(old!=NULL)
    mq_getattr(fd,old);
  
  if((new->mq_flags)&O_NONBLOCK)
    shm->mq.mq_flags=1;
  else
    shm->mq.mq_flags=0;
  
  return EXIT_SUCCESS;
}

int mq_send(mqd_t fd,const char * s,size_t len, unsigned prio)
{
  int i = 0;
  int ind=0;
  int pind=-1;
  int curr;
  int pempty=0;
  /*mqhd * cl_tmp;
    mqhd * cl_tmp_p=NULL;*/
  
  if(mqfd[fd]!=1)
    return -1;
  
  if(len > MAX_SIZ_MSG)
    return -1;
  
  if( ((int)prio > 32) || ((int)prio < 0) )
    return -1;
  
  if(1 == shm->mq.mq_flags)
    {
      if(0 != sem_trywait(shm->mutex_write))
	return -1;
    }
  else
    {
      sem_wait(shm->mutex_write);
    }
  
  fprintf(OUT_W,"w : write sem\n");
  sem_wait(&shm->mutex_access);
  fprintf(OUT_W,"w : access mutex\n");
	

  if(0 == shm->init)
    {
      while(i != MAX_MSG)
	{
	  shm->mq.queue[i].length=-1;
	  i++;
	}
      i=0;
      shm->init = 1;
    }
  fprintf(OUT_W,"after shm->init\n");
  
  while( -1 != (int)shm->mq.queue[i].length) i++;
  
  fprintf(OUT_W,"found place in queue\n");
  curr=i;
  i=0;
  shm->mq.queue[curr].prio=prio;
  shm->mq.queue[curr].length=len;
  strncpy(shm->mq.queue[curr].data,s,len);
  fprintf(OUT_W,"insertion\n");

  if(-1==shm->ind)
    {
      pempty=1;
      fprintf(OUT_W,"test init\n");
      shm->cl_mq[0].mq_msg=&(shm->mq.queue[curr]);
      shm->cl_mq[0].next=-1;
      shm->ind=0;
    }
  else
    {
      ind=shm->ind;
      
      while(((shm->cl_mq[ind].mq_msg->prio)>prio) && (-1!=(shm->cl_mq[ind].next)))
	{			
	  fprintf(OUT_W,"while -1\n");
	  pind=ind;
	  ind=shm->cl_mq[ind].next;
	}
      
      fprintf(OUT_W,"%d %d %d %d\n",shm->cl_mq[ind].mq_msg->prio,prio,ind,curr);
      
      if(-1==pind)
	{
	  fprintf(OUT_W,"test pind -1\n");
	  
	  while(-2!=shm->cl_mq[i].next)
	    i++;
	  
	  if(shm->cl_mq[ind].mq_msg->prio<=prio)
	    {
	      fprintf(OUT_W,"test pind -1-1\n");
	      shm->ind=i;
	      shm->cl_mq[i].next=ind;
	      shm->cl_mq[i].mq_msg=&(shm->mq.queue[curr]);
	    }
	  else
	    {
	      fprintf(OUT_W,"test pind -1-2\n");
	      shm->cl_mq[ind].next=i;
	      shm->cl_mq[i].next=-1;
	      shm->cl_mq[i].mq_msg=&(shm->mq.queue[curr]);
				
	    }
	}
      else if((shm->cl_mq[ind].mq_msg->prio)<=prio)
	{
	  fprintf(OUT_W,"else if\n");
	  while(-2!=shm->cl_mq[i].next)
	    i++;
	  shm->cl_mq[i].next=ind;
	  shm->cl_mq[i].mq_msg=&(shm->mq.queue[curr]);
	  shm->cl_mq[pind].next=i;
	}
      else
	{
	  fprintf(OUT_W,"else\n");
	  
	  while(-2!=shm->cl_mq[i].next)
	    i++;
	  
	  shm->cl_mq[i].next=-1;
	  shm->cl_mq[i].mq_msg=&(shm->mq.queue[curr]);
	  shm->cl_mq[ind].next=i;
	}
    }
  
  fprintf(OUT_W,"before notif tests\n");			
  sem_getvalue(shm->mutex_write,&shm->mq.nb_blocked_writers);
  fprintf(OUT_W,"nbbr : %d   nbbw : %d\n",shm->mq.nb_blocked_readers,shm->mq.nb_blocked_writers);
  
  if((1==pempty) && (-1!=shm->mq.notification_sig) && (0<=(shm->mq.nb_blocked_readers)))
    {
      fprintf(OUT_W,"%d send sig %d to %d\n",getpid(),shm->mq.notification_sig,shm->mq.subscriber); 
      kill(shm->mq.subscriber,shm->mq.notification_sig);
      fprintf(OUT_W,"KIll done\n");
      mq_notify(fd,NULL);
      fprintf(OUT_W,"unsubscribing notif\n");
    }
  else
    {
      fprintf(OUT_W,"no notification\n");
    }
  
  printqueue();
  fprintf(OUT_W,"printqueue done\n");
  sem_post(&shm->mutex_access);
  fprintf(OUT_W,"w : access mutex released\n");
  sem_post(shm->mutex_read);
  fprintf(OUT_W,"w : post read sem\n");
  
  return EXIT_SUCCESS;
}

ssize_t mq_receive(mqd_t fd, char * buff,size_t len, unsigned * prio)
{
  if(1!=mqfd[fd])
    {
      fprintf(OUT_W,"1\n");
      return -1;
    }
  
  if(32>len)
    {
      fprintf(OUT_W,"2\n");
      return -1;
    }
  
  if(NULL==prio)
    {
      fprintf(OUT_W,"3\n");
      return -1;
    }
  
  int ind;
  
  if(1==shm->mq.mq_flags)
    {
      fprintf(OUT_W," r : flags == 1\n");
      if(0!=sem_trywait(shm->mutex_read))
	return -1;
    }
  else
    {
      sem_wait(&shm->mutex_update_nb);
      shm->mq.nb_blocked_readers--;
      sem_post(&shm->mutex_update_nb);
      fprintf(OUT_W,"r : sem_wait\n");
      sem_wait(shm->mutex_read);
    }
  
  sem_wait(&shm->mutex_update_nb);
  shm->mq.nb_blocked_readers++;
  sem_post(&shm->mutex_update_nb);
  fprintf(OUT_W,"r : reader sem\n");
  sem_wait(&shm->mutex_access);
  sem_getvalue(shm->mutex_write,&shm->mq.nb_blocked_writers);
  fprintf(OUT_W,"r : access mutex\n");
  fprintf(OUT_W,"nbbr : %d   nbbw : %d\n",shm->mq.nb_blocked_readers,shm->mq.nb_blocked_writers);
  ind=shm->ind;
  
  if(-1==shm->cl_mq[shm->ind].next)
    shm->ind=-1;
  else
    shm->ind=shm->cl_mq[shm->ind].next;
  
  strcpy(buff,shm->cl_mq[ind].mq_msg->data);
  *prio=shm->cl_mq[ind].mq_msg->prio;
  shm->cl_mq[ind].mq_msg->length=-1;
  shm->cl_mq[ind].next=-2;


  sem_post(&shm->mutex_access);
  fprintf(OUT_W,"r : access mutex released\n");
  sem_post(shm->mutex_write);
  fprintf(OUT_W,"r : post write sem\n");

  return EXIT_SUCCESS;
}

mqd_t mq_notify(mqd_t fd, const struct sigevent* event)
{
  if(mqfd[fd]!=1)
    return -1;
  
  if((event==NULL) && (0!=shm->mq.subscriber))
    {
      shm->mq.subscriber=0;
      shm->mq.notification_sig=-1;
      fprintf(OUT_W,"sucessfully unsubscribed\n");
      return EXIT_SUCCESS;
    }

  if((SIGEV_SIGNAL!=event->sigev_notify) && (SIGEV_NONE!=event->sigev_notify))
    {
      return -1;
    }
  
  if(SIGEV_NONE==event->sigev_notify)
    {
      shm->mq.subscriber=getpid();
      return EXIT_SUCCESS;
    }
  
  shm->mq.notification_sig=event->sigev_signo;
  shm->mq.subscriber=getpid();
  fprintf(OUT_W," %d sucessfully subscribed for %d\n",shm->mq.subscriber,shm->mq.notification_sig);
  
  return EXIT_SUCCESS;
}
