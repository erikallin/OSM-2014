#include <stdio.h>
typedef int bool;

typedef void item; /*we store pointers */

typedef struct node_ {
item *thing;
struct node_ *ptr;
} node;

typedef struct dlist_ {
node *head, *tail;
} dlist;

void insert(dlist *this, item* thing, bool atTail) {
node *new;
void *tmpptr;
printf("%p", this->tail);
/*if (atTail != 0) {
new->item = thing;
new->ptr = (this->tail) ^ null; 
printf("%d",(&this));
this->tail->ptr = (this->tail->ptr) ^ &(new); 
}

else {


}
*/
}



item* extract( dlist *this, bool atTail) {

}

void reverse(dlist  *this) {

}

item* search(dlist *this, bool (*matches(item*))) {

}

int main() {
return 1;
}
