/**
 *  @file scanner.h
 *
 *  @brief Header file for scanner.c (Lexical analyzer).
 *
 *  @author <xvagne10> Dominik Vágner
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define str(x) #x
#define xstr(x) str(x)

/**
 * @brief Enum of all IFJ21 keywords.
 */
typedef enum {
    T_OTHER,
    T_EOL,
    T_EOF,
	T_SUB,
	T_ADD,
	T_MULL,
	T_DIV,
    T_IDIV,
	T_STRLEN,
	T_LEFT_PAR,
    T_RIGHT_PAR,
	T_DOUBLE_DOT,
    T_ASSIGN,       // =
    T_EQL,          // ==
	T_GT,           // >
	T_GTE,          // >=
	T_LT,           // <
	T_LTE,          // <=
    T_NEQL,         // ~=
    T_COMMA,
	T_ID,
	T_INT,
	T_DOUBLE,
	T_STRING,

	T_K_DO,
	T_K_ELSE,
	T_K_END,
    T_K_FUNCTION,
    T_K_GLOBAL,
	T_K_IF,
	T_K_INTEGER,
	T_K_LOCAL,
	T_K_NIL,
	T_K_NUMBER,
	T_K_REQUIRE,
	T_K_RETURN,
	T_K_STRING,
	T_K_THEN,
	T_K_WHILE,
} token_type;

/**
 * @brief Enum of all IFJ21 keywords.
 */
typedef enum {
    DO,
    ELSE,
    END,
    FUNCTION,
    GLOBAL,
    IF,
    INTEGER,
    LOCAL,
    NIL,
    NUMBER,
    REQUIRE,
    RETURN,
    STRING,
    THEN,
    WHILE
} keywords;

/**
 * @brief Enum of all states for lexical analysis FSM. 
 */
typedef enum {
	S_START,
    S_SPACE,
    S_SUB,
    S_COMMENT0,
    S_COMMENT1,
    S_COMMENT2,
    S_COMMENT3,
    S_ADD,
    S_MUL,
    S_DIV,
	S_IDIV,
	S_STRLEN,
    S_LEFT_PAR,
	S_RIGHT_PAR,
	S_DOUBLE_DOT,
    S_ASSIGN,
    S_EQL,          // ==
	S_GT,           // >
	S_GTE,          // >=
	S_LT,           // <
	S_LTE,          // <=
    S_COMMA,
	S_ID,
    S_INT,
	S_INT0,
    S_HEX0,
	S_HEX1,
	S_FP_DOT,
    S_NUMBER,
    S_EXP0,
	S_EXP1,
    S_EXP2,
	S_EOL,
	S_EOF,
	S_DOT0,
	S_DOT1,
    S_TILDA,
	S_NEQL,
	S_STR_START,    // "
    S_STR_T1,       // \?                T8
    S_STR_T2,       // \0                T9
    S_STR_T3,       // \00               T10
    S_STR_T4,       // \0(1-9)  \1(0-9)  T11
    S_STR_T5,       // \1                T12
    S_STR_T6,       // \2                T13
    S_STR_T7,       // \2(0-4)           T14
    S_STR_T8,       // \25               T15
    S_STR_FIN       // "
} lexfsm_state;

/**
 * @brief Union of different possible term values.  
 */
typedef union {
    char *string; // change to our type string as in showcase
    int integer;
    double number;
} token_data; 

/**
 * @brief Token pointer struct. 
 */
typedef struct token {
    token_type type;
    token_data data;
    unsigned int line_num;
    unsigned int col_num;
    struct token *prev;
    struct token *next;
} *token_ptr;

/**
 * @brief Function to read whole input and append all tokens to a token list. 
 * 
 * @param first Pointer to first token in list. Insert new tokens after.
 * @return int State code, If there aren't any errors ? 0 : 2 
 */
int get_token_list(token_ptr *first);

/**
 * @brief Read single token from stdin and insert into passed token pointer.
 * 
 * @param insert_into Token pointer into which are data supposed to be insterted. 
 * @return int State code, If there aren't any errors ? 0 : 2
 */
int get_single_token(token_ptr *insert_into);

/**
 * @brief Determine if token is keyword from IFJ21
 * 
 * @param token Token with ID type who can be a token.
 * @return true If token is a keyword.
 * @return false If Token isn't a keyword.
 */
bool is_token_keyword(token_ptr *token);

/**
 * @brief Print all tokens from token list to stdout. 
 * 
 * @param first 
 */
void print_token_list(token_ptr first);

/**
 * @brief Print a single token to stdout. 
 * 
 * @param token Token to be printed.
 */
void print_single_token(token_ptr token);

/**
 * @brief Finiste state machine which decides the type of token.
 * 
 * @param token Token pointer whomst type we need to determine.
 * @return int State code, If there aren't any errors ? 0 : 2 
 */
int lex_fsm(token_ptr *token);