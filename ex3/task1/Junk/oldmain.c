#include <stdio.h>
#include "stack.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
stack_t stack;

typedef struct arg_struct {
    stack_t* arg1;
    int* arg2;
}args;

void* pop(void* arg) {
stack_pop(&stack);
return stack_pop(&stack);
}

int main() {
  pthread_t pt[1];


args* arg = malloc(sizeof(args));
arg->arg1 = &stack;
arg->arg2 = (int*)10;

  stack_init(&stack);
 //pthread_create(&pt[0],NULL,stack_push,arg);
printf("%d\n",pthread_create(&pt[0],NULL,&pop,arg));
pthread_join(pt[0],NULL);
//  stack_t *firstStack;


  return 0;
}
