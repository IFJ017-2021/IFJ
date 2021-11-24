/**
 *  @file tokenList.h
 *
 *  @brief Header file for tokenList.c (Doubly linked list of tokens).
 *
 *  @author <xpoliv06> Tomáš Polívka
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"

/**
 * @brief Doubly linked list of tokens
 */
typedef struct {
    token_ptr first;
    token_ptr active;
    token_ptr last;
} DLList;

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_Init(DLList *list);

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_Dispose(DLList *list);

void DLL_InsertFirst(DLList *list, token_ptr token);

void DLL_InsertLast(DLList *list, token_ptr token);

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_First(DLList *list );

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_Last(DLList *list );

void DLL_GetFirst(DLList *list, token_ptr *token);

void DLL_GetLast(DLList *list, token_ptr *token);

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteFirst(DLList *list);

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteLast(DLList *list);

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteAfter(DLList *list);

/**
 * @brief List initialization
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteBefore(DLList *list);

void DLL_InsertAfter(DLList *list, token_ptr token);

void DLL_InsertBefore(DLList *list, token_ptr token);

void DLL_GetValue(DLList *list, token_ptr *token);

void DLL_SetValue(DLList *list, token_ptr token);

/**
 * @brief Moves the activity to the next list element.
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_Next(DLList *list);

/**
 * @brief Moves the activity to the previous list element.
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_Previous(DLList *list);

/**
 * @brief Check the activity of list.
 * @param list Pointer to the structure of a doubly linked list
 * @return 1 - is active
 * @return 0 - is not active
 */
int DLL_IsActive(DLList *list);