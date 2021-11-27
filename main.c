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
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
//  FILE *f = stdin;
//  if (argc == 1) {
//    fprintf(stderr, "Missing a file argument.\n");
//    err_call(ERR_INTERNAL);
//  }
//  if (freopen(argv[1], "r", f)) {
//    fprintf(stderr, "File can't be opened.\n");
//    err_call(ERR_INTERNAL);
//  }

  DLList token_list;
  token_ptr tokenPtr = malloc(sizeof (struct token));
  tokenPtr->type = T_K_REQUIRE;
  DLL_Init(&token_list);
  DLL_InsertLast(&token_list, tokenPtr);

  // Zavolani syntakticke analyzy
  start(&token_list);

//  int lex_return = get_token_list(&token_list);
//  if(lex_return == 1)
//  {
//      err_call(ERR_LEX);
//  }

//  if (lex_return == 0) {
//    print_token_list(*token_list);
//  }

  return 0;
}
