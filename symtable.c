/**
 *  @file symtable.c
 *
 *  @brief // TODO :)
 *
 *  @author <xpoliv06> Tomáš Polívka
 */

#include "symtable.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//void local_bst_init(LocalBSTNodePtr *tree)
//{
//    (*tree) = NULL;
//}
//
//void local_bst_insert(LocalBSTNodePtr *tree, char *key, ValueType type)
//{
//
//}
//
//bool local_bst_search(LocalBSTNodePtr tree, char *key)
//{
//    if(tree == NULL)
//    {
//        return false;
//    }
//
//    if(strcmp(tree->key, key))
//    {
//        return true;
//    }
//    else
//    {
//        if(tree->key > key)
//        {
//            return bst_search(tree->left, key);
//        }
//        else
//        {
//            return bst_search(tree->right, key);
//        }
//    }
//}
//
//void local_bst_delete(LocalBSTNodePtr *tree, char *key)
//{
//    // TODO
//}
//
//void local_bst_dispose(LocalBSTNodePtr *tree)
//{
//    if(*tree != NULL)
//    {
//        local_bst_dispose(&(*tree)->left);
//        local_bst_dispose(&(*tree)->right);
//        free(*tree);
//    }
//    (*tree) = NULL;
//}

void global_bst_init(GlobalBSTNodePtr *tree)
{
    *tree = NULL;
}

bool global_bst_search(GlobalBSTNodePtr tree, char *key)
{
    if(tree == NULL)
    {
        return false;
    }

    if(strcmp(tree->key, key) == 0)
    {
        return true;
    }
    else
    {
        if(strcmp(tree->key, key) > 0)
        {
            return global_bst_search(tree->left, key);
        }
        else
        {
            return global_bst_search(tree->right, key);
        }
    }
}

void global_bst_insert(GlobalBSTNodePtr *tree, char *key, functionPtrData function_data)
{
    if((*tree) == NULL)
    {
        *tree = (GlobalBSTNodePtr) malloc(sizeof(struct GlobalBSTNode));

        if(*tree != NULL)
        {
            strcpy((*tree)->key, key);
            (*tree)->content = function_data;
            (*tree)->left = NULL;
            (*tree)->right = NULL;
        }

        return;
    }

    if(strcmp(key, (*tree)->key) < 0)
    {
        global_bst_insert(&(*tree)->left, key, function_data);
    }
    else
    {
        if(strcmp((*tree)->key, key) < 0)
        {
            global_bst_insert(&(*tree)->right, key, function_data);
        }
        else
        {
            (*tree)->content = function_data;
        }
    }
}

void global_bst_dispose(GlobalBSTNodePtr *tree)
{
    if(*tree != NULL)
    {
        global_bst_dispose(&(*tree)->left);
        global_bst_dispose(&(*tree)->right);
        free(*tree);
    }
    (*tree) = NULL;
}
//
//
//void BST_DLL_Init(BSTDLList *list)
//{
//    list->first = NULL;
//    list->last = NULL;
//    list->active = NULL;
//}
//
//void BST_DLL_Next(BSTDLList *list)
//{
//    if (list->active != NULL) {
//        list->active = list->active->;
//    }
//}
//
//void BST_DLL_Dispose(BSTDLList *list)
//{
//
//}
//
//void BST_DLL_InsertLast(BSTDLList *list, LocalBSTNodePtr nodePtr)
//{
//
//}
//
//void BST_DLL_DeleteLast(BSTDLList *list)
//{
//
//}