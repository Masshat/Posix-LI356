#include <list.h>
#include <fifo.h>
#include <stdlib.h>
#include <stdio.h>

struct list_type *l;

int init_queue()
{
  l = (struct list_type*) malloc (sizeof(struct list_type));
  
  if (l == NULL)
    {
      perror("init_queue");
      exit(EXIT_FAILURE);
    }
  
  init_list(l);
  return size();
}

int queue(void* element)
{
  cell* new_cell = malloc(sizeof(cell));

  if (new_cell == NULL)
    {
      perror("queue");
      exit(EXIT_FAILURE);
    }
  
  new_cell->content = element;
  new_cell->next = NULL;
  new_cell->previous = l->tail;
  l->tail = new_cell;
  l->tail->next = new_cell;
  if(l->head == NULL)
    {
      l->head = l->tail;
    }
  l->tail->next = l->head;
  l->taille++;
  
  return size();
}

void* dequeue()
{
  extract_tail(l);
}

int size()
{
  return list_size(l);
}
