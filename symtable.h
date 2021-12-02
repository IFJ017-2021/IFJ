/**
 *  @file symtable.h
 *
 *  @brief Header file for symtable.c
 *
 *  @author <xpoliv06> Tomáš Polívka
 */

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>
#include "tokenList.h"
#define MAXPARAMS 256

/**
 * @brief Doubly linked list of BST
 */
typedef struct {
    struct LocalBSTNode *first;
    struct LocalBSTNode *active;
    struct LocalBSTNode *last;
} BSTDLList;

/**
 * @brief Struct for node of local tree
 */
typedef struct LocalBSTNode {
    char *key;
    token_type type;            // only T_K_NUMBER, T_K_INT, T_K_STRING, T_K_NILL
    struct LocalBSTNode *left;
    struct LocalBSTNode *right;
} *LocalBSTNodePtr;

/**
 * @brief Struct for data of function
 */
typedef struct functionData{
    int numOfParams;
    int numOfReturns;
    char *params[MAXPARAMS];
    char *key;
    token_type paramsType[MAXPARAMS];
    token_type returns[MAXPARAMS];
} *functionPtrData;

/**
 * @brief Struct for node of global tree
 */
typedef struct GlobalBSTNode {
    char *key;
    struct functionData *content;
    struct GlobalBSTNode *left;
    struct GlobalBSTNode *right;
} *GlobalBSTNodePtr;

// Function for LOCAL_BST

///**
// * @brief
// *
// * @param tree
// */
//void local_bst_init(LocalBSTNodePtr *tree);
//
///**
// * @brief
// *
// * @param tree
// * @param key
// * @param type
// */
//void local_bst_insert(LocalBSTNodePtr *tree, char *key, ValueType type);
//
///**
// * @brief
// *
// * @param tree
// * @param key
// */
//bool local_bst_search(LocalBSTNodePtr tree, char *key);
//
///**
// * @brief
// *
// * @param tree
// * @param key
// */
//void local_bst_delete(LocalBSTNodePtr *tree, char *key);
//
///**
// * @brief
// *
// * @param tree
// */
//void local_bst_dispose(LocalBSTNodePtr *tree);

// Function for GLOBAL_BST

/**
 * @brief
 *
 * @param tree
 */
void global_bst_init(GlobalBSTNodePtr *tree);

/**
 * @brief
 *
 * @param tree
 * @param key
 */
bool global_bst_search(GlobalBSTNodePtr tree, char *key, functionPtrData *data);

/**
 * @brief
 *
 * @param tree
 * @param key
 * @param fuction_data
 */
void global_bst_insert(GlobalBSTNodePtr *tree, char *key, functionPtrData function_data);

/**
 * @brief
 *
 * @param tree
 */
void global_bst_dispose(GlobalBSTNodePtr *tree);

// Function for BST_DLL

///**
// * @brief List initialization
// * @param list Pointer to the structure of a doubly linked list
// */
//void BST_DLL_Init(BSTDLList *list);
//
///**
// * @brief Moves the activity to the next list element.
// * @param list Pointer to the structure of a doubly linked list
// */
//void BST_DLL_Next(BSTDLList *list);
//
///**
// * @brief List dispose
// * @param list Pointer to the structure of a doubly linked list
// */
//void BST_DLL_Dispose(BSTDLList *list);
//
///**
// * @brief Inserts new element at the end of the list
// * @param list Pointer to the structure of a doubly linked list
// * @param token Value to insert at the end of the list
// */
//void BST_DLL_InsertLast(BSTDLList *list, LocalBSTNodePtr nodePtr);
//
///**
// * @brief Deletes last element in the list
// * @param list Pointer to the structure of a doubly linked list
// */
//void BST_DLL_DeleteLast(BSTDLList *list);

#endif
