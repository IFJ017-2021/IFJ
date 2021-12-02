/**
 *  @file prec_table.c
 *
 *  @brief Implementation of precedence table
 *
 *  @author <xvasin11> Ladislav Vasina
 */

#include "prec_table.h"
#include "stack.h"
#include "error.h"
#include "parser.h"
#include "tokenList.h"

#define TABLE_SIZE 17

prec_table_actions precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    //         |+ |-| *| /|//| <| >|<=|>=|~=|==| (| )| i| #|..| $|
    /* + */ {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
    /* - */ {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
    /* * */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* / */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* //*/ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* < */ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* > */ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* <=*/{S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* >=*/{S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* ~=*/{S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
    /* ==*/{S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
    /* ( */ {S, S, S, S, S, S, S, S, S, S, S, S, E, S, S, S, X},
    /* ) */ {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
    /* i */ {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
    /* # */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* ..*/ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* $ */ {S, S, S, S, S, S, S, S, S, S, S, S, X, S, S, S, X},
};

token_ptr prec_token;
DLList prec_token_list;
Stack stack;

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
        default:
            err_call(ERR_SYNTAX, prec_token);
    }
}

void expression(DLList *list){
    if(list == NULL){
        err_call(ERR_SYNTAX, NULL);
    }
    prec_token_list = *list;
    DLL_GetFirst(&prec_token_list, prec_token);
    while (prec_token != NULL){
        int row = number_in_table(prec_token);
        int col = number_in_table(prec_token->next);
        prec_table_actions action = precedence_table[row][col];
        switch (action) {
            case R:

                break;
            case S:

                break;
            case E:

                break;
            case X:
                err_call(ERR_SYNTAX, prec_token);
                break;
        }
    }
}