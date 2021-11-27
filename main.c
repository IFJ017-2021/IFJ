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
  token_ptr tokenPtr0 = malloc(sizeof (struct token));
  tokenPtr0->type = T_K_STRING;
  //tokenPtr->data->string = "ifj21";
  DLL_Init(&token_list);
  DLL_InsertLast(&token_list, tokenPtr0);
  token_ptr tokenPtr1 = malloc(sizeof (struct token));
  tokenPtr1->type = T_K_GLOBAL;
  DLL_Init(&token_list);
  DLL_InsertLast(&token_list, tokenPtr1);
  token_ptr tokenPtr2 = malloc(sizeof (struct token));
  tokenPtr2->type = T_ID;
  DLL_Init(&token_list);
  DLL_InsertLast(&token_list, tokenPtr2);
  token_ptr tokenPtr3 = malloc(sizeof (struct token));
  tokenPtr3->type = T_DOUBLE_DOT;
  DLL_Init(&token_list);
  DLL_InsertLast(&token_list, tokenPtr3);
//  tokenPtr->type = T_K_FUNCTION;
//  DLL_Init(&token_list);
//  DLL_InsertLast(&token_list, tokenPtr);
//  tokenPtr->type = T_LEFT_PAR;
//  DLL_Init(&token_list);
//  DLL_InsertLast(&token_list, tokenPtr);
//  tokenPtr->type = T_RIGHT_PAR;
//  DLL_Init(&token_list);
//  DLL_InsertLast(&token_list, tokenPtr);
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
