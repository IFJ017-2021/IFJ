/**
 *  Project: Implementation of imperative language compiler IFJ21
 *  @file parser.h
 *
 *  @brief Header file for parser.c (Syntax and semantic  analyzer).
 *
 *  @author <xpoliv06> Tomáš Polívka
 *  @author <xhajek51> Vojtěch Hájek
 */

#include "parser.h"
#include "error.h"
#include "symtable.c"
#include "tokenList.h"
#include "scanner.h"
#include "prec_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_TOKEN()                                                            \
  DLL_Next(&token_list);                                                       \
  DLL_GetValue(&token_list, &token);

#define CHECK_TYPE(_type)                                                      \
  if (token->type != (_type)){                                                 \
    err_call(ERR_SYNTAX, token);                                               \
  }                                                                            \

#define IS_TYPE_VALUE()                                                         \
  if ((token->type != T_K_NUMBER) && (token->type != T_K_INTEGER) &&            \
      (token->type != T_K_STRING) && (token->type != T_K_NIL))                  \
  err_call(ERR_SYNTAX, token)

#define IS_EXPRESSION()                                                        \
  if ((token->next->type != T_EQL) && (token->next->type != T_GT) &&           \
      (token->next->type != T_GTE) && (token->next->type != T_LT) &&           \
      (token->next->type != T_LTE) && (token->next->type != T_NEQL) &&         \
      (token->next->type != T_MUL) && (token->next->type != T_SUB) &&          \
      (token->next->type != T_ADD) && (token->next->type != T_DIV) &&          \
      (token->next->type != T_IDIV) && (token->next->type != T_STRLEN) &&      \
      (token->next->type != T_CONCAT)) {                                       \
      DLL_Next(&token_list);                                                       \
      DLL_GetValue(&token_list, &token);                                       \
      DLL_DeleteBefore(&token_list);                                           \
      DLL_Previous(&token_list);                                               \
      DLL_GetValue(&token_list, &token);  \
    break;                                                                     \
  } else {                                                                     \
    GET_TOKEN()                                                                \
  }

#define INIT_TOKEN_POINTER()                                                    \
        token_ptr tmp = (token_ptr) malloc(sizeof(struct token));               \
        tmp->type = token->type;                                                \
        tmp->data = token->data;                                                \
        tmp->line_num = token->line_num;                                        \
        tmp->col_num = token->col_num;

#define INIT_FUNCTION_DATA(_name)                                                            \
        functionPtrData _name = (functionPtrData) malloc(sizeof(struct functionData));       \
        if(_name == NULL){                                                                   \
            err_call(ERR_INTERNAL, NULL);                                                    \
        }

#define FUNCTION_IS_NOT_DEFINED()                                                            \
        if (global_bst_search(bst_tree_of_functions, token->data->string->data, &tmpData) == false){   \
            err_call(ERR_SMNTIC_UNDEFINED_F, token_ID);                                         \
        }

#define CHECK_LIST_OF_PARAMS_AND_SAVE_TO_FUNCTION_DATA()                                    \
        functionData->params[functionData->numOfParams] = token->data->string->data;        \
        GET_TOKEN()                                                                         \
        CHECK_TYPE(T_DOUBLE_DOT);                                                           \
        GET_TOKEN()                                                                         \
        type_value();                                                                       \
        functionData->paramsType[functionData->numOfParams] = token->type;                  \
        functionData->numOfParams++;                                                        \
        GET_TOKEN()                                                                         \
        param_next(functionData);

#define CHECK_RETURN_LIST_OF_TYPES_AND_SAVE_TO_FUNCTION_DATA()                              \
        GET_TOKEN()                                                                         \
        type_value();                                                                       \
        functionData->returnsType[functionData->numOfReturns] = token->type;                \
        functionData->numOfReturns++;                                                       \
        GET_TOKEN()                                                                         \
        return_type_next(functionData);

