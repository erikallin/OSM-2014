#ifndef DL_LIST_H
#define DL_LIST_H

typedef int bool;
typedef void item;

typedef struct node_ {
  item           *thing;
  struct node_   *ptr;
} node;

typedef struct dlist_ {
  node *head, *tail;
} dlist;

/* Inserts an item to either the start or end of the list */
void insert(dlist *this, item* thing, bool atTail);

/* Extracts either the first or last element in the list, remove it from the
 * list and returns the item
 */
item* extract(dlist *this, bool atTail);

/* Flips the direction of the links */
void reverse(dlist *this);

item* search(dlist *this, bool (*matches)(item*));
#endif // DL_LIST_H
