#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "stack.h"

void stack_init(stack_t* stack) {
  stack->top = -1;
}

int stack_empty(stack_t* stack) {
  return stack->top == -1;
}

void* stack_top(stack_t* stack) {
  return stack->data[stack->top];
}

void* stack_pop(stack_t* stack) {
  /* lås tilgangen til stakken så der ikke er samtidighedsproblemer */
  if (!stack_empty(stack))
    return stack->data[stack->top--];

  printf("Stack is empty!\n");
  return (int*)-1;
}

int stack_push(stack_t* stack, void* data) {
  if (stack->top == STACK_MAX_SIZE-1) {
    return 1;
  }
  else {
    stack->data[++stack->top] = data;
    return 0;
  }
}
