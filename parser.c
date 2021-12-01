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
#include "symtable.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_TOKEN()                                                            \
  DLL_Next(&token_list);                                                       \
  DLL_GetValue(&token_list, &token);

#define CHECK_TYPE(_type)                                                      \
  if (token->type != (_type))                                                  \
  err_call(ERR_SYNTAX, token)

#define IS_TYPE_VALUE()                                                         \
  if ((token->type != T_K_NUMBER) && (token->type != T_K_INTEGER) &&            \
      (token->type != T_K_STRING) && (token->type != T_K_NIL))                  \
  err_call(ERR_SYNTAX, token)

#define  IS_EXPRESSION()                                                        \
    if((token->next->type != T_ASSIGN)                                          \
    && (token->next->type != T_EQL)                                             \
    && (token->next->type != T_GT)                                              \
    && (token->next->type != T_GTE)                                             \
    && (token->next->type != T_LT)                                              \
    && (token->next->type != T_LTE)                                             \
    && (token->next->type != T_NEQL)                                            \
    && (token->next->type != T_MUL)                                             \
    && (token->next->type != T_SUB)                                             \
    && (token->next->type != T_ADD)                                             \
    && (token->next->type != T_DIV)                                             \
    && (token->next->type != T_IDIV)                                            \
    && (token->next->type != T_STRLEN)                                          \
    && (token->next->type != T_CONCAT)                                          \
    && (token->next->type != T_RIGHT_PAR)                                       \
    ){break;}                                                                   \
    else{                                                                       \
        GET_TOKEN()                                                             \
    }
#define INIT_TOKEN_POINTER()                                                    \
        token_ptr tmp = (token_ptr) malloc(sizeof (struct token));              \
        tmp->type = token->type;                                                \
        tmp->data = token->data;
DLList token_list;
token_ptr token;

void start(DLList *testlist) {
  token_list = *testlist;
  DLL_First(&token_list);
  DLL_GetFirst(&token_list, &token);
  if(token->type == T_EOL || token->type == T_OTHER){
      GET_TOKEN();
  }

  CHECK_TYPE(T_K_REQUIRE);
  GET_TOKEN()

  if (token->type != T_STRING || strcmp(token->data->string->data, "ifj21") != 0) {
    err_call(ERR_SYNTAX, token);
  }
  GET_TOKEN()
  main_list();

  GET_TOKEN()
  CHECK_TYPE(T_EOF);
}
void main_list() {
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
    while (token->type == T_EOL) {
      DLL_Next(&token_list);
      DLL_GetValue(&token_list, &token);
    }

    return_list_of_types();

    GET_TOKEN()
    main_next();
    break;
  default:
    err_call(ERR_SYNTAX, token);
    break;
  }
}
void main_next() {
  if (token->type == T_ID || token->type == T_K_FUNCTION ||
      token->type == T_K_GLOBAL) {
    main_list();
  } else {
    DLL_Previous(&token_list);
  }
}

