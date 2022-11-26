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
#include "../symtable/symtable.h"
#include "../data/data.h"
#include "../scanner/scanner.h"

uint32_t getKey(const char *str);
TypesInd typeIndChoice(token_t token);
int varToken(const program_t *program, int poss, Symtable *table, token_t token);
int funCallToken(const program_t *program, int poss, Symtable *table, token_t token);
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
