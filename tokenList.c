/**
 *  @file tokenList.c
 *
 *  @brief Implemantation doubly linked list.
 *
 *  @author <xpoliv06> Tomáš Polívka
 */

#include "tokenList.h"
#include "scanner.h"
void DLL_Init(DLList *list)
{
    list->first = NULL;
    list->last = NULL;
    list->active = NULL;
};

void DLL_Dispose(DLList *list);

void DLL_InsertFirst(DLList *list, token_ptr token);

void DLL_InsertLast(DLList *list, token_ptr token);

void DLL_First(DLList *list);

void DLL_Last(DLList *list);

void DLL_GetFirst(DLList *list, token_ptr *token);

void DLL_GetLast(DLList *list, token_ptr *token);

void DLL_DeleteFirst(DLList *list);

void DLL_DeleteLast(DLList *list);

void DLL_DeleteAfter(DLList *list);

void DLL_DeleteBefore(DLList *list);

void DLL_InsertAfter(DLList *list, token_ptr token);

void DLL_InsertBefore(DLList *list, token_ptr token);

void DLL_GetValue(DLList *list, token_ptr *token);

void DLL_SetValue(DLList *list, token_ptr token)
{
    if(list->active != NULL)
    {
        list->active->data = data;
    }
} /*DLL_SetValue*/

void DLL_Next(DLList *list)
{
    if(list->active != NULL)
    {
        list->active = list->active->next;
    }
} /*DLL_Next*/

void DLL_Previous(DLList *list)
{
    if(list->active != NULL)
    {
        list->active = list->active->prev;
    }
} /*DLL_Previous*/

int DLL_IsActive( DLList *list)
{
    return (list->active != NULL);
} /*DLL_IsActive*/