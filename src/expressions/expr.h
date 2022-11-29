/******************************************************************************
 *                                  IFJ22
 *                                 expr.h
 * 
 *      Authors: Nikita Kotvitskiy (xkotvi01)
 *      Purpose: Declaration of structure and functions for expressions transformation to postfix notation.
 * 
 *                        Last change: 29. 11. 2022
 *****************************************************************************/

#ifndef EXPR_H
#define EXPR_H

#include "../scanner/scanner.h"
#include <stdlib.h>
#include <stdbool.h>

typedef token_t * expr_t;

typedef struct PostfixExpression {
    int size;
    expr_t expr;
} pfExpr_t;

typedef void * stackData_t;

typedef struct Stack {
    int size;
    token_t * data; 
} stack_t;

pfExpr_t makeExpression(program_t * prog, int startIndex);
void stackInit(stack_t * stack);
void stackPuch(stack_t * stack, token_t tok);
token_t stackPop(stack_t * stack);
token_t stackTop(stack_t * stack);
bool stackEmpty(stack_t * stack);
void stackFree(stack_t * stack);

#endif