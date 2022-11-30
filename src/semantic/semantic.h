/******************************************************************************
 *                                  IFJ22
 *                                symtable.c
 * 
 *      Authors: Alžbeta Hricovová (xhrico00)
 *      Purpose: Semantic control
 * 
 *                        Last change: 23. 11. 2022
 *****************************************************************************/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include "../data/data.h"
#include "../scanner/scanner.h"
#include "../expressions/expr.h"

typedef enum {
    S_START,
    S_END,
    S_FUN_RET,
    S_GET_PARAM,
    S_VAR_ROW,
    S_ROW_END,
    S_FUN_CALL,
    S_WHILE_IF,
    S_CB_O,
    S_ELSE,
    S_RET
} SemStates;

int varToken(const program_t *program, int poss, Symtable *globalTable, token_t token, Symtable *localTable, bool local);
int funCallToken(const program_t *program, int poss, Symtable *globalTable, token_t token, Symtable *localTable, bool local);
int semanticControl(const program_t *program);

typedef enum {
    SF_START,
    SF_END,
    SF_FUN_DEC,
    SF_SKIP_BR,
    SF_GET_PARAM,
    SF_SKIP_NAME,
    SF_CHECK_NEXT,
    SF_SKIP_COLON,
    SF_GET_TYPE,
    SF_GET_QUEST_TYPE,
    SF_FUN_START
} FunDefsStates;

int getFunTable(const program_t * program, Symtable * funcTable);

#endif
