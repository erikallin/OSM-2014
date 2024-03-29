#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "stack.h"

void stack_init(stack_t* stack) {
  stack->top = -1;
  pthread_mutex_init(&stack->lock,NULL);
  stack->condVar = 0;
}

int stack_empty(stack_t* stack) {
  while(stack->condVar) { printf("Empty venter\n");}
  return stack->top == -1;
}

void* stack_top(stack_t* stack) {
  while(stack->condVar) {}
  if (!stack_empty(stack))
    return stack->list->head;
  return (int*)-1;
}

void* stack_pop(stack_t* stack) {
  /* Lås tilgangen til stakken så der ikke er samtidighedsproblemer */
  pthread_mutex_lock(&stack->lock);
  stack->condVar = 1;
  if (!stack->top == -1) {
    stack->top--;
    void* a = extract(stack->list,0);
    
    pthread_mutex_unlock(&stack->lock);
    stack->condVar = 0;
    return a;
  }
  
  pthread_mutex_unlock(&stack->lock);
  stack->condVar = 0;
  return (int*)-1;
}

int stack_push(stack_t* stack, void* data) {
  pthread_mutex_lock(&stack->lock);
  stack->condVar = 1;
  stack->top++;
  insert(stack->list,data,0);
  pthread_mutex_unlock(&stack->lock);
  stack->condVar = 0;
  return 0;
}



