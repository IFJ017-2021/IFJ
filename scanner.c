/**
 *  @file scanner.c
 *
 *  @brief Implementation of lexical analyzer.
 *
 *  @author <xvagne10> Dominik Vágner
 */

#include "scanner.h"
#include "error.h"
#include <stdlib.h>

int get_token_list(token_ptr *first) {
  token_ptr prev = NULL;

  while (prev == NULL || prev->type != T_EOF) {
    token_ptr new;
    int error = get_single_token(&new);

    if (prev == NULL) {
      prev = new;
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

  (*insert_into)->data.string = NULL;
  (*insert_into)->next = NULL;
  (*insert_into)->prev = NULL;

  lex_fsm(insert_into);

  return 0;
};

bool is_token_keyword(token_ptr *token);

void print_token_list(token_ptr first);

void print_single_token(token_ptr token);

int lex_fsm(token_ptr *token);
