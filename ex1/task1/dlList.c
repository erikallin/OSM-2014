#include <stdio.h>
#include <stdlib.h>
#include "dlList.h"

void insert(dlist *this, item *thing, bool atTail) {
  node *newNode = malloc(sizeof(node));
  newNode->thing = thing;


  if (atTail) {
    newNode->ptr = &(this->tail);
    this->tail->ptr = (int)this->tail->ptr ^ (int)newNode->ptr;
    this->tail = newNode;
  }
  else {
    newNode->ptr = &(this->head);
    this->head->ptr = (int)this->head->ptr ^ (int)newNode->ptr;
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

  if (atTail)
    ext = this->tail;
  else
    ext = this->head;

  return ext;
}
