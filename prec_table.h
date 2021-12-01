/**
 *  @file prec_table.h
 *
 *  @brief Header file for prec_table.c (File that implements precedence table)
 *
 *  @author <xvasin11> Ladislav Vasina
 */

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
