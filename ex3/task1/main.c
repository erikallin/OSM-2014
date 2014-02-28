#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "stack.h"



// STRUCT DER SKAL GIVES TIL rowmult (UDLEVERET KODE FRA ABSALON)
typedef struct {
  int *row_a;
  int *matrix_b;
  int a_length, b_columns;
  int *row_result;
} ttask_t;

void output_square_matrix(double *m, int size) {
  int i, j;

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%3.2f  ", m[i * size + j]);
    }
    putchar('\n');
  }
}


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


int main() {

int i, j;
int* matrices,* a,* b,* r;
int size = 2;
ttask_t* t;
stack_t* stack = malloc(sizeof(stack_t));
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
  for(i = 0; i < size; i++)  {
    t = malloc(sizeof(ttask_t));
    t->row_a = a + i * size;
    t->matrix_b = b;
    t->a_length = size;
    t->b_columns = size;
    t->row_result = r + i * size;
    
    printf("yono\n");
    printf("%p\n",(void*)stack);
    printf("%p\n",(void*)t);
    stack_push((stack_t*)stack,(void*)t);
    printf("yolo2\n");
  }




//printf("%p",stack_pop(&stack));

j = 0;
return j ;

}
