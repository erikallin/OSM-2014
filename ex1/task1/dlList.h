#ifndef DL_LIST_H
#define DL_LIST_H

typedef int bool;
typedef void item; /* We store pointers, cuz why not? */

typedef struct node_ {
  item           *thing;
  struct node_   *ptr;
} node;
typedef struct dlist_ {
  node *head, *tail;
} dlist;


void insert(dlist *this, item* thing, bool atTail);
item* extract(dlist *this, bool atTail);
void reserve(dlist *this);

item* search(dlist *this, bool (*matches(item*)));
#endif // DL_LIST_H
