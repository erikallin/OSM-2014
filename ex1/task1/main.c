#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dlList.h"


int main() {
 
 // Tildeler memory/plads til listen samt dens head og tail.
  node *tail = malloc(sizeof(node));
  node *head = malloc(sizeof(node));
  dlist *liste = malloc(sizeof(dlist));
  
 /* Tildeler memory/plads til de elementer, der senere bliver indsat via
    insert. */
  int *i = malloc(sizeof(int));
  int *j = malloc(sizeof(int));
  int *n = malloc(sizeof(int));
  int *k = malloc(sizeof(int));
 
 // Værdier for elementer der senere bliver indsat.
  i = (int*)1;
  j = (int*)2;
  n = (int*)3;
  k = (int*)4;
  
 // Tail og head tildeles værdier. 
  tail->thing = j;
  tail->ptr = head;
  
  head->thing = n;
  head->ptr = tail;
  
 // Funktioner for at teste om bestemte værdier findes i dllist via search.
  bool *eqone(int a) {
    return (bool*)(a == 1);
  }
  
  bool *eqseven(int a) {
    return (bool*)(a == 7);
  }
  
 // Værdier for at tjekke tiden det tager at indsætte elementer.
  clock_t startInsert1, startInsertAll, endInsert1, endInsertAll;
  
  
 /* Head og tail tildeles deres pladser i listen. 
    Der tjekkes herudover, om de har de korrekte værdier via et print. */
  liste->head = head;
  liste->tail = tail;
  printf("Tail: %p\n", liste->tail);
  printf("Head: %p\n", liste->head);
  
  
 /* Der sættes en clock for at tjekke programmets hidtige køretid, 
    og der bliver indsat en række elementer.
    Senere sættes der to "slut" clocks, der senere bruges til at tjekke
    tiden det har taget at indsætte elementerne */
  startInsert1 = clock();
  startInsertAll = clock();
  insert(liste,i,1);
  endInsert1 = clock();
  insert(liste,i,1);
  insert(liste,i,1);
  insert(liste,i,1);
  insert(liste,i,1);
  insert(liste,i,1);
  insert(liste,k,1);
  insert(liste,k,1);
  insert(liste,k,0);
  endInsertAll = clock();
  
  /* Hvorfor har vi denne?
  bool search_item(int a) {
    return (a == -1);
  }
  
  search(liste,search_item);
 */ 
 
  printf("Insertion time for 1 element: %f\n",
  (double)(endInsert1 - startInsert1) / CLOCKS_PER_SEC);
  
  printf("Insertion time for alle elementer: %f\n",
  (double)(endInsertAll - startInsertAll) / CLOCKS_PER_SEC);
  
  printf("%p værdi af thing i tail\n",tail->thing);
  printf("%p værdi af thing i nye tail\n",liste->tail->thing);
 
 /* Denne test er ikke korrekt. Slet eller fix
  printf("%p pointer i nye tail\n",liste->tail->ptr);
  printf("%p gamle tail (skal være lig pointer i nye tail)\n",tail);
 */
  
  printf("Tester om 1 er i listen. Returner %p, hvilket betyder at den er der.\n",
  search(liste, (item*)eqone));
  printf("Tester om 7 er i listen. Returner %p, hvilket betyder at den ikke er det.\n",
  search(liste, (item*)eqseven));
  

  // UDKOMMENTER FOR AT TESTE REVERSE;
  /* DEN VIL LAVE PRINT OM FRA 4,3,2,1 TIL 1,2,3,4 
  reverse(liste);
  */
  printf("\nFØLGENDE ER TEST FOR EXTRACT\n");
  printf("%p er thing i head (rigtigt hvis = 4)\n",liste->head->thing);
  extract(liste,0);
  printf("%p er nu thing i nye head (rigtigt hvis = 3)\n",liste->head->thing);
  extract(liste,0);
  printf("%p er nu thing i nye head (rigtigt hvis = 2)\n",liste->head->thing);
  extract(liste,0);
  printf("%p er nu thing i nye head (rigtigt hvis = 1)\n",liste->head->thing);
  
  return 42;
}
