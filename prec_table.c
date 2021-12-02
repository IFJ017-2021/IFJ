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
        return LENGTH;
    case T_DOUBLE_DOT:
        return CONCAT;
    default:
        return END_DOLLAR;
    }
}

/**
* @brief Function converts given character to index
* @param character
*/
prec_table_symbols get_index_from_char(char character){
    
    switch (character)
    {
    case '+':
       return PLUS;
    case '-':
        return MINUS;
    case '*':
        return MUL;
    case '/':
        return DIV;
    case '//':
        return IDIV;
    case '(':
        return LEFT_BRACKET;
    case ')':
        return RIGHT_BRACKET;
    case 'i':
        return ID;
    case '#':
        return LENGTH;
    case '..':
        return CONCAT;
    default:
        return character;
    }
}

/**
* @brief Function converts given symbol(index) to character
* @param symbol
*/
char get_symbol_from_index(prec_table_symbols symbol){

    switch (symbol){
    case PLUS:
        return '+';
    case MINUS:
        return '-';
    case MUL:
        return '*';
    case DIV:
        return '/';
    case IDIV:
        return '//';
    case LEFT_BRACKET:
        return '(';
    case RIGHT_BRACKET:
        return ')';
    case ID:
        return 'i';
    case LENGTH:
        return '#';
    case CONCAT:
        return '..';
    default:
        return symbol;
    }
}


/**
 * @brief This function tests if symbol/parametr combination is valid according to rules
 * @param symbCount count of symbols used to evaluate the rule
 * @param t1 symbol 1
 * @param t2 symbol 2
 * @param t3 symbol 3
 * @return rule code
 */
static prec_parsing_rules check_rule(int symbCount, token_ptr t1, token_ptr t2, token_ptr t3){
    switch (symbCount)
    {
    case 1:
        // E -> i
        if(t1->type == T_ID || t1->type == T_INT || t1->type == T_DOUBLE || t1->type == T_STRING){
            return R_OP;
        }else{
            return R_NOTDEFINED;
        }
    case 3:
        // E -> E + E
        if(t1->type == T_OTHER && t2->type == T_ADD && t3->type == T_OTHER){
            return R_PLUS;
        }
        // E -> E - E
        if(t1->type == T_OTHER && t2->type == T_SUB && t3->type == T_OTHER){
            return R_MINUS;
        }
        // E -> E * E
        if(t1->type == T_OTHER && t2->type == T_MUL && t3->type == T_OTHER){
            return R_MUL;
        }
        // E -> E / E
        if(t1->type == T_OTHER && t2->type == T_DIV && t3->type == T_OTHER){
            return R_DIV;
        }
        // E -> E = E
        if(t1->type == T_OTHER && t2->type == T_EQL && t3->type == T_OTHER){
            return R_EQUAL;
        }
        // E -> E != E
        if(t1->type == T_OTHER && t2->type == T_NEQL && t3->type == T_OTHER){
            return R_NEQUAL;
        }
        // E -> E >= E
        if(t1->type == T_OTHER && t2->type == T_GTE && t3->type == T_OTHER){
            return R_MOE;
        }
        // E -> E <= E
        if(t1->type == T_OTHER && t2->type == T_LTE && t3->type == T_OTHER){
            return R_LOE;
        }
        // E -> E < E
        if(t1->type == T_OTHER && t2->type == T_LT && t3->type == T_OTHER){
            return R_LESS;
        }
        // E -> E > E
        if(t1->type == T_OTHER && t2->type == T_GT && t3->type == T_OTHER){
            return R_MORE;
        }
        // E -> (E)
        if(t1->type == T_LEFT_PAR && t2->type == T_OTHER && t3->type == T_RIGHT_PAR){
            return R_EBRACES;
        }
        return R_NOTDEFINED;
    default:
        return R_NOTDEFINED;
    }
}

