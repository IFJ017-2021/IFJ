/**
 *  Project: Implementation of imperative language compiler IFJ21
 *  @file prec_table.c
 *
 *  @brief Implementation of precedence table
 *
 *  @author <xvasin11> Ladislav Vasina
 */

#include "prec_table.h"
#include "error.h"
#include "parser.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 17
#define EX_TABLE 50

prec_table_actions precedence_table[TABLE_SIZE][TABLE_SIZE] = {
    //      |+ |-| *| /|//| <| >|<=|>=|~=|==| (| )| i| #|..| $|
    /* + */ {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
    /* - */ {R, R, S, S, S, R, R, R, R, R, R, S, R, S, S, R, R},
    /* * */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* / */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* //*/ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* < */ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* > */ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* <=*/ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* >=*/ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* ~=*/ {S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
    /* ==*/ {S, S, S, S, S, S, S, S, S, R, R, S, R, S, S, S, R},
    /* ( */ {S, S, S, S, S, S, S, S, S, S, S, S, E, S, S, S, X},
    /* ) */ {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
    /* i */ {R, R, R, R, R, R, R, R, R, R, R, X, R, X, X, R, R},
    /* # */ {R, R, R, R, R, R, R, R, R, R, R, S, R, S, S, R, R},
    /* ..*/ {S, S, S, S, S, R, R, R, R, R, R, S, R, S, S, S, R},
    /* $ */ {S, S, S, S, S, S, S, S, S, S, S, S, X, S, S, S, X},
};

token_ptr prec_token;
DLList prec_token_list;
Stack_Bst stack_bst_tree_exp;

int number_in_table(token_ptr token_table, bool a) {
  if (a == 0) {
    if (token_table->type == T_LT || token_table->type == T_GT ||
        token_table->type == T_LTE || token_table->type == T_GTE ||
        token_table->type == T_EQL || token_table->type == T_NEQL) {
      err_call(ERR_SYNTAX, token_table);
    }
  }
  switch (token_table->type) {
  case T_ADD:
    return 0;
  case T_SUB:
    return 1;
  case T_MUL:
    return 2;
  case T_DIV:
    return 3;
  case T_IDIV:
    return 4;
  case T_LT:
    return 5;
  case T_GT:
    return 6;
  case T_LTE:
    return 7;
  case T_GTE:
    return 8;
  case T_NEQL:
    return 9;
  case T_EQL:
    return 10;
  case T_LEFT_PAR:
    return 11;
  case T_RIGHT_PAR:
    return 12;
  case T_INT:
  case T_ID:
  case T_STRING:
  case T_DOUBLE:
    return 13;
  case T_K_NIL:
    if ((token_table->prev != NULL && token_table->prev->type != T_P_DOLLAR &&
        token_table->prev->type != T_EQL && token_table->prev->type != T_NEQL) ||
        (token_table->next != NULL && token_table->next->type != T_P_DOLLAR &&
        token_table->next->type != T_EQL && token_table->next->type != T_NEQL)) {
      err_call(ERR_RUN_NILL, token_table);
    }
    return 13;
  case T_STRLEN:
    return 14;
  case T_CONCAT:
    return 15;
  case T_P_DOLLAR:
    return 16;
  default:
    err_call(ERR_SYNTAX, token_table);
    return -1;
  }
}
/*
    1. E -> E + E
    2. E -> E - E
    3. E -> E * E
    4. E -> E / E
    5. E -> E // E
    6. E -> E < E
    7. E -> E > E
    8. E -> E <= E
    9. E -> E >= E
    10. E -> E ~= E
    11. E -> E == E
    12. E -> (E)
    13. E -> i
    14. E -> #E
    15. E -> E .. E
*/
prec_parsing_rules check_rule(int symbCount, token_ptr *tokens) {
  token_ptr t1 = tokens[0];
  token_ptr t2 = tokens[1];
  token_ptr t3 = tokens[2];
  switch (symbCount) {
  case 1:
    // E -> i
    if (t1->type == T_ID || t1->type == T_INT || t1->type == T_DOUBLE ||
        t1->type == T_STRING || t1->type == T_K_NIL) {
      return R_ID;
    } else {
      return R_NOTDEFINED;
    }
  case 2:
    // E -> #E
    if (t2->type == T_STRLEN && t1->type == T_P_E) {
      return R_STRLEN;
    } else {
      return R_NOTDEFINED;
    }
  case 3:
    // E -> E + E
    if (t1->type == T_P_E && t2->type == T_ADD && t3->type == T_P_E) {
      return R_ADD;
    }
    // E -> E - E
    if (t1->type == T_P_E && t2->type == T_SUB && t3->type == T_P_E) {
      return R_SUB;
    }
    // E -> E * E
    if (t1->type == T_P_E && t2->type == T_MUL && t3->type == T_P_E) {
      return R_MUL;
    }
    // E -> E / E
    if (t1->type == T_P_E && t2->type == T_DIV && t3->type == T_P_E) {
      return R_DIV;
    }
    // E -> E // E
    if (t1->type == T_P_E && t2->type == T_IDIV && t3->type == T_P_E) {
      return R_IDIV;
    }
    // E -> E == E
    if (t1->type == T_P_E && t2->type == T_EQL && t3->type == T_P_E) {
      return R_EQL;
    }
    // E -> E ~= E
    if (t1->type == T_P_E && t2->type == T_NEQL && t3->type == T_P_E) {
      return R_NEQL;
    }
    // E -> E >= E
    if (t1->type == T_P_E && t2->type == T_GTE && t3->type == T_P_E) {
      return R_GTE;
    }
    // E -> E <= E
    if (t1->type == T_P_E && t2->type == T_LTE && t3->type == T_P_E) {
      return R_LTE;
    }
    // E -> E < E
    if (t1->type == T_P_E && t2->type == T_LT && t3->type == T_P_E) {
      return R_LT;
    }
    // E -> E > E
    if (t1->type == T_P_E && t2->type == T_GT && t3->type == T_P_E) {
      return R_GT;
    }
    // E -> (E)
    if (t1->type == T_RIGHT_PAR && t2->type == T_P_E &&
        t3->type == T_LEFT_PAR) {
      return R_PARS;
    }
    // E -> E .. E
    if (t1->type == T_P_E && t2->type == T_CONCAT && t3->type == T_P_E) {
      return R_CONCAT;
    }
    return R_NOTDEFINED;
  default:
    return R_NOTDEFINED;
  }
}

char *string_postfix(token_ptr string_token){
    char *a;
    int count;
    switch (string_token->type) {
        case T_ID:
            return string_token->data->string->data;
            break;
        case T_K_INTEGER:;
            count = snprintf(NULL, 0, "%i", string_token->data->integer);
            a = malloc(sizeof(char) * (count + 1));
            sprintf(a, "%d", string_token->data->integer);
            return a;
        case T_K_STRING:;
            string *b = malloc(sizeof(string));
            strInit(b);
            strAppendStr(b, "'");
            strAppendStr(b, string_token->data->string->data);
            strAppendStr(b, "'");
            return b->data;
        case T_K_NUMBER:;
            count = snprintf(NULL, 0, "%i", string_token->data->integer);
            a = malloc(sizeof(char) * (count + 1));
            sprintf(a, "%d", string_token->data->integer);
            return a;
        case T_K_NIL:
            return "nil";
        case T_ADD:
            return "+";
        case T_SUB:
            return "-";
        case T_MUL:
            return "*";
        case T_DIV:
            return "/";
        case T_IDIV:
            return "//";
        case T_LT:
            return "<";
        case T_GT:
            return ">";
        case T_LTE:
            return "<=";
        case T_GTE:
            return ">=";
        case T_NEQL:
            return "~=";
        case T_EQL:
            return "==";
        case T_LEFT_PAR:
            return "(";
        case T_RIGHT_PAR:
            return ")";
        case T_STRLEN:
            return "#";
        case T_CONCAT:
            return "..";
        default:
            err_call(ERR_INTERNAL, string_token);
            return a;
    }
}

int operation(token_ptr operation){
    switch (operation->type) {
        case T_ADD:
        case T_SUB:
        case T_MUL:
            return 0;
        case T_DIV:
            return 1;
        case T_IDIV:
            return 2;
        case T_LT:
        case T_GT:
        case T_LTE:
        case T_GTE:
        case T_NEQL:
        case T_EQL:
            return 3;
        case T_STRLEN:
            return 4;
        case T_CONCAT:
            return 5;
        default:
            return -1;
    }
}


token_ptr expression(DLList *list, bool where_expression, Stack_Bst *stackBst, token_type exp_type) {
  if (list == NULL) {
    err_call(ERR_SYNTAX, NULL);
    }
  if (list->first->next == NULL) {
      DLL_First(list);
      DLL_GetFirst(list, &prec_token);
      switch(prec_token->type) {
          case T_INT:
              prec_token->type = T_K_INTEGER;
              break;
          case T_DOUBLE:
              prec_token->type = T_K_NUMBER;
              break;
          case T_STRING:
              prec_token->type = T_K_STRING;
              break;
          default:
              break;
      }
      if (exp_type != T_OTHER){
          if(prec_token->type != exp_type && (prec_token->type != T_K_INTEGER || exp_type != T_K_NUMBER) &&
             (prec_token->type != T_K_NUMBER || exp_type == T_K_NUMBER)){
              err_call(ERR_SMNTIC_EXPR, prec_token);
          }
      }
      char *tmp = string_postfix(prec_token);
      prec_token->type = T_P_EXPRESSION;
      prec_token->data->string->data = tmp;
      return prec_token;
  }
    stack_bst_tree_exp = *stackBst;

    prec_token_list = *list;
    DLL_First(&prec_token_list);
    DLL_GetFirst(&prec_token_list, &prec_token);
    token_ptr temp = (token_ptr)malloc(sizeof(struct token));
    temp->type = T_P_DOLLAR;

    DLL_InsertLast(&prec_token_list, temp);

    Stack_Token *stack = (Stack_Token * )malloc(sizeof (Stack_Token));
    Stack_Token_Init(stack);
    Stack_Token_Push(stack, temp);

    Stack_Token *stack_sym = (Stack_Token* )malloc(sizeof (Stack_Token));
    Stack_Token_Init(stack_sym);
    Stack_Token_Push(stack_sym, temp);
    int count;
    count = 0;
    token_ptr expression_table[EX_TABLE];


    while (prec_token->type != T_P_DOLLAR || stack->array[stack->topIndex]->type != T_P_DOLLAR ){
        token_ptr tmp;
        Stack_Token_Top(stack, &tmp);
        int row = number_in_table(tmp, where_expression);
        int col = number_in_table(prec_token, where_expression);
        prec_table_actions action = precedence_table[row][col];
        switch (action) {
            case R:;
                int num_symbols = 0;
                token_ptr symbols[3];
                while (stack_sym->array[stack_sym->topIndex]->type != T_OTHER ){
                    Stack_Token_Top(stack_sym, &tmp);
                    Stack_Token_Pop(stack_sym);
                    symbols[num_symbols] = tmp;
                    num_symbols++;

                }
                while (stack->array[stack->topIndex]->type != T_OTHER ){
                    Stack_Token_Top(stack, &tmp);
                    Stack_Token_Pop(stack);
                    if(tmp->type != T_LEFT_PAR && tmp->type != T_RIGHT_PAR && tmp->type != T_OTHER){
                        expression_table[count] = tmp;
                        count++;
                    }
                }
                if( check_rule(num_symbols, symbols) == R_NOTDEFINED){
                    err_call(ERR_SYNTAX, prec_token);
                }
                Stack_Token_Pop(stack);
                Stack_Token_Pop(stack_sym);

                token_ptr tmp = (token_ptr) malloc(sizeof (struct token));
                tmp->type = T_P_E;
                Stack_Token_Push(stack_sym, tmp);
                break;
            case S:;
                token_ptr temp = (token_ptr) malloc(sizeof (struct token));
                if(stack_sym->array[stack_sym->topIndex]->type == T_P_E){
                    Stack_Token_Pop(stack_sym);
                    temp->type = T_OTHER;
                    Stack_Token_Push(stack, temp);
                    Stack_Token_Push(stack_sym, temp);
                    token_ptr temp2 = (token_ptr) malloc(sizeof (struct token));
                    temp2->type = T_P_E;
                    Stack_Token_Push(stack_sym, temp2);
                } else{
                    temp->type = T_OTHER;
                    Stack_Token_Push(stack, temp);
                    Stack_Token_Push(stack_sym, temp);
                }
                Stack_Token_Push(stack, prec_token);
                Stack_Token_Push(stack_sym, prec_token);
                DLL_Next(&prec_token_list);
                DLL_GetValue(&prec_token_list, &prec_token);
                break;
            case E:
                Stack_Token_Push(stack, prec_token);
                Stack_Token_Push(stack_sym, prec_token);
                DLL_Next(&prec_token_list);
                DLL_GetValue(&prec_token_list, &prec_token);
                break;
            case X:
                err_call(ERR_SYNTAX, prec_token);
                break;
        }

    }
    Stack_Token_Pop(stack_sym);

    Stack_Token *sem_stack = (Stack_Token * )malloc(sizeof (Stack_Token));
    Stack_Token_Init(sem_stack);
    int state = 0;
    for (int j = 0; j < count;) {
        switch (state) {
            case 0:
                if(expression_table[j]->type != T_INT && expression_table[j]->type != T_DOUBLE &&
                   expression_table[j]->type != T_STRING && expression_table[j]->type != T_K_NIL &&
                   expression_table[j]->type != T_ID){
                    state = 1;break;
                }
                if(expression_table[j]->type == T_ID){
                    LocalBSTNodePtr tmpValue;
                    bool isFound = false;
                    token_ptr tmp= (token_ptr) malloc(sizeof (struct token));
                    for (int i = stack_bst_tree_exp.topIndex; i >= 0; i--) {
                        if (local_bst_search(stack_bst_tree_exp.array[i], expression_table[j]->data->string->data, &tmpValue) == true) {
                            tmp->type = tmpValue->type;
                            isFound = true;
                            break;
                        }
                    }
                    if (isFound == false) {
                        err_call(ERR_SMNTIC_UNDEFINED_V, expression_table[j]);
                    }
                    Stack_Token_Push(sem_stack, tmp);
                }
                switch (expression_table[j]->type) {
                    case T_INT:
                        expression_table[j]->type = T_K_INTEGER;
                        Stack_Token_Push(sem_stack, expression_table[j]);
                        break;
                    case T_DOUBLE:
                        expression_table[j]->type = T_K_NUMBER;
                        Stack_Token_Push(sem_stack, expression_table[j]);
                        break;
                    case T_STRING:
                        expression_table[j]->type = T_K_STRING;
                        Stack_Token_Push(sem_stack, expression_table[j]);
                        break;
                    case T_K_NIL:
                        Stack_Token_Push(sem_stack, expression_table[j]);
                        break;
                    default:
                        break;
                }
                j++;
                break;
            case 1:;
                int op = operation(expression_table[j]);
                if(op != 4){
                    token_ptr tmp1;
                    Stack_Token_Top(sem_stack, &tmp1);
                    Stack_Token_Pop(sem_stack);

                    token_ptr tmp2;
                    Stack_Token_Top(sem_stack, &tmp2);
                    Stack_Token_Pop(sem_stack);

                    token_ptr tmp = (token_ptr) malloc(sizeof (struct token));
                    if(op == 0){
                        if((tmp1->type != T_K_INTEGER && tmp1->type != T_K_NUMBER) ||
                           (tmp2->type != T_K_INTEGER && tmp2->type != T_K_NUMBER)){
                            err_call(ERR_SMNTIC_EXPR, expression_table[j]);
                        }
                        if(tmp1->type != tmp2->type){
                            tmp->type = T_K_NUMBER;
                            Stack_Token_Push(sem_stack, tmp);
                        }
                        if(tmp1->type == T_K_INTEGER && tmp2->type == T_K_INTEGER){
                            tmp->type = T_K_INTEGER;
                            Stack_Token_Push(sem_stack, tmp);
                        }
                        if(tmp1->type == T_K_NUMBER && tmp2->type == T_K_NUMBER){
                            tmp->type = T_K_NUMBER;
                            Stack_Token_Push(sem_stack, tmp);
                        }
                        j++;
                        state = 0;
                        break;
                    }
                    if(op == 1){
                        if((tmp1->type != T_K_INTEGER && tmp1->type != T_K_NUMBER) ||
                           (tmp2->type != T_K_INTEGER && tmp2->type != T_K_NUMBER)){
                            err_call(ERR_SMNTIC_EXPR, expression_table[j]);
                        }
                        tmp->type = T_K_NUMBER;
                        Stack_Token_Push(sem_stack, tmp);
                        j++;
                        state = 0;
                        break;
                    }
                    if(op == 2){
                        if(tmp1->type != T_K_INTEGER || tmp2->type != T_K_INTEGER){
                            err_call(ERR_SMNTIC_EXPR, expression_table[j]);
                        }
                        tmp->type = T_K_INTEGER;
                        Stack_Token_Push(sem_stack, tmp);
                        j++;
                        state = 0;
                        break;
                    }
                    if(op == 3) {
                        tmp->type = T_K_INTEGER;
                        Stack_Token_Push(sem_stack, tmp);
                        j++;
                        state = 0;
                        break;
                    }
                    if(op == 5){
                        if(tmp1->type != T_K_STRING || tmp2->type != T_K_STRING){
                            err_call(ERR_SMNTIC_EXPR, expression_table[j]);
                        }
                        tmp->type = T_K_STRING;
                        Stack_Token_Push(sem_stack, tmp);
                        j++;
                        state = 0;
                        break;
                    }
                }
                if(op == 4){
                    token_ptr tmp;
                    Stack_Token_Top(sem_stack, &tmp);
                    Stack_Token_Pop(sem_stack);

                    if(tmp->type != T_K_STRING){
                        err_call(ERR_SMNTIC_EXPR, expression_table[j]);
                    }
                    token_ptr tmp1 = (token_ptr) malloc(sizeof (struct token));
                    tmp1->type = T_K_INTEGER;
                    Stack_Token_Push(sem_stack, tmp1);
                    j++;
                    state = 0;
                    break;
                }
                break;
        }
    }
    token_ptr res;
    Stack_Token_Top(sem_stack, &res);
    Stack_Token_Pop(sem_stack);

    if(exp_type != T_OTHER){
        if(res->type != exp_type && (res->type != T_K_INTEGER || exp_type != T_K_NUMBER) &&
           (res->type != T_K_NUMBER || exp_type == T_K_NUMBER))
        {
            err_call(ERR_SMNTIC_EXPR, res);
        }
    }

    token_ptr result = (token_ptr) malloc(sizeof (struct token));
    result->data =  malloc(sizeof(struct token_data));
    result->data->integer = 0;
    result->data->number = 0.0;
    result->type = T_P_EXPRESSION;
    string *s = malloc(sizeof(string));
    strInit(s);
    int i=0;
    while (i < count){
        char *tmp = string_postfix(expression_table[i]);
        if(i != 0){
            strAppendStr(s, " ");
        }
        strAppendStr(s, tmp);
        expression_table[i] = NULL;
        i++;
    }
    DLL_Dispose(&prec_token_list);
    result->data->string = s;
    return result;
}