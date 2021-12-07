/**
 *  @file generator.c
 *
 *  @brief Implementation of IFJCode21 generator for IFJ21 compiler.
 *
 *  @author <xvagne10> Dominik Vágner
 */

#include "generator.h"
#include "error.h"
#include "scanner.h"
#include "str.h"
#include "tokenList.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_code_from_list(DLList *list) {
  token_ptr index = NULL;
  DLL_First(list);
  DLL_GetFirst(list, &index);

  if (index == NULL)
    return;

  string *base_code;
  base_code = malloc(sizeof(string));
  strInit(base_code);
  strAppendStr(base_code, ".IFJcode21\nJUMP $$main\n\n");
  // generate_built_in_code(base_code);
  printf("%s", strGetString(base_code));

  string *main;
  main = malloc(sizeof(string));
  strInit(main);
  strAppendStr(main, "\nLABEL $$main\nCREATEFRAME\nPUSHFRAME\n");

  generate_code_from_token(&index, main);

  while (index != NULL) {
    generate_code_from_token(&index, main);
    if (index->next == NULL)
      break;
    else {
      DLL_Next(list);
      DLL_GetValue(list, &index);
    }
  }

  printf("%s", strGetString(main));

  return;
}

void generate_code_from_token(token_ptr *token, string *main) {
  static int def_function = 0;
  static int def_params = 0;
  static int def_retvals = 0;
  static int func_body = 0;
  static int func_call = 0;
  static char *called_func;

  static int curr_param_num = 0;
  static int curr_retval_num = 0;

  static int curr_func_write = 0;

  static int require_read = 0;

  if (!require_read) {
    if ((*token)->type == T_STRING &&
        !strCompareConstant((*token)->data->string, "ifj21")) {
      require_read = 1;
      return;
    } else {
      return;
    }
  }

  token_type t_type = (*token)->type;

  switch (t_type) {
  case T_K_FUNCTION:
    def_function = 1;
    break;

  case T_ID:
    if (def_function && !def_params && !func_body) {
      printf("LABEL %s\nPUSHFRAME\n", strGetString((*token)->data->string));
    } else if (def_params) {
      curr_param_num++;
      printf("DEFVAR LFparam%d\nMOVE LF@param%d LF@\037%d\n", curr_param_num,
             curr_param_num, curr_param_num);
    } else if (func_body) {
      if ((*token)->next->type == T_LEFT_PAR) {
        func_call = 1;
        called_func = strGetString((*token)->data->string);
      }
    } else {
      strAppendStr(main, "CREATEFRAME\nCALL ");
      strAppendStr(main, strGetString((*token)->data->string));
      strAppendChar(main, '\n');
    }
    break;

  case T_LEFT_PAR:
    if (def_function && !func_body) {
      def_params = 1;
      break;
    }
    if (func_call) {
      if (!strcmp(called_func, "write"))
        curr_func_write = 1;
    }
    break;

  case T_RIGHT_PAR:
    if (def_params) {
      def_params = 0;
      curr_param_num = 0;
      if (def_function && (*token)->next->type == T_DOUBLE_DOT) {
        def_retvals = 1;
      } else if (def_function) {
        func_body = 1;
      }
      break;
    }
    break;

  case T_P_EXPRESSION:
    if (curr_func_write) {
      char *test_str = strGetString((*token)->data->string);
      int test_double = (*token)->data->number;
      int test_int = (*token)->data->integer;

      if (strlen(test_str) == 1 && atoll(test_str) == 0 && test_int == 0 &&
          test_double == 0.0) {
        printf("WRITE int@0\n");
      } else if (test_int != 0 && test_double == 0.0) {
        printf("WRITE int@%d\n", (*token)->data->integer);
      } else if (test_int == 0 && test_double != 0.0) {
        printf("WRITE float@%a\n", (*token)->data->number);
      } else if (test_int == 0 && test_double == 0) {
        printf("WRITE ");
        asciiConvert((*token)->data->string);
        printf("\n");
      }
    }
    break;

  case T_K_END:
    if (def_function && func_body) {
      def_function = 0;
      func_body = 0;
      printf("POPFRAME\nRETURN\n");
    }
    break;

  case T_K_INTEGER:
  case T_K_NUMBER:
  case T_K_STRING:
    if (def_retvals) {
      curr_retval_num++;
      printf("DEFVAR LF\037retval%d\nMOVE LF@\037retval%d nil@nil\n",
             curr_retval_num, curr_retval_num);

      if ((*token)->next->type != T_COMMA) {
        def_retvals = 0;
        func_body = 1;
      }
    }
    break;

  default:
    break;
  }

  return;
}

void generate_built_in_code(string *code) {
  strAppendStr(code, "\
LABEL $$check_if_int\n\
TYPE TF@$type_check_result TF@$type_check\n\
JUMPIFNEQ $$type_error $type_check_result string@int\n\
RETURN\n\
\n\
LABEL $$type_error\n\
WRITE string@\\010\\032Incompatible\\032type\\032error.\n\
EXIT int@4\n\
\n");
}

/**
LABEL write\n\
PUSHFRAME\n\
DEFVAR LF@param1\n\
MOVE LF@param1 LF@$1\n\
WRITE LF@param
CALL $$check_if_int"

    if (func_call && curr_func_write) {
      int ret = 0;
      char *str = strGetString((*token)->data->string);
      for (unsigned long i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
          ret = 1;
        }
      }
      if (!ret) {
        printf("WRITE int@%d\n", (*token)->data->integer);
        break;
      }
      char *ptr;
      strtod(strGetString((*token)->data->string), &ptr);
      if (*ptr == '\0') {
        printf("WRITE float@%a\n", (*token)->data->number);
        break;
      }
      printf("WRITE string@%s\n", strGetString((*token)->data->string));
      break;
    }

    if ((currentAsciiVal < 32 && currentAsciiVal > 0) ||
        currentAsciiVal == 92 || currentAsciiVal == 35) {
      if (currentAsciiVal == 0) {
        printf("\\000");
      } else if (currentAsciiVal < 10) {
        printf("\\00%d", currentAsciiVal);
      } else if (currentAsciiVal > 9 && currentAsciiVal < 100) {
        printf("\\0%d", currentAsciiVal);
      } else if (currentAsciiVal > 99) {
        printf("\\%d", currentAsciiVal);
      }
    } else {
      printf("%d", s->data[i]);
    }
**/