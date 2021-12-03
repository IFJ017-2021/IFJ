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
#define EX_TABLE 50

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
        case T_K_NIL:
            if((token_table->prev != NULL && token_table->prev->type != T_EQL && token_table->prev->type != T_NEQL)
            && (token_table->next != NULL && token_table->next->type != T_EQL && token_table->next->type != T_NEQL )){
                err_call(ERR_RUN_NILL, token_table);
            }
            return 13;
        case T_STRLEN:
            return 14;
        case T_CONCAT:
            return 15;
        case T_P_DOLLAR:
            return 16;
        default:
            err_call(ERR_SYNTAX, token_table);
            return -1;
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
prec_parsing_rules check_rule(int symbCount, token_ptr *tokens) {
    token_ptr t1 = tokens[0]; token_ptr t2 = tokens[1]; token_ptr t3 = tokens[2];
    switch (symbCount) {
        case 1:
            // E -> i
            if (t1->type == T_ID || t1->type == T_INT || t1->type == T_DOUBLE || t1->type == T_STRING || t1->type == T_K_NIL) {
                return R_ID;
            } else {
                return R_NOTDEFINED;
            }
        case 2:
            // E -> #E
            if(t2->type == T_STRLEN && t1->type == T_P_E){
                return R_STRLEN;
            } else{
                return R_NOTDEFINED;
            }
        case 3:
            // E -> E + E
            if (t1->type == T_P_E && t2->type == T_ADD && t3->type == T_P_E) {
                return R_ADD;
            }
            // E -> E - E
            if (t1->type == T_P_E && t2->type == T_SUB && t3->type == T_P_E) {
                return R_SUB;
            }
            // E -> E * E
            if (t1->type == T_P_E && t2->type == T_MUL && t3->type == T_P_E) {
                return R_MUL;
            }
            // E -> E / E
            if (t1->type == T_P_E && t2->type == T_DIV && t3->type == T_P_E) {
                return R_DIV;
            }
            // E -> E // E
            if (t1->type == T_P_E && t2->type == T_IDIV && t3->type == T_P_E) {
                return R_IDIV;
            }
            // E -> E == E
            if (t1->type == T_P_E && t2->type == T_EQL && t3->type == T_P_E) {
                return R_EQL;
            }
            // E -> E ~= E
            if (t1->type == T_P_E && t2->type == T_NEQL && t3->type == T_P_E) {
                return R_NEQL;
            }
            // E -> E >= E
            if (t1->type == T_P_E && t2->type == T_GTE && t3->type == T_P_E) {
                return R_GTE;
            }
            // E -> E <= E
            if (t1->type == T_P_E && t2->type == T_LTE && t3->type == T_P_E) {
                return R_LTE;
            }
            // E -> E < E
            if (t1->type == T_P_E && t2->type == T_LT && t3->type == T_P_E) {
                return R_LT;
            }
            // E -> E > E
            if (t1->type == T_P_E && t2->type == T_GT && t3->type == T_P_E) {
                return R_GT;
            }
            // E -> (E)
            if (t1->type == T_RIGHT_PAR && t2->type == T_P_E && t3->type == T_LEFT_PAR) {
                return R_PARS;
            }
            // E -> E .. E
            if(t1->type == T_P_E && t2->type == T_CONCAT && t3->type == T_P_E){
                return  R_CONCAT;
            }
            return R_NOTDEFINED;
        default:
            return R_NOTDEFINED;
    }
}

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
    temp->type = T_P_DOLLAR;

    DLL_InsertLast(&prec_token_list, temp);

    Stack *stack = (Stack* )malloc(sizeof (Stack));
    Stack_Init(stack);
    Stack_Push(stack, temp);

    Stack *stack_sym = (Stack* )malloc(sizeof (Stack));
    Stack_Init(stack_sym);
    Stack_Push(stack_sym, temp);
    int count;
    count = 0;
    token_ptr expression_table[EX_TABLE];


    while (prec_token->type != T_P_DOLLAR || stack->array[stack->topIndex]->type != T_P_DOLLAR ){
        token_ptr tmp;
        Stack_Top(stack, &tmp);
        int row = number_in_table(tmp);
        int col = number_in_table(prec_token);
        prec_table_actions action = precedence_table[row][col];
        switch (action) {
            case R:;
                int num_symbols = 0;
                token_ptr symbols[3];
                while (stack_sym->array[stack_sym->topIndex]->type != T_OTHER ){
                    Stack_Top(stack_sym, &tmp);
                    Stack_Pop(stack_sym);
                    symbols[num_symbols] = tmp;
                    num_symbols++;
                }
                while (stack->array[stack->topIndex]->type != T_OTHER ){
                    Stack_Top(stack, &tmp);
                    Stack_Pop(stack);
                    if(tmp->type != T_LEFT_PAR && tmp->type != T_RIGHT_PAR && tmp->type != T_OTHER){
                        expression_table[count] = tmp;
                        count++;
                    }
                }
                if( check_rule(num_symbols, symbols) == R_NOTDEFINED){
                    err_call(ERR_SYNTAX, prec_token);
                }
                Stack_Pop(stack);
                Stack_Pop(stack_sym);

                token_ptr tmp = (token_ptr) malloc(sizeof (struct token));
                tmp->type = T_P_E;
                Stack_Push(stack_sym, tmp);
                break;
            case S:;
                token_ptr temp = (token_ptr) malloc(sizeof (struct token));
                if(stack_sym->array[stack_sym->topIndex]->type == T_P_E){
                    Stack_Pop(stack_sym);
                    temp->type = T_OTHER;
                    Stack_Push(stack, temp);
                    Stack_Push(stack_sym, temp);
                    token_ptr temp2 = (token_ptr) malloc(sizeof (struct token));
                    temp2->type = T_P_E;
                    Stack_Push(stack_sym, temp2);
                } else{
                    temp->type = T_OTHER;
                    Stack_Push(stack, temp);
                    Stack_Push(stack_sym, temp);
                }
                Stack_Push(stack, prec_token);
                Stack_Push(stack_sym, prec_token);
                DLL_Next(&prec_token_list);
                DLL_GetValue(&prec_token_list, &prec_token);
                break;
            case E:
                Stack_Push(stack, prec_token);
                Stack_Push(stack_sym, prec_token);
                DLL_Next(&prec_token_list);
                DLL_GetValue(&prec_token_list, &prec_token);
                break;
            case X:
                err_call(ERR_SYNTAX, prec_token);
                break;
        }
    }
    Stack_Pop(stack_sym);
}