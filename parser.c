/**
 *  @file parser.h
 *
 *  @brief Header file for parser.c (Syntax analyzer).
 *
 *  @author <xpoliv06> Tomáš Polívka
 *  @author <xhajek51> Vojtěch Hájek
 */

#include "parser.h"
#include "error.h"
#include "str.h"
#include "scanner.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define GET_TOKEN()              \
    DLL_Next(&token_list);                          \
    DLL_GetValue(&token_list, &token); \
    while(token->type == T_EOL)                       \
    {                                                 \
      DLL_Next(&token_list);                          \
      DLL_GetValue(&token_list, &token);              \
    }

#define CHECK_TYPE(_type)                           \
	if (token->type != (_type)) err_call(ERR_SYNTAX)

#define IS_TYPE_VALUE(_token)										\
	if((_token->type != T_DOUBLE)					\
	&& (_token->type != T_INT)						\
	&& (_token->type != T_STRING)							\
	&& (_token->type != T_ID)) err_call(ERR_SYNTAX)

#define  IS_EXPRESSION(token) \
    if((token->type == T_ID)          \
    && (token->next->type != T_ASSIGN)  \
    && (token->next->type != T_EQL)     \
    && (token->next->type != T_GT)      \
    && (token->next->type != T_GTE)     \
    && (token->next->type != T_LT)      \
    && (token->next->type != T_LTE)     \
    && (token->next->type != T_NEQL)    \
    && (token->next->type != T_MUL)     \
    && (token->next->type != T_SUB)     \
    && (token->next->type != T_ADD)     \
    && (token->next->type != T_DIV)     \
    && (token->next->type != T_IDIV)    \
    && (token->next->type != T_STRLEN)  \
    ){}else {/*expression(); */}

DLList token_list;
token_ptr token;

void start(DLList *testlist)
{
    token_list = *testlist;
    DLL_First(&token_list);
    DLL_GetFirst(&token_list, &token);
//    if(get_token_list(&token_list) == ERR_LEX)
//    {
//        err_call(ERR_LEX);
//    }

    CHECK_TYPE(T_K_REQUIRE);
    GET_TOKEN();
    if(token->type != T_STRING){
        err_call(ERR_SYNTAX);
    }
    GET_TOKEN()
    main_list();

    GET_TOKEN()
    CHECK_TYPE(T_EOF);
    return;
}
void main_list(){
    switch (token->type) {
        case T_K_FUNCTION:
            GET_TOKEN()
            CHECK_TYPE(T_ID);

            GET_TOKEN()
            CHECK_TYPE(T_LEFT_PAR);

            GET_TOKEN()
            list_of_params();

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);

            GET_TOKEN()
            return_list_of_types();

            GET_TOKEN()
            statement();

            GET_TOKEN()
            CHECK_TYPE(T_K_END);

            GET_TOKEN()
            main_next();
            break;
        case T_ID:
            GET_TOKEN()
            CHECK_TYPE(T_LEFT_PAR);

            GET_TOKEN()
            entry_list_params();

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);

            GET_TOKEN()
            main_next();
            break;
        case T_K_GLOBAL:

            GET_TOKEN()
            CHECK_TYPE(T_ID);

            GET_TOKEN()
            CHECK_TYPE(T_DOUBLE_DOT);

            GET_TOKEN()
            CHECK_TYPE(T_K_FUNCTION);

            GET_TOKEN()
            CHECK_TYPE(T_LEFT_PAR);

            GET_TOKEN()
            list_of_types();

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);

            DLL_Next(&token_list);
            DLL_GetValue(&token_list, &token);
            while(token->type == T_EOL)
            {
              DLL_Next(&token_list);
              DLL_GetValue(&token_list, &token);
            }

            return_list_of_types();

            GET_TOKEN()
            main_next();
            break;
        default:
            err_call(ERR_SYNTAX);
            break;
    }
}
void main_next(){
    if(token->type == T_ID || token->type == T_K_FUNCTION || token->type == T_K_GLOBAL){
        main_list();
    } else{
        DLL_Previous(&token_list);
    }
}

