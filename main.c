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
  token_type pole_type[23] = {T_K_REQUIRE, T_STRING, T_K_GLOBAL, T_ID, T_EOL, T_DOUBLE_DOT, T_EOL, T_K_FUNCTION, T_LEFT_PAR, T_EOL, T_RIGHT_PAR, T_EOL,
                              T_K_GLOBAL, T_ID, T_EOL, T_DOUBLE_DOT, T_EOL, T_K_FUNCTION, T_LEFT_PAR, T_EOL, T_RIGHT_PAR, T_EOL, T_EOF};

  DLList token_list;
  DLL_Init(&token_list);

  for (int i = 0; i < 23; i++)
  {
      token_ptr tokenPtr = (token_ptr) malloc(sizeof (struct token));
      tokenPtr->type = pole_type[i];
      DLL_InsertLast(&token_list, tokenPtr);
  }

  // Zavolani syntakticke analyzy
  start(&token_list);

  fprintf(stdout, "%s", "Proslo to");
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
