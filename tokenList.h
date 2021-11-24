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
 * @brief List dispose
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_Dispose(DLList *list);

/**
 * @brief Inserts new element at the beginning of the list
 * @param list Pointer to the structure of a doubly linked list
 * @param token Value to insert at the beginning of the list
 */
void DLL_InsertFirst(DLList *list, token_ptr token);

/**
 * @brief Inserts new element at the end of the list
 * @param list Pointer to the structure of a doubly linked list
 * @param token Value to insert at the end of the list
 */
void DLL_InsertLast(DLList *list, token_ptr token);

/**
 * @brief Sets first list item to active
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_First(DLList *list );

/**
 * @brief Sets last list item to active
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_Last(DLList *list );

/**
 * @brief The token parameter returns the value of the first list item
 * @param list Pointer to the structure of a doubly linked list
 * @param token Pointer to target variable
 */
void DLL_GetFirst(DLList *list, token_ptr *token);

/**
 * @brief The token parameter returns the value of the last list item
 * @param list Pointer to the structure of a doubly linked list
 * @param token Pointer to target variable
 */
void DLL_GetLast(DLList *list, token_ptr *token);

/**
 * @brief Deletes first element in the list
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteFirst(DLList *list);

/**
 * @brief Deletes last element in the list
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteLast(DLList *list);

/**
 * @brief Deletes an element after the active item list
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteAfter(DLList *list);

/**
 * @brief Deletes an element before the active item list
 * @param list Pointer to the structure of a doubly linked list
 */
void DLL_DeleteBefore(DLList *list);

/**
 * @brief Insert an element after active item list
 * @param list Pointer to the structure of a doubly linked list
 * @param token Value to insert into the list after the currently active element
 */
void DLL_InsertAfter(DLList *list, token_ptr token);

/**
 * @brief Insert an element before active item of list
 * @param list Pointer to the structure of a doubly linked list
 * @param token Value to insert into the list before the currently active element
 */
void DLL_InsertBefore(DLList *list, token_ptr token);

/**
 * @brief The token parameter returns the value of the active list item
 * @param list Pointer to the structure of a doubly linked list
 * @param token Pointer to target variable
 */
void DLL_GetValue(DLList *list, token_ptr *token);

/**
 * @brief Overwrites the contents of the active list item.
 * @param list Pointer to the structure of a doubly linked list
 * @param token New value of active item
 */
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