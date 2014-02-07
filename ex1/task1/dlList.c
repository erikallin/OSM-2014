#include <stdio.h>
#include <stdlib.h>
#include "dlList.h"

void insert(dlist *this, item *thing, bool atTail) {
  if (atTail)
    this->tail = thing;
  else
    this->head = thing;
}


item* search(dlist *this, bool (*matches(item*))) {
  item *ret;
  int *p = &this;

  return ret;
}


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