void statement() {
  switch (token->type) {
  case T_ID:
    GET_TOKEN()
    if (token->type == T_LEFT_PAR) {
      GET_TOKEN()
      entry_list_params();

      GET_TOKEN()
      CHECK_TYPE(T_RIGHT_PAR);

      GET_TOKEN()
      statement();
    } else {
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
    init_local_value();

    GET_TOKEN()
    statement();
    break;
  case T_K_IF:
    GET_TOKEN()
    DLList expression_list_if;
    DLL_Init(&expression_list_if);
    while (token->type != T_K_THEN && token->type != T_EOF){
        INIT_TOKEN_POINTER()
        DLL_InsertLast(&expression_list_if, tmp);
        if(token->type == T_LEFT_PAR){
            GET_TOKEN()
            expression_par_tmp(&expression_list_if);
        }
        GET_TOKEN()
    }
    //expression(&expression_list_if);


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
    DLList expression_list_while;
    DLL_Init(&expression_list_while);
    while (token->type != T_K_DO && token->type != T_EOF){
        INIT_TOKEN_POINTER()
        DLL_InsertLast(&expression_list_if, tmp);
        if(token->type == T_LEFT_PAR){
            GET_TOKEN()
            expression_par_tmp(&expression_list_if);
        }
        GET_TOKEN()
    }
    //expression(&expression_list_while);
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
void list_of_params() {
  if (token->type == T_ID) {
    GET_TOKEN()
    CHECK_TYPE(T_DOUBLE_DOT);

    GET_TOKEN()
    type_value();

    GET_TOKEN()
    param_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void param_next() {
  if (token->type == T_COMMA) {
    GET_TOKEN()
    CHECK_TYPE(T_ID);

    GET_TOKEN()
    CHECK_TYPE(T_DOUBLE_DOT);

    GET_TOKEN()
    type_value();

    GET_TOKEN()
    param_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void entry_list_params(){
    if((token->type == T_ID) || (token->type == T_INT)
    || (token->type == T_DOUBLE) || (token->type == T_STRING)
    || (token->type == T_STRLEN) || (token->type == T_LEFT_PAR)){
        entry_param();

        GET_TOKEN()
        entry_param_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void entry_param() {
    if((token->type == T_ID)
       && (token->next->type != T_ASSIGN)
       && (token->next->type != T_EQL)
       && (token->next->type != T_GT)
       && (token->next->type != T_GTE)
       && (token->next->type != T_LT)
       && (token->next->type != T_LTE)
       && (token->next->type != T_NEQL)
       && (token->next->type != T_MUL)
       && (token->next->type != T_SUB)
       && (token->next->type != T_ADD)
       && (token->next->type != T_DIV)
       && (token->next->type != T_IDIV)
       && (token->next->type != T_STRLEN)
       && (token->next->type != T_CONCAT)){
        return;
    } else{
        DLList expression_list;
        DLL_Init(&expression_list);
        while (token->type != T_EOF){
            INIT_TOKEN_POINTER()
            DLL_InsertLast(&expression_list, tmp);
            if(token->type == T_LEFT_PAR){
                expression_par_tmp(&expression_list);
            }
            if((token->type == T_ID) || (token->type == T_INT)
            || (token->type == T_DOUBLE) || (token->type == T_STRING)){
                IS_EXPRESSION()
            } else{
                GET_TOKEN()
            }
        }
        //expression(&expression_list);
    }
}

void entry_param_next() {
  if (token->type == T_COMMA) {
    GET_TOKEN()
    entry_param();

    GET_TOKEN()
    entry_param_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void list_of_types() {
  if ((token->type == T_K_INTEGER) || (token->type == T_K_STRING) ||
      (token->type == T_K_NUMBER) || (token->type == T_K_NIL)) {
    type_value();

    GET_TOKEN()
    type_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void return_list_of_types() {
  if (token->type == T_DOUBLE_DOT) {
    GET_TOKEN()
    type_value();

    GET_TOKEN()
    type_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void type_next() {
  if (token->type == T_COMMA) {
    GET_TOKEN()
    type_value();

    GET_TOKEN()
    type_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void type_value() { IS_TYPE_VALUE(); }
void return_list() {
  if (token->type == T_ID || token->type == T_STRING
  || token->type == T_INT || token->type == T_DOUBLE
  || token->type == T_STRLEN || token->type == T_LEFT_PAR) {
      if((token->type == T_ID)
         && (token->next->type != T_ASSIGN)
         && (token->next->type != T_EQL)
         && (token->next->type != T_GT)
         && (token->next->type != T_GTE)
         && (token->next->type != T_LT)
         && (token->next->type != T_LTE)
         && (token->next->type != T_NEQL)
         && (token->next->type != T_MUL)
         && (token->next->type != T_SUB)
         && (token->next->type != T_ADD)
         && (token->next->type != T_DIV)
         && (token->next->type != T_IDIV)
         && (token->next->type != T_STRLEN)
         && (token->next->type != T_CONCAT)){
        GET_TOKEN()
        if (token->type == T_LEFT_PAR) {
          GET_TOKEN()
          entry_list_params();

          GET_TOKEN()
          CHECK_TYPE(T_RIGHT_PAR);

          GET_TOKEN()
          return_value_next();
        } else {
          return_value_next();
        }
      } else{
          DLList expression_list;
          DLL_Init(&expression_list);
          while (token->type != T_EOF){
              INIT_TOKEN_POINTER()
              DLL_InsertLast(&expression_list, tmp);
              if(token->type == T_LEFT_PAR){
                  expression_par_tmp(&expression_list);
              }
              if((token->type == T_ID) || (token->type == T_INT)
                 || (token->type == T_DOUBLE) || (token->type == T_STRING)){
                  IS_EXPRESSION()
              } else{
                  GET_TOKEN()
              }
          }
          //expression(&expression_list);
          GET_TOKEN()
          return_value_next();
      }
  } else {
    DLL_Previous(&token_list);
  }
}
void return_value_next() {
  if (token->type == T_COMMA) {
      if((token->type == T_ID)
         && (token->next->type != T_ASSIGN)
         && (token->next->type != T_EQL)
         && (token->next->type != T_GT)
         && (token->next->type != T_GTE)
         && (token->next->type != T_LT)
         && (token->next->type != T_LTE)
         && (token->next->type != T_NEQL)
         && (token->next->type != T_MUL)
         && (token->next->type != T_SUB)
         && (token->next->type != T_ADD)
         && (token->next->type != T_DIV)
         && (token->next->type != T_IDIV)
         && (token->next->type != T_STRLEN)
         && (token->next->type != T_CONCAT)){
          GET_TOKEN()
          if (token->type == T_LEFT_PAR) {
              GET_TOKEN()
              entry_list_params();

              GET_TOKEN()
              CHECK_TYPE(T_RIGHT_PAR);

              GET_TOKEN()
              return_value_next();
          } else {
              GET_TOKEN()
              return_value_next();
          }
      } else{
          DLList expression_list;
          DLL_Init(&expression_list);
          while (token->type != T_EOF){
              INIT_TOKEN_POINTER()
              DLL_InsertLast(&expression_list, tmp);
              if(token->type == T_LEFT_PAR){
                  expression_par_tmp(&expression_list);
              }
              if((token->type == T_ID) || (token->type == T_INT)
              || (token->type == T_DOUBLE) || (token->type == T_STRING)){
                  IS_EXPRESSION()
              } else{
                  GET_TOKEN()
              }
          }
          //expression(&expression_list);
          GET_TOKEN()
          return_value_next();
      }
  } else {
    DLL_Previous(&token_list);
  }
}
void state_else() {
  if (token->type == T_K_ELSE) {
    GET_TOKEN()
    statement();
  } else {
    DLL_Previous(&token_list);
  }
}
void init_value() {
  if ((token->type == T_ID) || (token->type == T_INT)
  ||(token->type == T_DOUBLE) || (token->type == T_STRING)
  || (token->type == T_STRLEN)|| token->type == T_LEFT_PAR) {
    if((token->type == T_ID)
    && (token->next->type != T_ASSIGN)
    && (token->next->type != T_EQL)
    && (token->next->type != T_GT)
    && (token->next->type != T_GTE)
    && (token->next->type != T_LT)
    && (token->next->type != T_LTE)
    && (token->next->type != T_NEQL)
    && (token->next->type != T_MUL)
    && (token->next->type != T_SUB)
    && (token->next->type != T_ADD)
    && (token->next->type != T_DIV)
    && (token->next->type != T_IDIV)
    && (token->next->type != T_STRLEN)
    && (token->next->type != T_CONCAT)){
        if(token->next->type == T_LEFT_PAR) {
            GET_TOKEN()
            CHECK_TYPE(T_LEFT_PAR);

            GET_TOKEN()
            entry_list_params();

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);
        }
    } else {
        DLList expression_list;
        DLL_Init(&expression_list);
        while (token->type != T_EOF){
            INIT_TOKEN_POINTER()
            DLL_InsertLast(&expression_list, tmp);
            if(token->type == T_LEFT_PAR){
                expression_par_tmp(&expression_list);
            }
            if((token->type == T_ID) || (token->type == T_INT)
               || (token->type == T_DOUBLE) || (token->type == T_STRING)){
                IS_EXPRESSION()
            } else{
                GET_TOKEN()
            }
        }
        //expression(&expression_list);
    }
  } else {
    DLL_Previous(&token_list);
  }
}
void init_value_next() {
  if (token->type == T_COMMA) {
    GET_TOKEN()
    init_value();

    GET_TOKEN()
    init_value_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void init_local_value() {
  if (token->type == T_ASSIGN) {
    GET_TOKEN()
    init_value();
  } else {
    DLL_Previous(&token_list);
  }
}
void value_id_next() {
  if (token->type == T_COMMA) {
    GET_TOKEN()
    CHECK_TYPE(T_ID);

    GET_TOKEN()
    value_id_next();
  } else {
    DLL_Previous(&token_list);
  }
}
void expression_par_tmp(DLList *list){
    while (token->type != T_RIGHT_PAR){
        INIT_TOKEN_POINTER()
        if(token->type == T_LEFT_PAR){
            DLL_InsertLast(list, tmp);
            GET_TOKEN()
            expression_par_tmp(list);
        } else{
            DLL_InsertLast(list, tmp);
        }
        GET_TOKEN()
    }
    INIT_TOKEN_POINTER()
    DLL_InsertLast(list, tmp);
}