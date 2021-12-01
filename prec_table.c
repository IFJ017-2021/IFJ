/**
 *  @file prec_table.c
 *
 *  @brief Implementation of precedence table
 *
 *  @author <xvasin11> Ladislav Vasina
 */

#include "prec_table.h"
#include "error.h"
#include "parser.h"

#define TABLE_SIZE 17

int precedence_table[TABLE_SIZE][TABLE_SIZE] =
{
//         |+ |-| *| /|//| <| >|<=|>=|~=|==| (| )| i| #|..| $|
/* + */    {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
/* - */    {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
/* * */    {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
/* / */    {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
/* //*/    {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
/* < */    {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
/* > */    {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
/* <=*/    {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
/* >=*/    {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
/* ~=*/    {S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
/* ==*/    {S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
/* ( */    {S, S, S, S, S, S, S, S, S, S, S, S, E, S, S, S, X},
/* ) */    {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
/* i */    {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
/* # */    {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
/* ..*/    {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
/* $ */    {S, S, S, S, S, S, S, S, S, S, S, S, X, S, S, S, X},
};



/**
* @brief Function converts token type to symbol
* @param token pointer
* 
*/

static prec_table_symbols get_symbol_from_token(token_ptr t){

    switch (t->type)
    {
    case T_ADD:
        return PLUS;
    case T_SUB:
        return MINUS;
    case T_MUL:
        return MUL;
    case T_DIV:
        return DIV;
    case T_IDIV:
        return IDIV;
    case T_LT:
        return LT;
    case T_GT:
        return T_GT;
    case T_LTE:
        return LEQ;
    case T_GTE:
        return GEQ;
    case T_NEQL:
        return NEQ;
    case T_EQL:
        return EQ;
    case T_LEFT_PAR:
        return LEFT_BRACKET;
    case T_RIGHT_PAR:
        return RIGHT_BRACKET;
    case T_ID:
    case T_INT:
    case T_DOUBLE:
        return ID;
    case T_STRLEN:
        return LENGHT;
    case T_DOUBLE_DOT:
        return CONCAT;
    default:
        return END_DOLLAR;
    }
}
