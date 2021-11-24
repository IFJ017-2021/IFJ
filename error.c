/**
 *  @file error.c
 *
 *  @brief Implementation of printing error codes
 *
 *  @author <xhajek51> Vojtěch Hájek
 */
#include "scanner.h"
#include "error.h"
#include "str.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void err_call(int error, token_ptr *list) {
    switch (error) {
        case ERR_LEX:
            fprintf(stderr, "ERR 1 - Lexical error\n");
            break;
        case ERR_SYNTAX:
            fprintf(stderr, "ERR 2 - Syntax error");
            break;
        case ERR_SMNTIC_UNDEF:
            fprintf(stderr, "ERR 3 - Semantic error (indefinite function or variable..)");
            break;
        case ERR_SMNTIC_TYPE:
            fprintf(stderr, "ERR 4 - Semantic error (type incompatibility)");
            break;
        case ERR_SMNTIC_PARAMS:
            fprintf(stderr, "ERR 5 - Semantic error (wrong number / type of parameters or return values when calling a function or returning from a function");
            break;
        case ERR_SMNTIC_EXPR:
            fprintf(stderr, "ERR 6 - Semantic error (type incompatibility in arithmetic, string or relational expressions)");
            break;
        case ERR_SMNTIC_OTHER:
            fprintf(stderr, "ERR 7 - Semantic error (other)");
            break;
        case ERR_RUN_NILL:
            fprintf(stderr, "ERR 8 - Runtime error when working with unexpected nil value");
            break;
        case ERR_RUN_ZERODIV :
            fprintf(stderr, "ERR 9 - Runtime error of integer division by zero constant ")
            break;
        case ERR_INTERNAL:
            fprintf(stderr, "ERR 99 - Internal compiler error");
            break;
    }
    exit(error);
}

