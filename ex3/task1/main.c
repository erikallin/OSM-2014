#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "stack.h"

typedef struct {
  stack_t* stack;
  stack_t* stack2;
  void* arg2;
} args;

// STRUCT DER SKAL GIVES TIL rowmult (UDLEVERET KODE FRA ABSALON)
typedef struct {
  int *pos;
  int *row_a;
  int* result;
  int *matrix_b;
  int a_length, b_columns;
  int *row_result;
} ttask_t;

// ROWMULT; UDLEVERET KODE FRA ABSALON
void* rowmult(void *arg) {
  int i, j;
  ttask_t* t;

  t = (ttask_t*) arg;

  for (j = 0; j < t->b_columns; j++) {
    t->row_result[j] = 0;
  }
  for (i = 0; i < t->a_length; i++) {
    for (j = 0; j < t->b_columns; j++) {
      t->row_result[j] += t->row_a[i] * t->matrix_b[i * (t->b_columns) + j];
    }
  }
  return NULL;
}


void output_square_matrix(int *m, int size) {
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%d  ", m[i * size + j]);
    }
    putchar('\n');
  }
}
//Wrapper cus thread crate be hating on return int functions

void* pop(void* arg) {
  args* tmp = (args*) arg;
  stack_push((stack_t*)tmp->stack2,(void*)rowmult(stack_pop(tmp->stack)));
  return arg;
}

void* push(void* arg ) {
  printf("fuck the world");
 args* tmp =(args*) arg;
 stack_push(tmp->stack,tmp->arg2);
 return tmp;
}

void* puttogether(void* arg) {
  args* tmp = (args*) arg; 
  ttask_t* temp = (ttask_t*)(stack_pop(tmp->stack2));
  int* temp2 = (int*)temp->row_result;
  int length = (int) temp->a_length;
  int pos = (int) temp->pos;
  for(int i = 0; i < length; i++) {
  temp2[pos*length+i] = temp2[i];
  } 
 return (void*) 0;
}

int main() {
 args* arg = malloc(sizeof(args));
 pthread_t* pid = (pthread_t*) malloc(5 * sizeof(pthread_t));
 int i ;
 int* matrices,* a,* b,* r;
 int size = 2;
  ttask_t* t;
 stack_t* stack = malloc(sizeof(stack_t));
 stack_t* stack2 = malloc(sizeof(stack_t));
 arg->stack = stack;
 arg->stack2 = stack2;
 printf("%p\n",(void*)stack);
 stack_init(stack);
 printf("%d\n",stack->top);
  matrices = (int*) malloc(sizeof(int) * size * size * 3);
    if (matrices == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    a = matrices;
    b = matrices + size * size;
    r = matrices + size * size * 2;

// Make a stack with the arguments needed to be cool-like
 b[1] = 1;
 b[0] = 1;
 a[0] = 1;
 a[1] = 1;
  for(i = 0; i < size; i++)  {
    t = malloc(sizeof(ttask_t));
    t->row_a = a + i * size;
    t->matrix_b = b;
    t->a_length = size;
    t->b_columns = size;
    t->row_result = r + i * size;
    t->result = r;
    arg->arg2 = t;
    
    printf("%p\n",(void*)stack);
     
    pthread_create(&pid[0],NULL,push,&arg);
    pthread_join(pid[0],NULL);
  }
 printf("meh");
  for(i = 0; i < size; i++) {
   printf("wat");
     pthread_create(&pid[i],NULL,pop,&arg);    
     pthread_join(pid[i],NULL);
  }
  printf("den nåede hertil");
/*  for(i = 0; i < size; i++) {
     pthread_join(pid[i],NULL);
  }*/
  for(i = 0; i < size; i++) {
  printf("den nåede ned til putt");
    pthread_create(&pid[i],NULL,puttogether,&arg);
    pthread_join(pid[i],NULL);
  } 

 output_square_matrix(r, 2);



//printf("%p",stack_pop(&stack));

return i ;

}
