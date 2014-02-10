#include "dlList.h"

int main() {

  node *tail = malloc(sizeof(node));
  node *head = malloc(sizeof(node));
  dlist *liste = malloc(sizeof(dlist));
  int *i = malloc(sizeof(int));
  int *j = malloc(sizeof(int));
  i = 5;
  j = 10;
  tail->thing = i;
  tail->ptr = head;

  head->thing = i;
  head->ptr = tail;

 liste->head = head;
 liste->tail = tail;
 printf("%d\n", i);
 printf("%p\n", liste->tail->thing);
 printf("%p\n", liste->head->thing);
 insert(liste,j,1);
 insert(liste,j,0);
 printf("%p\n", liste->tail->thing);
 printf("%p\n", liste->head->thing);
}
