#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dlList.h"

void insert(dlist *this, item *thing, bool atTail) {
  node *newNode = malloc(sizeof(node));
  newNode->thing = thing;

  if (atTail) {
    newNode->ptr = (this->tail);
    this->tail->ptr = (node*)((uintptr_t)this->tail->ptr ^ (uintptr_t) newNode);
    this->tail = newNode;
  }
  else {
    newNode->ptr = (this->head);
    this->head->ptr = (node*)((uintptr_t)this->head->ptr ^ (uintptr_t) newNode);
    this->head = newNode;
  }
}


item* search(dlist *this, bool (*matches(item*))) {
  if (matches(this->head->thing))
    return this->head->thing;

  node *prev = this->head;
  node *next = this->head->ptr;

  while ((node*)((uintptr_t) next->ptr ^ (uintptr_t) prev)) {
    if (matches(next->thing))
      return next->thing; 
    node *tmp = next;
    next = (node*)((uintptr_t) next->ptr ^ (uintptr_t) prev);
    prev = tmp;
  }
   if (matches(this->tail->thing))
      return next->thing; 

  return 0;
}


void reverse(dlist *this) {
  dlist *tmp = this;
  this->head = tmp->tail;
  this->tail = tmp->head;
}

item* extract(dlist *this, bool atTail) {
  item *ext;
  node *address;
  node *cleanup;
  if (atTail){
   address = this->tail->ptr;
   ext = this->tail->thing;
   address->ptr = (node*) ((uintptr_t)address->ptr  ^ (uintptr_t) this->tail);
   cleanup = this->tail;
   this->tail = address; 
   free(cleanup);
   return ext;
}
  else
   address = this->head->ptr;
   ext = this->head->thing;
   address->ptr = (node*) ((uintptr_t)address->ptr  ^ (uintptr_t) this->head);
   cleanup = this->head;
   this->head = address; 
   free(cleanup);
   return ext;
}

int main() {
/* Test for insert, skal give 5,5,10,10 */
  node *tail = malloc(sizeof(node));
  node *head = malloc(sizeof(node));
  dlist *liste = malloc(sizeof(dlist));
  int *i = malloc(sizeof(int));
  int *j = malloc(sizeof(int));
  int *n = malloc(sizeof(int));
  int *k = malloc(sizeof(int));
  i = (int*)1;
  j = (int*)2;
  n = (int*)3;
  k = (int*)4;
  tail->thing = j;
  tail->ptr = head;

  head->thing = n;
  head->ptr = tail;

 bool *eqone(int a) {
   return (bool*)(a == 1);
  }

 bool *eqseven(int a) {
   return (bool*)(a == 7);
  }



 liste->head = head;
 liste->tail = tail;
 printf("%p tail\n", liste->tail);
 printf("%p head\n", liste->head); 
 insert(liste,i,1);
 insert(liste,k,0);
 printf("%p thing i tail\n",tail->thing);
 printf("%p thing i nye tail\n",liste->tail->thing);
 printf("%p pointer i nye tail\n",liste->tail->ptr);
 printf("%p gamle tail (skal være lig pointer i nye tail)\n",tail);

  printf("%p returner 1, hvis 1 er i listen (Er den)\n", search(liste, (item*)eqone));
  printf("%p returner nil, da 7 ikke er i listen\n",search(liste, (item*)eqseven));
/* test for extract */
/* UDKOMMENTER FOR AT TESTE REVERSE; DEN VIL LAVE PRINT OM FRA 4,3,2,1 TIL 1,2,3,4 
  reverse(liste); */
 printf("FØLGENDE ER TEST FOR EXTRACT\n");
 printf("%p thing i head (4)\n",liste->head->thing);
 extract(liste,0);
 printf("%p thing i nye head(3)\n",liste->head->thing);
 extract(liste,0);
 printf("%p thing i nye head(2)\n",liste->head->thing); 
 extract(liste,0);
 printf("%p thing i nye head(1)\n",liste->head->thing);
/* extract(liste,0);
 printf("%p thing i nye head\n",liste->head->thing); */

return 42; 
}

