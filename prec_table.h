/**
 *  Project: Implementation of imperative language compiler IFJ21
 *  @file prec_table.h
 *
 *  @brief Header file for prec_table.c (File that implements precedence table)
 *
 *  @author <xvasin11> Ladislav Vasina
 *  @author <xhajek51> Vojtěch Hájek
 */
#ifndef IFJ_PREC_TABLE_H
#define IFJ_PREC_TABLE_H

#include "tokenList.h"
#include "stack.h"
#include "stdbool.h"

/**
 * @enum Precedence table symbols
 * 
 */
typedef enum{
    S,  // < shift
    E,  // = equal
    R,  // > reduce
    X   // X error
} prec_table_actions;

/**
 * @enum Precedence table symbols
 * 
 */
typedef enum{
    PLUS,           // +
    MINUS,          // -
    MUL,            // *
    DIV,            // /
    IDIV,           // //
    LT,             // <
    GT,             // >
    LEQ,            // <=
    MEQ,            // >=
    NEQ,            // ~=
    EQ,             // ==
    LEFT_BRACKET,   // (
    RIGHT_BRACKET,  // )
    ID,             // i
    LENGHT,         // #
    CONCAT,         // ..
    END_DOLLAR      // $
} prec_table_symbols;

typedef enum{
    R_ADD,         // E -> E + E
    R_SUB,        // E-> E - E
    R_MUL,          // E -> E * E
    R_DIV,          // E -> E / E
    R_IDIV,         // E -> E // E
    R_LT,         // E -> E < E
    R_GT,         // E -> E > E
    R_LTE,          // E -> E <= E
    R_GTE,          // E -> E >= E
    R_NEQL,       // E -> E ~= E
    R_EQL,          // E -> E == E
    R_PARS,      // E -> (E)
    R_ID,           // E -> i
    R_STRLEN,       // E -> #E
    R_CONCAT,        // E -> E .. E
    R_NOTDEFINED    // NONDEFINED RULE
} prec_parsing_rules;

token_ptr expression(DLList *list, bool where_expression, Stack_Bst stackBst);
int number_in_table(token_ptr token_table, bool a);
#endif