#define CHECK_LIST_OF_TYPES_AND_SAVE_TO_FUNCTION_DATA()                                     \
        GET_TOKEN()                                                                         \
        type_value();                                                                       \
        functionData->paramsType[functionData->numOfParams] = token->type;                  \
        functionData->numOfParams++;                                                        \
        GET_TOKEN()                                                                         \
        type_next(functionData);

#define CHECK_COUNT_OF_PARAMS()                                                            \
        if(functionData->numOfParams != tmpData->numOfParams){                              \
            err_call(ERR_SMNTIC_NUMBER_OF_PARAMS,token_ID);                                 \
        }

#define CHECK_COUNT_AND_TYPES_OF_PARAMS()                                                  \
        if(functionData->numOfParams != tmpData->numOfParams){                              \
            err_call(ERR_SMNTIC_NUMBER_OF_PARAMS,token_ID);                                 \
        }                                                                                   \
        for(int i = 0; i < functionData->numOfParams; i++){                                 \
            if (functionData->paramsType[i] != tmpData->paramsType[i]){                     \
                err_call(ERR_SMNTIC_PARAMS_TYPE,token_ID);                                  \
            }                                                                               \
        }

#define CHECK_COUNT_AND_TYPES_OF_RETURN_PARAMS()                                           \
        if(functionData->numOfReturns != tmpData->numOfReturns){                            \
            err_call(ERR_SMNTIC_NUMBER_OF_RETURN_PARAMS, token_ID);                         \
        }                                                                                   \
        for(int i = 0; i < functionData->numOfReturns; i++){                                \
            if (functionData->returnsType[i] != tmpData->returnsType[i]){                   \
                err_call(ERR_SMNTIC_RETURN_PARAMS_TYPE,token_ID);                           \
            }                                                                               \
        }

Stack_Bst stack_bst_tree;
GlobalBSTNodePtr bst_tree_of_functions;
DLList token_list;
token_ptr token;
char *valueId;
token_type valueType;

