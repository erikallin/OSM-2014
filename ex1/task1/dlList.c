#include <stdio.h>
#include <stdlib.h>
#include "dlList.h"

void insert(dlist *this, item *thing, bool atTail) {
  node *newNode = malloc(sizeof(node));
  newNode->thing = thing;


  if (atTail) {
    newNode->ptr = &(this->tail);
    this->tail->ptr = (int)this->tail->ptr ^ (int) &newNode;
    this->tail = newNode;
  }
  else {
    newNode->ptr = &(this->head);
    this->head->ptr = (int)this->head->ptr ^ (int) &newNode;
    this->head = newNode;
  }
}

/*
item* search(dlist *this, bool (*matches(item*))) {
  item *ret;
  int *p = &this;

  return ret;
}
*/

void reverse(dlist *this) {
  dlist *tmp = this;
  this->head = tmp->tail;
  this->tail = tmp->head;
}

item* extract(dlist *this, bool atTail) {
  item *ext;
  node *address = this->tail->ptr;
  if (atTail){
   ext = this->tail->thing;
   address->ptr = (int)address->ptr  ^ (int) &this->tail;
   printf("%p item i address\n", address->thing);
    printf("%p item i address->ptr\n", address->ptr); 
   this->tail = address; 
   printf("%p\n", address);
   printf("%p ext værdi \n", ext);
   return ext;
}
  else
    ext = this->head;

  return ext;
}

int main() {
/* Test for insert, skal give 5,5,10,10 */
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
 printf("%p\n", liste->tail->thing);
 printf("%p\n", liste->head->thing);
 insert(liste,j,1);
 insert(liste,j,0);
 printf("%p\n", liste->tail->thing);
 printf("%p\n", liste->head->thing);
/* test for extract */
 printf("%p tailaddress\n",&liste->tail);
 extract(liste,1);
 extract(liste,0);
 printf("%p thing i tail\n",liste->tail->thing);
 printf("%p\n",liste->head->thing);



}

