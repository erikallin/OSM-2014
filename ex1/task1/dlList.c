#include <stdlib.h>
#include <stdint.h>
#include "dlList.h"

void insert(dlist *this, item *thing, bool atTail) {
  node *newNode = malloc(sizeof(node));
  newNode->thing = thing;

  if (atTail) {
    newNode->ptr = (this->tail);
    this->tail->ptr = (node*)((uintptr_t)this->tail->ptr ^ (uintptr_t)newNode);
    this->tail = newNode;
  }
  else {
    newNode->ptr = (this->head);
    this->head->ptr = (node*)((uintptr_t)this->head->ptr ^ (uintptr_t)newNode);
    this->head = newNode;
  }
}


item* search(dlist *this, bool (*matches)(item*)) {
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
