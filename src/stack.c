#include "../include/stack.h"

bool stack_push(Stack *stack, u16 element)
{
    if (stack->top + 1 == STACK_SIZE)
        return false;
    stack->elements[stack->top++] = element; 
    return true;
}

u16 stack_pop(Stack *stack)
{
    if (!stack->top)
        return false;
    return stack->elements[--stack->top];
}

