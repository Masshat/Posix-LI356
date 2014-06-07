#include <stdlib.h>
#include <stdio.h>
#include <list.h>
#include <stack.h>

struct list_type *l;

/*Initialisation*/
int init_stack()
{
  init_list(l);
  return size();  
}

/*Empilement*/
int push (void* element)
{
  insert_head(l,element);
  return size();
}

/*Depilement*/
void* pop() 
{
  extract_head(l);
}

/*Taille*/
int size()
{
  return list_size(l);
}
