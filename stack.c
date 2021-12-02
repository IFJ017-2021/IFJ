/**
 *  @file stack.c
 *
 *  @brief Implementation of stack.
 *
 *  @author <xhajek51> Vojtěch Hájek
 */

#include "stack.h"
#include "tokenList.h"
#include "error.h"

int STACK_SIZE = MAX_STACK;

void Stack_Init( Stack *stack ) {

    if(stack == NULL){
        err_call(ERR_INTERNAL, NULL);
    }
    else{
        stack->topIndex = -1;
    }
}
/* Stack_Init */

int Stack_IsEmpty( const Stack *stack ) {

    return (stack->topIndex == -1);
}
/* Stack_IsEmpty */

int Stack_IsFull( const Stack *stack ) {

    return (stack->topIndex == STACK_SIZE-1);
}
/* Stack_IsFull */

void Stack_Top( const Stack *stack, token_ptr *dataPtr ) {

    if(!Stack_IsEmpty(stack)){
        *dataPtr = stack->array[stack->topIndex];
    }
    else{
        err_call(ERR_INTERNAL, NULL);
    }
}
/* Stack_Top */

void Stack_Pop( Stack *stack ) {

    if(!Stack_IsEmpty(stack)){
        stack->topIndex--;
    }
}
/* Stack_Pop */

void Stack_Push( Stack *stack, token_ptr token ) {

    if(!Stack_IsFull(stack)){
        stack->array[++stack->topIndex] = token;
    }
    else{
        err_call(ERR_INTERNAL, NULL);
    }
}
/* Stack_Push */
