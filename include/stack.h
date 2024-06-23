#ifndef STACK
#define STACK

#include <stdbool.h>
#include "constants.h"
#include "types.h"

typedef struct Stack {
    int top;
    u16 elements[STACK_SIZE];
} Stack;

bool stack_push(Stack *stack, u16 element);

u16 stack_pop(Stack *stack);

#endif