void start(DLList *testlist) {
    // Insert into global frame built-in functions
    global_bst_init(&bst_tree_of_functions);
    built_in_functions();

    Stack_Bst_Init(&stack_bst_tree);

    token_list = *testlist;
    DLL_First(&token_list);
    DLL_GetFirst(&token_list, &token);
    if (token->type == T_EOL || token->type == T_OTHER) {
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
    global_bst_dispose(&bst_tree_of_functions);
}

void main_list() {
    // Malloc nové struktury pro functionData
    functionPtrData functionData = (functionPtrData) malloc(sizeof(struct functionData));
    functionPtrData tmpData;
    token_ptr token_ID;
    switch (token->type) {
        case T_K_FUNCTION:
        GET_TOKEN()
            CHECK_TYPE(T_ID);

            token_ID = token;
            // Malloc a přiřazení klíče do functionData, následně kontrola zda se již název funkce nachází ve stromě
            bool isDefined = false;
            functionData->key = malloc(sizeof(char) * strlen(token->data->string->data));
            strcpy(functionData->key, token->data->string->data);
            if (global_bst_search(bst_tree_of_functions, token->data->string->data, &tmpData) == 1) {
                isDefined = true;
            }

            GET_TOKEN()
            CHECK_TYPE(T_LEFT_PAR);

            GET_TOKEN()
            list_of_params(functionData);

            // Save list of params to local frame
            LocalBSTNodePtr root;
            local_bst_init(&root);
            for (int i = 0; i < functionData->numOfParams; i++) {
                local_bst_insert(&root, functionData->params[i], functionData->paramsType[i]);
            }

            // Push tree (local frame) to stack
            Stack_Bst_Push(&stack_bst_tree, root);

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);

            GET_TOKEN()
            return_list_of_types(functionData);

            GET_TOKEN()
            statement();

            GET_TOKEN()
            CHECK_TYPE(T_K_END);

            if (isDefined) {
                CHECK_COUNT_AND_TYPES_OF_PARAMS()
                CHECK_COUNT_AND_TYPES_OF_RETURN_PARAMS()
            } else {
                global_bst_insert(&bst_tree_of_functions, functionData->key, functionData);
            }

            Stack_Bst_Pop(&stack_bst_tree);

            GET_TOKEN()
            main_next();
            break;
        case T_ID:
            token_ID = token;
            FUNCTION_IS_NOT_DEFINED()

            GET_TOKEN()
            CHECK_TYPE(T_LEFT_PAR);

            GET_TOKEN()
            entry_list_params(functionData);

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);

            // TODO zkontrolovat typy jednotlivých ID
            if (strcmp(token_ID->data->string->data, "write") != 0) {
                CHECK_COUNT_OF_PARAMS()
            }

            GET_TOKEN()
            main_next();
            break;
        case T_K_GLOBAL:
        GET_TOKEN()
            CHECK_TYPE(T_ID);

            if (global_bst_search(bst_tree_of_functions, token->data->string->data, &tmpData) == true) {
                err_call(ERR_SMNTIC_REDEFINE_F, token);
            }

            functionData->key = malloc(sizeof(char) * strlen(token->data->string->data));
            strcpy(functionData->key, token->data->string->data);

            GET_TOKEN()
            CHECK_TYPE(T_DOUBLE_DOT);

            GET_TOKEN()
            CHECK_TYPE(T_K_FUNCTION);

            GET_TOKEN()
            CHECK_TYPE(T_LEFT_PAR);

            GET_TOKEN()
            list_of_types(functionData);

            GET_TOKEN()
            CHECK_TYPE(T_RIGHT_PAR);

            GET_TOKEN()
            return_list_of_types(functionData);

            // Vlozeni nove funkce do globalniho vyhledavaciho stromu
            global_bst_insert(&bst_tree_of_functions, functionData->key, functionData);

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
    functionPtrData functionData = (functionPtrData) malloc(sizeof(struct functionData));
    functionPtrData tmpData;
    token_ptr token_ID;
    switch (token->type) {
        case T_ID:
            if (token->next->type == T_LEFT_PAR) {
                token_ID = token;
                if (global_bst_search(bst_tree_of_functions, token->data->string->data, &tmpData) == 0) {
                    err_call(ERR_SMNTIC_UNDEFINED_F, token);
                }

                GET_TOKEN()
                GET_TOKEN()
                entry_list_params(functionData);

                GET_TOKEN()
                CHECK_TYPE(T_RIGHT_PAR);

                if (strcmp(token_ID->data->string->data, "write") != 0) {
                    CHECK_COUNT_OF_PARAMS()
                }

                GET_TOKEN()
                statement();
            } else {
                GET_TOKEN();
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
            valueId = malloc(sizeof(char) * strlen(token->data->string->data));
            strcpy(valueId, token->data->string->data);
            // Search in local frame to redeclare
            if (local_bst_search(stack_bst_tree.array[stack_bst_tree.topIndex], token->data->string->data, NULL) ==
                true) {
                err_call(ERR_SMNTIC_REDEFINE_V, token);
            }

            GET_TOKEN()
            CHECK_TYPE(T_DOUBLE_DOT);

            GET_TOKEN()
            type_value();
            valueType = token->type;

            // Insert new declared value to BST
            local_bst_insert(&stack_bst_tree.array[stack_bst_tree.topIndex], valueId, token->type);

            GET_TOKEN()
            init_local_value();

            GET_TOKEN()
            statement();
            break;
        case T_K_IF:
            GET_TOKEN()
            DLList expression_list_if;
            DLL_Init(&expression_list_if);
            while (token->type != T_K_THEN && token->type != T_EOF) {
                INIT_TOKEN_POINTER()
                DLL_InsertLast(&expression_list_if, tmp);
                if (token->type == T_LEFT_PAR) {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                    expression_par_tmp(&expression_list_if);
                }
                GET_TOKEN()
                DLL_DeleteBefore(&token_list);
            }
            token_ptr prec_token = expression(&expression_list_if, 1, &stack_bst_tree);
            DLL_InsertBefore(&token_list, prec_token);

            CHECK_TYPE(T_K_THEN);

            // Save list of params to local frame
            LocalBSTNodePtr localFrame;
            local_bst_init(&localFrame);
            // Push tree (local frame) to stack
            Stack_Bst_Push(&stack_bst_tree, localFrame);

            GET_TOKEN()
            statement();

            GET_TOKEN();
            state_else();

            GET_TOKEN()
            CHECK_TYPE(T_K_END);

            Stack_Bst_Pop(&stack_bst_tree);

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
            while (token->type != T_K_DO && token->type != T_EOF) {
                INIT_TOKEN_POINTER()
                DLL_InsertLast(&expression_list_while, tmp);
                if (token->type == T_LEFT_PAR) {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                    expression_par_tmp(&expression_list_while);
                }
                GET_TOKEN()
                DLL_DeleteBefore(&token_list);
            }
            token_ptr prec_token1 = expression(&expression_list_while, 1, &stack_bst_tree);
            DLL_InsertBefore(&token_list, prec_token1);

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

void list_of_params(functionPtrData functionData) {
    if (token->type == T_ID) {
        CHECK_LIST_OF_PARAMS_AND_SAVE_TO_FUNCTION_DATA()
    } else {
        DLL_Previous(&token_list);
    }
}

void param_next(functionPtrData functionData) {
    if (token->type == T_COMMA) {
        GET_TOKEN()
        CHECK_TYPE(T_ID);

        CHECK_LIST_OF_PARAMS_AND_SAVE_TO_FUNCTION_DATA()
    } else {
        DLL_Previous(&token_list);
    }
}

void entry_list_params(functionPtrData functionData) {
    if ((token->type == T_ID) || (token->type == T_INT) ||
        (token->type == T_DOUBLE) || (token->type == T_STRING) ||
        (token->type == T_STRLEN) || (token->type == T_LEFT_PAR) ||
        token->type == T_K_NIL) {
        entry_param(functionData);
        GET_TOKEN()
        entry_param_next(functionData);
    } else {
        DLL_Previous(&token_list);
    }
}

void entry_param(functionPtrData functionData) {
    if ((token->type == T_ID) && (token->next->type != T_EQL) &&
        (token->next->type != T_GT) && (token->next->type != T_GTE) &&
        (token->next->type != T_LT) && (token->next->type != T_LTE) &&
        (token->next->type != T_NEQL) && (token->next->type != T_MUL) &&
        (token->next->type != T_SUB) && (token->next->type != T_ADD) &&
        (token->next->type != T_DIV) && (token->next->type != T_IDIV) &&
        (token->next->type != T_STRLEN) && (token->next->type != T_CONCAT)) {

    } else {
        DLList expression_list;
        DLL_Init(&expression_list);
        while (token->type != T_EOF) {
            INIT_TOKEN_POINTER()
            DLL_InsertLast(&expression_list, tmp);
            if (token->type == T_LEFT_PAR) {
                GET_TOKEN()
                DLL_DeleteBefore(&token_list);
                expression_par_tmp(&expression_list);
            }
            if ((token->type == T_ID) || (token->type == T_INT) ||
                (token->type == T_DOUBLE) || (token->type == T_STRING) ||
                (token->type == T_RIGHT_PAR) || (token->type == T_K_NIL)) {
                IS_EXPRESSION()
                DLL_DeleteBefore(&token_list);
            } else {
                GET_TOKEN()
                DLL_DeleteBefore(&token_list);
            }
        }
        token_ptr prec_token = expression(&expression_list, 0, &stack_bst_tree);
        DLL_InsertAfter(&token_list, prec_token);
        GET_TOKEN()
    }
    functionData->paramsType[functionData->numOfParams] = token->type;
    functionData->params[functionData->numOfParams] = token->data->string->data;
    functionData->numOfParams++;
}

void entry_param_next(functionPtrData functionData) {
    if (token->type == T_COMMA) {
        GET_TOKEN()
        entry_param(functionData);

        GET_TOKEN()
        entry_param_next(functionData);
    } else {
        DLL_Previous(&token_list);
    }
}

void list_of_types(functionPtrData functionData) {
    if ((token->type == T_K_INTEGER) || (token->type == T_K_STRING) ||
        (token->type == T_K_NUMBER) || (token->type == T_K_NIL)) {
        type_value();
        functionData->paramsType[functionData->numOfParams] = token->type;
        functionData->numOfParams++;
        GET_TOKEN()
        type_next(functionData);
    } else {
        DLL_Previous(&token_list);
    }
}

void type_next(functionPtrData functionData) {
    if (token->type == T_COMMA) {
        CHECK_LIST_OF_TYPES_AND_SAVE_TO_FUNCTION_DATA()
    } else {
        DLL_Previous(&token_list);
    }
}

void return_list_of_types(functionPtrData functionData) {
    if (token->type == T_DOUBLE_DOT) {
        CHECK_RETURN_LIST_OF_TYPES_AND_SAVE_TO_FUNCTION_DATA()
    } else {
        DLL_Previous(&token_list);
    }
}

void return_type_next(functionPtrData functionData) {
    if (token->type == T_COMMA) {
        CHECK_RETURN_LIST_OF_TYPES_AND_SAVE_TO_FUNCTION_DATA()
    } else {
        DLL_Previous(&token_list);
    }
}

void type_value() { IS_TYPE_VALUE(); }

void return_list(functionPtrData functionData) {
    if (token->type == T_ID || token->type == T_STRING || token->type == T_INT ||
        token->type == T_DOUBLE || token->type == T_STRLEN ||
        token->type == T_LEFT_PAR || token->type == T_K_NIL) {
        if ((token->type == T_ID) && (token->next->type != T_EQL) &&
            (token->next->type != T_GT) && (token->next->type != T_GTE) &&
            (token->next->type != T_LT) && (token->next->type != T_LTE) &&
            (token->next->type != T_NEQL) && (token->next->type != T_MUL) &&
            (token->next->type != T_SUB) && (token->next->type != T_ADD) &&
            (token->next->type != T_DIV) && (token->next->type != T_IDIV) &&
            (token->next->type != T_STRLEN) && (token->next->type != T_CONCAT)) {
            GET_TOKEN()
            if (token->type == T_LEFT_PAR) {
                GET_TOKEN()
                entry_list_params(functionData);

                GET_TOKEN()
                CHECK_TYPE(T_RIGHT_PAR);

                GET_TOKEN()
                return_value_next();
            } else {
                return_value_next();
            }
        } else {
            DLList expression_list;
            DLL_Init(&expression_list);
            while (token->type != T_EOF) {
                INIT_TOKEN_POINTER()
                DLL_InsertLast(&expression_list, tmp);
                if (token->type == T_LEFT_PAR) {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                    expression_par_tmp(&expression_list);
                }
                if ((token->type == T_ID) || (token->type == T_INT) ||
                    (token->type == T_DOUBLE) || (token->type == T_STRING) ||
                    (token->type == T_RIGHT_PAR) || (token->type == T_K_NIL)) {
                    IS_EXPRESSION()
                    DLL_DeleteBefore(&token_list);
                } else {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                }
            }
            GET_TOKEN()
            token_ptr prec_token = expression(&expression_list, 0, &stack_bst_tree);
            DLL_InsertBefore(&token_list, prec_token);

            return_value_next();
        }
    } else {
        DLL_Previous(&token_list);
    }
}

void return_value_next(functionPtrData functionData) {
    if (token->type == T_COMMA) {
        GET_TOKEN()
        if ((token->type == T_ID) && (token->next->type != T_EQL) &&
            (token->next->type != T_GT) && (token->next->type != T_GTE) &&
            (token->next->type != T_LT) && (token->next->type != T_LTE) &&
            (token->next->type != T_NEQL) && (token->next->type != T_MUL) &&
            (token->next->type != T_SUB) && (token->next->type != T_ADD) &&
            (token->next->type != T_DIV) && (token->next->type != T_IDIV) &&
            (token->next->type != T_STRLEN) && (token->next->type != T_CONCAT)) {
            if (token->next->type == T_LEFT_PAR) {
                GET_TOKEN()
                CHECK_TYPE(T_LEFT_PAR);

                GET_TOKEN()
                entry_list_params(functionData);

                GET_TOKEN()
                CHECK_TYPE(T_RIGHT_PAR);

                GET_TOKEN()
                return_value_next(functionData);
            } else {
                GET_TOKEN()
                return_value_next(functionData);
            }
        } else {
            DLList expression_list;
            DLL_Init(&expression_list);
            while (token->type != T_EOF) {
                INIT_TOKEN_POINTER()
                DLL_InsertLast(&expression_list, tmp);
                if (token->type == T_LEFT_PAR) {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                    expression_par_tmp(&expression_list);
                }
                if ((token->type == T_ID) || (token->type == T_INT) ||
                    (token->type == T_DOUBLE) || (token->type == T_STRING) ||
                    (token->type == T_RIGHT_PAR) || (token->type == T_K_NIL)) {
                    IS_EXPRESSION()
                    DLL_DeleteBefore(&token_list);
                } else {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                }
            }
            GET_TOKEN()
            token_ptr prec_token = expression(&expression_list, 0, &stack_bst_tree);
            DLL_InsertBefore(&token_list, prec_token);

            return_value_next(functionData);
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
    if ((token->type == T_ID) || (token->type == T_INT) ||
        (token->type == T_DOUBLE) || (token->type == T_STRING) ||
        (token->type == T_STRLEN) || token->type == T_LEFT_PAR ||
        token->type == T_K_NIL) {
        if ((token->type == T_ID) && (token->next->type != T_EQL) &&
            (token->next->type != T_GT) && (token->next->type != T_GTE) &&
            (token->next->type != T_LT) && (token->next->type != T_LTE) &&
            (token->next->type != T_NEQL) && (token->next->type != T_MUL) &&
            (token->next->type != T_SUB) && (token->next->type != T_ADD) &&
            (token->next->type != T_DIV) && (token->next->type != T_IDIV) &&
            (token->next->type != T_STRLEN) && (token->next->type != T_CONCAT)) {
            if (token->next->type == T_LEFT_PAR) {
                token_ptr token_ID = token;
                functionPtrData tmpData;
                INIT_FUNCTION_DATA(functionData)

                if (global_bst_search(bst_tree_of_functions, token->data->string->data, &tmpData) == 0) {
                    err_call(ERR_SMNTIC_UNDEFINED_F, token);
                }

                GET_TOKEN()
                CHECK_TYPE(T_LEFT_PAR);

                GET_TOKEN()
                entry_list_params(functionData);

                if (strcmp(token_ID->data->string->data, "write") != 0) {
                    CHECK_COUNT_OF_PARAMS()
                }

                GET_TOKEN()
                CHECK_TYPE(T_RIGHT_PAR);
            } else {

                LocalBSTNodePtr tmpValue;
                bool isFound = false;
                for (int i = stack_bst_tree.topIndex; i >= 0; i--) {
                    if (local_bst_search(stack_bst_tree.array[i], token->data->string->data, &tmpValue) == true) {
                        isFound = true;
                        break;
                    }
                }
                if (isFound == false) {
                    err_call(ERR_SMNTIC_UNDEFINED_V, token);
                }

                if (valueType != tmpValue->type) {
                    err_call(ERR_SMNTIC_TYPE, token);
                }
            }
        } else {
            DLList expression_list;
            DLL_Init(&expression_list);
            while (token->type != T_EOF) {
                INIT_TOKEN_POINTER()
                DLL_InsertLast(&expression_list, tmp);
                if (token->type == T_LEFT_PAR) {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                    expression_par_tmp(&expression_list);
                }
                if ((token->type == T_ID) || (token->type == T_INT)
                    || (token->type == T_DOUBLE) || (token->type == T_STRING)
                    || (token->type == T_RIGHT_PAR) || (token->type == T_K_NIL)) {
                    IS_EXPRESSION()
                    DLL_DeleteBefore(&token_list);
                } else {
                    GET_TOKEN()
                    DLL_DeleteBefore(&token_list);
                }
            }
            token_ptr prec_token = expression(&expression_list, 0, &stack_bst_tree);
            DLL_InsertAfter(&token_list, prec_token);
            GET_TOKEN()
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

void expression_par_tmp(DLList *list) {
    while (token->type != T_RIGHT_PAR) {
        INIT_TOKEN_POINTER()
        if (token->type == T_LEFT_PAR) {
            DLL_InsertLast(list, tmp);
            GET_TOKEN()
            DLL_DeleteBefore(&token_list);
            expression_par_tmp(list);
        } else {
            DLL_InsertLast(list, tmp);
        }
        GET_TOKEN()
        DLL_DeleteBefore(&token_list);
    }
    INIT_TOKEN_POINTER()
    DLL_InsertLast(list, tmp);

}

void built_in_functions() {
    INIT_FUNCTION_DATA(reads)
    reads->numOfParams = 0;
    reads->numOfReturns = 1;
    reads->returnsType[0] = T_K_STRING;

    INIT_FUNCTION_DATA(readi)
    readi->numOfParams = 0;
    readi->numOfReturns = 1;
    readi->returnsType[0] = T_K_INTEGER;

    INIT_FUNCTION_DATA(readn)
    readn->numOfParams = 0;
    readn->numOfReturns = 1;
    readn->returnsType[0] = T_K_NUMBER;

    INIT_FUNCTION_DATA(write)
    write->numOfParams = MAXPARAMS;
    write->numOfReturns = 0;

    INIT_FUNCTION_DATA(toInteger)
    toInteger->numOfParams = 1;
    toInteger->numOfReturns = 1;
    toInteger->returnsType[0] = T_K_INTEGER;
    toInteger->paramsType[0] = T_K_NUMBER;

    INIT_FUNCTION_DATA(substr)
    substr->numOfParams = 3;
    substr->numOfReturns = 1;
    substr->returnsType[0] = T_K_STRING;
    substr->paramsType[0] = T_K_STRING;
    substr->paramsType[1] = T_K_NUMBER;
    substr->paramsType[2] = T_K_NUMBER;

    INIT_FUNCTION_DATA(ord)
    ord->numOfParams = 2;
    ord->numOfReturns = 1;
    ord->returnsType[0] = T_K_INTEGER;
    ord->paramsType[0] = T_K_STRING;
    ord->paramsType[1] = T_K_INTEGER;

    INIT_FUNCTION_DATA(chr)
    chr->numOfParams = 1;
    chr->numOfReturns = 1;
    chr->returnsType[0] = T_K_INTEGER;
    chr->paramsType[0] = T_K_STRING;

    global_bst_insert(&bst_tree_of_functions, "reads", reads);
    global_bst_insert(&bst_tree_of_functions, "readi", readi);
    global_bst_insert(&bst_tree_of_functions, "readn", readn);
    global_bst_insert(&bst_tree_of_functions, "write", write);
    global_bst_insert(&bst_tree_of_functions, "toInteger", toInteger);
    global_bst_insert(&bst_tree_of_functions, "substr", substr);
    global_bst_insert(&bst_tree_of_functions, "ord", ord);
    global_bst_insert(&bst_tree_of_functions, "chr", chr);
}