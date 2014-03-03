#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "stack.h"

// STRUCT DER SKAL GIVES TIL rowmult fra absalon, men med pos, for at genskabe
//result ved slutningen
typedef struct {
  int pos;
  int *row_a;
  int *matrix_b;
  int a_length, b_columns;
  int *row_result;
} ttask_t;


// Klargører stacken
stack_t* stack;
stack_t* stack_result;
stack_init(stack);
stack_init(stack_result);


/* Zero the result row of ints in memory, compute dot product of row A with
* all columns of B, return. */
void* rowmult(void *arg) {
  int i, j;
  ttask_t *t;
  
  t = (ttask_t*) arg;
  
  for (j = 0; j < t->b_columns; j++) {
    t->row_result[j] = 0;
  }
  for (i = 0; i < t->a_length; i++) {
    for (j = 0; j < t->b_columns; j++) {
      t->row_result[j] += t->row_a[i] * t->matrix_b[i * (t->b_columns) + j];
    }
  }
  stack_push(stack_result,t);
  return NULL;
}

int main()  {
  int i;
  pthread_t pid[MAX_THREADS];
  //Definere størrelsen af matricer (pt. arbejdes der kun på kvadratiske matricer
  int size = 5;
  // sætter size - 1, grundet 0 indexering
  size = size-1;
  //Klargøre matricerne, a, b og result matricen.
  int* a = malloc(sizeof(int)*size*size);
  int* b = malloc(sizeof(int)*size*size);
  int* result = malloc(sizeof(int)*size*size);
  int* arow;
  
  //Pusher en masse ttask_t på stacken.
  ttask_t* task = malloc(sizeof(ttask_t));
  for(i = 0; i < size; i++) {
    arow = malloc(sizeof(int)*size);
    for(int n = 0; n < size; n++) {
      arow[n] = a[i*size + n];
    }
    task->pos = i;
    task->row_a = arow;
    task->row_result = result;
    task->matrix_b = b;
    task->a_length = size;
    task->b_columns = size;
    stack_push(stack,task);
  }
  
  /* Nu starter vi tråde som starter på row_mult på stacken,
  og ligger resultatet over på stack_result, på ny stack */
  int n = 0;
  for(i = 0; i < size; i++) {
    if(n >= MAX_THREADS) {
      while(n < MAX_THREADS) {
        pthread_join(pid[n],NULL);
        n = 0;
      }
      pthread_create(&pid[n],NULL,rowmult,stack_pop(stack));
      n++;
    }
  }
  
  // Henter fra stack_result over i result matricen.
  for(i = 0; i < size; i++) {
    ttask_t* arg = (ttask_t*) stack_pop(stack_result);
    result[arg->pos*size + i] = arg->row_result[i];
  }
  
  // Printer result matricen
  for(i = 0; i < size; i++) {
    if(i % (size+1) == 0) {
      printf("\n");
    }
    printf("%d  ", result[i]);
  }
   
}
