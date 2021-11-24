/**
 *  @file parser.h
 *
 *  @brief Header file for parser.c (Syntax analyzer).
 *
 *  @author <xpoliv06> Tomáš Polívka
 *  @author <xhajek51> Vojtěch Hájek
 */

#include "parser.h"
#include "scanner.h"
#include "error.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>

token_ptr token;

void start()
{
    if(get_token_list(&token) == ERR_LEX)
    {
        err_call(ERR_LEX);
    }
    token = token->next;

}
