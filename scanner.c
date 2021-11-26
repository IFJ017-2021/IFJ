/**
 *  @file scanner.c
 *
 *  @brief Implementation of lexical analyzer.
 *
 *  @author <xvagne10> Dominik Vágner
 */

#include "scanner.h"
#include "error.h"
#include "str.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int get_token_list(token_ptr *first) {
  token_ptr prev = NULL;

  while (prev == NULL || prev->type != T_EOF) {
    token_ptr new;
    int error = get_single_token(&new);

    if (prev == NULL) {
      *first = new;
    } else {
      prev->next = new;
      new->prev = prev;
      prev = new;
    }

    if (error) {
      // TODO destroy list
      return ERR_LEX;
    }
  }

  return 0;
}

int get_single_token(token_ptr *insert_into) {
  *insert_into = malloc(sizeof(struct token));

  if (*insert_into == NULL) {
    return ERR_INTERNAL;
  }

  (*insert_into)->data->string = NULL;
  (*insert_into)->next = NULL;
  (*insert_into)->prev = NULL;

  lex_fsm(insert_into);

  is_token_keyword(insert_into);

  return 0;
};

bool is_token_keyword(token_ptr *token);

void print_token_list(token_ptr first);

void print_single_token(token_ptr token);

int lex_fsm(token_ptr *token) {
  lexfsm_state pstate = S_START;
  lexfsm_state nstate;

  bool fsm_activator = true;
  static unsigned int line_num = 1;
  static unsigned int col_num = 1;

  static uint8_t escape_code = 0;

  string *read_input = NULL;
  strInit(read_input);

  (*token)->type = T_OTHER;
  (*token)->line_num = line_num;
  (*token)->col_num = col_num;

  while (fsm_activator) {
    char current = getc(stdin);
    col_num == 1 ? true : col_num++;
    strAppendChar(read_input, current);
    nstate = S_NULL;

    switch (pstate) {
    case S_START:
      if (current == '-')
        nstate = S_SUB;
      else if (current == '+')
        nstate = S_ADD;
      else if (current == '*')
        nstate = S_MUL;
      else if (current == '/')
        nstate = S_DIV;
      else if (current == '#')
        nstate = S_STRLEN;
      else if (current == '(')
        nstate = S_LEFT_PAR;
      else if (current == ')')
        nstate = S_RIGHT_PAR;
      else if (current == ':')
        nstate = S_DOUBLE_DOT;
      else if (current == '=')
        nstate = S_EQL;
      else if (current == '>')
        nstate = S_GT;
      else if (current == '<')
        nstate = S_LT;
      else if (current == ',')
        nstate = S_COMMA;
      else if (current == '_' || isalpha(current))
        nstate = S_ID;
      else if (current == '~')
        nstate = S_TILDA;
      else if (current == '0')
        nstate = S_INT0;
      else if (isdigit(current))
        nstate = S_INT;
      else if (current == '\"')
        nstate = S_STR_START;
      else if (current == '\n')
        nstate = S_EOL;
      else if (current == EOF)
        nstate = S_EOF;
      else if (isspace(current))
        nstate = S_SPACE;
      break;

    case S_SPACE:
      if (current != '\n' && isspace(current))
        nstate = S_SPACE;
      break;

    case S_SUB:
      if (current == '-')
        nstate = S_COMMENT0;
      else
        (*token)->type = T_SUB;
      break;

    case S_COMMENT0:
      if (current == '\n' || current == EOF) {
        ungetc(current, stdin);
        nstate = S_START;
      } else if (current == '[')
        nstate = S_COMMENT1;
      else
        nstate = S_COMMENT0;
      break;

    case S_COMMENT1:
      if (current == '\n' || current == EOF) {
        ungetc(current, stdin);
        nstate = S_START;
      } else if (current == '[')
        nstate = S_COMMENT2;
      else
        nstate = S_COMMENT1;
      break;

    case S_COMMENT2:
      if (current == EOF) {
        ungetc(current, stdin);
        nstate = S_START;
      } else if (current == ']')
        nstate = S_COMMENT3;
      else
        nstate = S_COMMENT2;
      break;

    case S_COMMENT3:
      if (current == EOF) {
        ungetc(current, stdin);
        nstate = S_START;
      } else if (current == ']')
        nstate = S_START;
      else
        nstate = S_COMMENT2;
      break;

    case S_ADD:
      (*token)->type = T_ADD;
      break;

    case S_MUL:
      (*token)->type = T_MUL;
      break;

    case S_DIV:
      if (current == '/')
        nstate = S_IDIV;
      else
        (*token)->type = T_DIV;
      break;

    case S_IDIV:
      (*token)->type = T_IDIV;
      break;

    case S_STRLEN:
      (*token)->type = T_STRLEN;
      break;

    case S_LEFT_PAR:
      (*token)->type = T_LEFT_PAR;
      break;

    case S_RIGHT_PAR:
      (*token)->type = T_RIGHT_PAR;
      break;

    case S_DOUBLE_DOT:
      (*token)->type = T_DOUBLE_DOT;
      break;

    case S_ASSIGN:
      if (current == '=')
        nstate = S_EQL;
      else
        (*token)->type = T_ASSIGN;
      break;

    case S_EQL:
      (*token)->type = T_EQL;
      break;

    case S_GT:
      if (current == '=')
        nstate = S_GTE;
      else
        (*token)->type = T_GT;
      break;

    case S_GTE:
      (*token)->type = T_GTE;
      break;

    case S_LT:
      if (current == '=')
        nstate = S_LTE;
      else
        (*token)->type = T_LT;
      break;

    case S_LTE:
      (*token)->type = T_LTE;
      break;

    case S_COMMA:
      (*token)->type = T_COMMA;
      break;

    case S_ID:
      if (current == '_' || isalnum(current)) {
        strAppendChar(read_input, current);
        nstate = S_ID;
      } else {
        (*token)->data->string = strGetString(read_input);
        (*token)->type = T_ID;
      }
      break;

    case S_INT:
      if (isdigit(current))
        nstate = S_INT;
      else if (current == '.')
        nstate = S_FP_DOT;
      else if (current == 'e' || current == 'E')
        nstate = S_EXP0;
      else {
        sscanf(strGetString(read_input), "%i", &(*token)->data->integer);
        (*token)->type = T_INT;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_INT0:
      if (isdigit(current))
        nstate = S_INT0;
      else if (current == '.')
        nstate = S_FP_DOT;
      else if (current == 'e' || current == 'E')
        nstate = S_EXP0;
      else if (current == 'x' || current == 'X')
        nstate = S_HEX0;
      else {
        sscanf(strGetString(read_input), "%i", &(*token)->data->integer);
        (*token)->type = T_INT;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_HEX0:
      if (isxdigit(current))
        nstate = S_HEX1;
      else {
        nstate = S_ERR;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_HEX1:
      if (isxdigit(current))
        nstate = S_HEX1;
      else {
        sscanf(strGetString(read_input), "%x", &(*token)->data->integer);
        (*token)->type = T_INT;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_FP_DOT:
      if (isdigit(current))
        nstate = S_NUMBER;
      else {
        nstate = S_ERR;
      }
      strAppendChar(read_input, current);
      break;

    case S_NUMBER:
      if (isdigit(current))
        nstate = S_NUMBER;
      else if (current == 'e' || current == 'E')
        nstate = S_EXP0;
      else {
        sscanf(strGetString(read_input), "%lf", &(*token)->data->number);
        (*token)->type = T_DOUBLE;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_EXP0:
      if (current == '+' || current == '-')
        nstate = S_EXP1;
      else if (isdigit(current))
        nstate = S_EXP2;
      else {
        nstate = S_ERR;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_EXP1:
      if (isdigit(current))
        nstate = S_EXP2;
      else {
        nstate = S_ERR;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_EXP2:
      if (isdigit(current))
        nstate = S_EXP2;
      else {
        sscanf(strGetString(read_input), "%lf", &(*token)->data->number);
        (*token)->type = T_DOUBLE;
        break;
      }
      strAppendChar(read_input, current);
      break;

    case S_EOL:
      line_num++;
      col_num = 1;
      (*token)->type = T_EOL;
      break;

    case S_EOF:
      (*token)->type = T_EOF;
      break;

    case S_DOT0:
      if (current == '.')
        nstate = S_DOUBLE_DOT;
      else
        nstate = S_ERR;
      break;

    case S_DOT1:
      (*token)->type = T_DOUBLE_DOT;
      break;

    case S_TILDA:
      if (current == '=')
        nstate = S_NEQL;
      else
        nstate = S_ERR;
      break;

    case S_NEQL:
      (*token)->type = T_NEQL;
      break;

    case S_STR_START:
      if (current > 31 && current != '\\') {
        nstate = S_STR_START;
        strAppendChar(read_input, current);
      } else if (current == '\"')
        nstate = S_STR_FIN;
      else if (current == '\\') {
        nstate = S_STR_T1;
        escape_code = 0;
      }
      break;

    case S_STR_T1:
      if (current == '\\') {
        strAppendChar(read_input, '\\');
        nstate = S_STR_START;
      } else if (current == '\"') {
        strAppendChar(read_input, '\"');
        nstate = S_STR_START;
      } else if (current == 'n') {
        strAppendChar(read_input, '\n');
        nstate = S_STR_START;
      } else if (current == 't') {
        strAppendChar(read_input, '\t');
        nstate = S_STR_START;
      } else if (current == '0') {
        nstate = S_STR_T2;
      } else if (current == '1') {
        nstate = S_STR_T5;
        escape_code += 100;
      } else if (current == '2') {
        nstate = S_STR_T6;
        escape_code += 200;
      }
      break;

    case S_STR_T2:
      if (current == '0')
        nstate = S_STR_T3;
      else if (isdigit(current)) {
        nstate = S_STR_T4;
        escape_code += ((current - '0') * 10);
      } else {
        nstate = S_ERR;
      }
      break;

    case S_STR_T3:
      if (isdigit(current) && current != '0') {
        escape_code += (current - '0');
        strAppendChar(read_input, escape_code);
        nstate = S_STR_START;
      } else
        nstate = S_ERR;
      break;

    case S_STR_T4:
      if (isdigit(current)) {
        escape_code += (current - '0');
        strAppendChar(read_input, escape_code);
        nstate = S_STR_START;
      } else {
        nstate = S_ERR;
      }
      break;

    case S_STR_T5:
      if (isdigit(current)) {
        escape_code += ((current - '0') * 10);
        nstate = S_STR_T4;
      } else {
        nstate = S_ERR;
      }
      break;

    case S_STR_T6:
      if (current >= '0' && current <= '4') {
        escape_code += ((current - '0') * 10);
        nstate = S_STR_T7;
      } else if (current == '5') {
        nstate = S_STR_T8;
      } else {
        nstate = S_ERR;
      }
      break;

    case S_STR_T7:
      if (isdigit(current)) {
        escape_code += (current - '0');
        strAppendChar(read_input, escape_code);
        nstate = S_STR_START;
      } else {
        nstate = S_ERR;
      }
      break;

    case S_STR_T8:
      if (current >= '0' && current <= '5') {
        escape_code += (current - '0');
        strAppendChar(read_input, escape_code);
        nstate = S_STR_START;
      } else {
        nstate = S_ERR;
      }
      break;

    case S_STR_FIN:
      (*token)->data->string = strGetString(read_input);
      (*token)->type = T_STRING;
      break;

    default:
      break;
    }

    if ((*token)->type == T_OTHER || nstate == S_NULL) {
      break;
    }

    if (nstate == S_ID || nstate == S_INT || nstate == S_INT0)
      strAppendChar(read_input, current);
    else if (pstate == S_START)
      strClear(read_input);

    if (current != EOF)
      col_num++;

    pstate = nstate;
  }

  return nstate == S_ERR ? ERR_LEX : 0;
}