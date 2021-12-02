/**
 *  @file prec_table.h
 *
 *  @brief Header file for prec_table.c (File that implements precedence table)
 *
 *  @author <xvasin11> Ladislav Vasina
 */
#ifndef IFJ_PREC_TABLE_H
#define IFJ_PREC_TABLE_H

#include "tokenList.h"

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
    GEQ,            // >=
    NEQ,            // ~=
    EQ,             // ==
    LEFT_BRACKET,   // (
    RIGHT_BRACKET,  // )
    ID,             // i
    LENGTH,         // #
    CONCAT,         // ..
    END_DOLLAR      // $
} prec_table_symbols;

/**
 * @enum Parsing rules
 */
typedef enum{
    R_PLUS,         // E -> E + E 0
    R_MINUS,        // E-> E - E 1
    R_MUL,          // E -> E * E 2
    R_DIV,          // E -> E / E 3
    R_EQUAL,        // E -> E = E 5
    R_NEQUAL,       // E -> E != E 6
    R_MOE,          // E -> E >= E 7
    R_LOE,          // E -> E <= E 8
    R_LESS,         // E -> E < E 9
    R_MORE,         // E -> E > E 10
    R_EBRACES,      // E -> (E) 11
    R_OP,           // E -> I 12
    R_NOTDEFINED    // NONDEFINED RULE 13
} prec_parsing_rules;

#endif