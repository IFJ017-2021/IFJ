/**
 *  @file stack.h
 *
 *  @brief Header file for stack.c.
 *
 *  @author <xhajek51> Vojtěch Hájek
 */

#ifndef IFJ_STACK_H
#define IFJ_STACK_H
#define MAX_STACK 50

#include "tokenList.h"

/**
 * @brief stack struct.
 */
typedef struct {
    token_ptr array[MAX_STACK];
    int topIndex;
} Stack;

/**
 * @brief Initialize of stack
 * @param stack Pointer to the stack struct
 */
void Stack_Init( Stack *stack );

/**
 * @brief Returns token from stack top
 * @param list Pointer to the stack struct
 * @param token Pointer to target variable
 */
void Stack_Top( Stack *stack, token_ptr *token);

/**
 * @brief Removes value from top of the stack
 * @param list Pointer to the stack struct
 */
void Stack_Pop( Stack *stack );

/**
 * @brief Returns not null value when stack is empty
 * @param list Pointer to the stack struct
 */
int Stack_IsEmpty( Stack *stack );

/**
 * @brief Return not null value when stack is full
 * @param list Pointer to the stack struct
 */
int Stack_IsFull( Stack *stack );

/**
 * @brief Adds value to top of the stack
 * @param list Pointer to the stack struct
 * @param token Value to insert
 */
void Stack_Push( Stack *stack, token_ptr token);

#endif
