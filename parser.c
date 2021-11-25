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
#include "tokenList.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

DLList token_list;
token_ptr token;

void start()
{
    DLL_Init(&token_list)
    if(get_token_list(&token) == ERR_LEX)
    {
        err_call(ERR_LEX);
    }
    DLL_Next(token_list);
    DLL_GetValue(token_list, &token);
    if(token->type != T_K_REQUIRE){
        err_call(ERR_SYNTAX);
    }
    token = token->next;
    DLL_InsertLast(&token_list, token);
    if(token->type != T_STRING || strcmp(token->data->string, "ifj21") != 0){
        err_call(ERR_SYNTAX);
    }
    main_list();

    token = token->next;
    DLL_InsertLast(&token_list, token);
    if(token->type != T_EOF){
        err_call(ERR_SYNTAX);
    }
    return;
}
void main_list(){
    switch (&token->type) {
        case T_K_FUNCTION:
            token = token->next;
            DLL_InsertLast(&token_list, token);
            if(token->type != T_ID){
                err_call(ERR_SYNTAX);
            }
            token = token->next;
            DLL_InsertLast(&token_list, token);
            if(token->type != T_LEFT_PAR){
                err_call(ERR_SYNTAX);
            }
            token = token->next;
            DLL_InsertLast(&token_list, token);

            list_of_params();

            token = token->next;
            DLL_InsertLast(&token_list, token);
            if(token->type != T_RIGHT_PAR){
                err_call(ERR_SYNTAX);
            }
            token = token->next;
            DLL_InsertLast(&token_list, token);
            return_list_of_types();

            token = token->next;
            DLL_InsertLast(&token_list, token);
            statement();

            token = token->next;
            DLL_InsertLast(&token_list, token);
            if(token->type != T_K_END){
                err_call(ERR_SYNTAX);
            }
            token = token->next;
            DLL_InsertLast(&token_list, token);
            main_list();break;
        case T_ID: ;break;
        case T_K_GLOBAL:;break;
    }
}
void statement(){

}
void list_of_params(){

}
void param_next(){

}
void entry_list_params(){

}
void entry_param(){

}
void entry_param_next(){

}
void list_of_types(){

}
void return_list_of_types(){

}
void type_next(){

}
void type_value(){

}
void return_list(){

}
void return_value_next(){

}
void state_else(){

}
void init_value(){

}
void init_value_next(){

}
void init_local_value(){

}
void value_id_next(){

}