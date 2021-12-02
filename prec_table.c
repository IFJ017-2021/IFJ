/**
 *  @file prec_table.c
 *
 *  @brief Implementation of precedence table
 *
 *  @author <xvasin11> Ladislav Vasina
 */

#include "prec_table.h"
#include "stack.c"
#include "error.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 17

prec_table_actions precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    //      |+ |-| *| /|//| <| >|<=|>=|~=|==| (| )| i| #|..| $|
    /* + */ {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
    /* - */ {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
    /* * */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* / */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* //*/ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* < */ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* > */ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* <=*/ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* >=*/ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* ~=*/ {S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
    /* ==*/ {S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
    /* ( */ {S, S, S, S, S, S, S, S, S, S, S, S, E, S, S, S, X},
    /* ) */ {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
    /* i */ {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
    /* # */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* ..*/ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* $ */ {S, S, S, S, S, S, S, S, S, S, S, S, X, S, S, S, X},
};

token_ptr prec_token;
DLList prec_token_list;



int number_in_table(token_ptr token_table){
    switch (token_table->type) {
        case T_ADD:
            return 0;
        case T_SUB:
            return 1;
        case T_MUL:
            return 2;
        case T_DIV:
            return 3;
        case T_IDIV:
            return 4;
        case T_LT:
            return 5;
        case T_GT:
            return 6;
        case T_LTE:
            return 7;
        case T_GTE:
            return 8;
        case T_NEQL:
            return 9;
        case T_EQL:
            return 10;
        case T_LEFT_PAR:
            return 11;
        case T_RIGHT_PAR:
            return 12;
        case T_INT:
        case T_ID:
        case T_STRING:
        case T_DOUBLE:
            return 13;
        case T_STRLEN:
            return 14;
        case T_CONCAT:
            return 15;
        case T_OTHER:
            return 16;
        default:
            err_call(ERR_SYNTAX, prec_token);
    }
}
/*
    1. E -> E + E
    2. E -> E - E
    3. E -> E * E
    4. E -> E / E
    5. E -> E // E
    6. E -> E < E
    7. E -> E > E
    8. E -> E <= E
    9. E -> E >= E
    10. E -> E ~= E
    11. E -> E == E
    12. E -> (E)
    13. E -> i
    14. E -> #E
    15. E -> E .. E
*/
void expression(DLList *list){
    if(list == NULL){
        err_call(ERR_SYNTAX, NULL);
    }
    if(list->first->next == NULL){
        return;
    }
    prec_token_list = *list;
    DLL_First(&prec_token_list);
    DLL_GetFirst(&prec_token_list, &prec_token);
    token_ptr temp = (token_ptr) malloc(sizeof (struct token));
    temp->type = T_OTHER;

    DLL_InsertLast(&prec_token_list, temp);

    Stack *stack = (Stack* )malloc(sizeof (Stack));
    Stack_Init(stack);
    Stack_Push(stack, temp);

    int count;

    token_ptr expression_table[EX_TABLE];

    count = 0;
    while (prec_token->type != T_OTHER || stack->array[stack->topIndex]->type != T_OTHER ){
        token_ptr tmp;
        Stack_Top(stack, &tmp);
        int row = number_in_table(tmp);
        int col = number_in_table(prec_token);
        prec_table_actions action = precedence_table[row][col];
        switch (action) {
            case R:
                while (stack->array[stack->topIndex]->type != T_OTHER ){
                    Stack_Top(stack, &tmp);
                    Stack_Pop(stack);
                    if(tmp->type != T_LEFT_PAR && tmp->type != T_RIGHT_PAR && tmp->type != T_OTHER){
                        expression_table[count] = tmp;
                        count++;
                    }
                }
                Stack_Pop(stack);
                break;
            case S:;
                token_ptr tmp = (token_ptr) malloc(sizeof (struct token));
                tmp->type = T_OTHER;
                Stack_Push(stack, tmp);
                Stack_Push(stack, prec_token);
                DLL_Next(&prec_token_list);
                DLL_GetValue(&prec_token_list, &prec_token);
                break;
            case E:
                Stack_Push(stack, prec_token);
                DLL_Next(&prec_token_list);
                DLL_GetValue(&prec_token_list, &prec_token);
                break;
            case X:
                err_call(ERR_SYNTAX, prec_token);
                break;
        }

    }
}