void statement(){
    switch (token->type) {
        case T_ID:
            GET_TOKEN()
            if(token->type == T_LEFT_PAR){
                entry_list_params();

                GET_TOKEN()
                CHECK_TYPE(T_RIGHT_PAR);

                GET_TOKEN()
                statement();
            } else{
                value_id_next();

                GET_TOKEN()
                CHECK_TYPE(T_ASSIGN);

                GET_TOKEN()
                init_value();

                GET_TOKEN()
                init_value_next();

                GET_TOKEN()
                statement();
            }
            break;
        case T_K_LOCAL:
            GET_TOKEN()
            CHECK_TYPE(T_ID);

            GET_TOKEN()
            CHECK_TYPE(T_DOUBLE_DOT);

            GET_TOKEN()
            type_value();

            GET_TOKEN()
            init_value();

            GET_TOKEN()
            statement();
            break;
        case T_K_IF:
            GET_TOKEN()
            // expression();

            GET_TOKEN()
            CHECK_TYPE(T_K_THEN);

            GET_TOKEN()
            statement();

            GET_TOKEN();
            state_else();

            GET_TOKEN()
            CHECK_TYPE(T_K_END);

            GET_TOKEN()
            statement();
            break;
        case T_K_RETURN:
            GET_TOKEN()
            return_list();

            GET_TOKEN()
            statement();
            break;
        case T_K_WHILE:
            GET_TOKEN()
            // expression();

            GET_TOKEN()
            CHECK_TYPE(T_K_DO);

            GET_TOKEN()
            statement();

            GET_TOKEN()
            CHECK_TYPE(T_K_END);

            GET_TOKEN()
            statement();
            break;
        default:
            DLL_Previous(&token_list);
            break;
    }
}
void list_of_params(){
    if(token->type == T_ID){
        GET_TOKEN()
        CHECK_TYPE(T_DOUBLE_DOT);

        GET_TOKEN()
        type_value();

        GET_TOKEN()
        param_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void param_next(){
    if(token->type == T_COMMA){
        GET_TOKEN()
        CHECK_TYPE(T_ID);

        GET_TOKEN()
        CHECK_TYPE(T_DOUBLE_DOT);

        GET_TOKEN()
        type_value();

        GET_TOKEN()
        param_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void entry_list_params(){
    if(/*expression*/ (token->type == T_ID)
    || (token->type == T_INT)
    || (token->type == T_DOUBLE)
    || (token->type == T_STRING)){
        entry_param();

        GET_TOKEN()
        entry_param_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void entry_param(){
    IS_EXPRESSION(token)
}
void entry_param_next(){
    if(token->type == T_COMMA){
        GET_TOKEN()
        entry_param();

        GET_TOKEN()
        entry_param_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void list_of_types(){
    if((token->type == T_K_INTEGER)
	|| (token->type == T_K_STRING)
	|| (token->type == T_K_NUMBER)
	|| (token->type == T_K_NIL)){
        type_value();

        GET_TOKEN()
        type_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void return_list_of_types(){
    if(token->type == T_DOUBLE_DOT){
        GET_TOKEN()
        type_value();

        GET_TOKEN()
        type_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void type_next(){
    if(token->type == T_COMMA){
        GET_TOKEN()
        type_value();

        GET_TOKEN()
        type_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void type_value(){
    IS_TYPE_VALUE(token);
}
void return_list(){
    if(token->type == T_ID){
        GET_TOKEN()
        if(token->type == T_LEFT_PAR){
            GET_TOKEN()
            entry_list_params();

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);
        } else{
            return_value_next();
        }
    } else{
        DLL_Previous(&token_list);
    }
}
void return_value_next(){
    if(token->type == T_COMMA){
        GET_TOKEN()
        CHECK_TYPE(T_ID);

        GET_TOKEN()
        return_value_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void state_else(){
    if(token->type == T_K_ELSE){
        GET_TOKEN()
        statement();
    } else{
        DLL_Previous(&token_list);
    }
}
void init_value(){
    if(/*expression*/(token->type == T_ID)
    || (token->type == T_INT)
    || (token->type == T_DOUBLE)
    || (token->type == T_STRING)) {
        GET_TOKEN()
        CHECK_TYPE(T_LEFT_PAR);

        GET_TOKEN()
        entry_list_params();

        GET_TOKEN()
        CHECK_TYPE(T_RIGHT_PAR);
    } else{
        DLL_Previous(&token_list);
    }
}
void init_value_next(){
    if(token->type == T_COMMA){
        GET_TOKEN()
        init_value();

        GET_TOKEN()
        init_value_next();
    } else{
        DLL_Previous(&token_list);
    }
}
void init_local_value(){
    if(token->type == T_ASSIGN){
        GET_TOKEN()
        init_value();
    } else {
        DLL_Previous(&token_list);
    }
}
void value_id_next(){
    if(token->type == T_COMMA){
        GET_TOKEN()
        CHECK_TYPE(T_ID);

        GET_TOKEN()
        value_id_next();
    } else{
        DLL_Previous(&token_list);
    }
}