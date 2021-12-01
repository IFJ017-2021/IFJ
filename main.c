/**
 *  @file main.c
 *
 *  @brief Main file of the program.
 *
 *  @author <xvagne10> Dominik Vágner
 *  @author <xvasin11> Ladislav Vašina
 *  @author <xpoliv06> Tomáš Polívka
 *  @author <xhajek51> Vojtěch Hájek
 *
 */

#include "error.h"
#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *f;
  if (argc == 1) {
    fprintf(stderr, "Missing a file argument.\n");
    return ERR_INTERNAL;
  }
  if ((f = freopen(argv[1], "r", stdin)) == NULL) {
    fprintf(stderr, "File can't be opened.\n");
    return ERR_INTERNAL;
  }

  token_ptr *token_list = malloc(sizeof(struct token));

  int lex_return = get_token_list(token_list);

  if (lex_return == 0) {
    print_token_list(*token_list);
  }

  return 0;
}
