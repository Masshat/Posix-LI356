#include <list.h>
#include <stdlib.h>
#include <stdio.h>

#define db printf("ERROR == FILE : %s, LINE : %d ==\n",__FILE__,__LINE__)

void init_list(struct list_type *l) {
  l->head = NULL;
  l->tail = NULL;
  l->taille = 0;
}
	
	
void insert_head(struct list_type *l, void* element) {
  cell* new_cell = malloc(sizeof(cell));
  new_cell->content = element;
  new_cell->previous = NULL;
  new_cell->next = l->head;
  
  if (l->head != NULL)
    l->head->previous = new_cell;
  
  l->head = new_cell;
  
  if (l->tail == NULL)
    l->tail = l->head;
}


void* extract_head(struct list_type *l) {
  
  void* backup = l->head->content;
  cell* suiv = l->head->next;
  free(l->head);
  l->head = suiv;
  l->taille--;
  
  return backup;
}


void* extract_tail(struct list_type *l) {
  void* backup = l->tail->content;
  cell* prec = l->tail->previous;
  
  free(l->tail);
  l->tail = prec;
  l->taille--;
  
  return backup;
}


int list_size(struct list_type *l) {
  return l->taille;
}
