/******************************************************************************
 *                                  IFJ22
 *                                 expr.c
 * 
 *      Authors: Nikita Kotvitskiy (xkotvi01)
 *      Purpose: Definitions of functions used for expressions transformation to postfix notation.
 * 
 *                        Last change: 29. 11. 2022
 *****************************************************************************/

#include "expr.h"
#include "../data/data.h"

void stackInit(stack_t * stack) {
    stack->data = malloc(0);
    stack->size = 0;
}
void stackPuch(stack_t * stack, token_t tok) {
    stack->size++;
    stack->data = realloc(stack->data, sizeof(token_t) * stack->size);
    stack->data[stack->size - 1] = tok;
}
token_t stackPop(stack_t * stack) {
    token_t tok = stack->data[stack->size - 1];
    stack->size--;
    stack->data = realloc(stack->data, sizeof(token_t) * stack->size);
    return tok;
}
token_t stackTop(stack_t * stack) {
    token_t tok = stack->data[stack->size - 1];
    return tok;
}
bool stackEmpty(stack_t * stack) {
    if (stack->size == 0)
        return true;
    return false;
}
void stackFree(stack_t * stack) {
    free(stack->data);
}

void exprAdd(pfExpr_t * expr, token_t tok);
bool lowerOper(token_t oper1, token_t oper2);

pfExpr_t makeExpression(program_t * prog, int startIndex) {
    pfExpr_t expr;
    expr.expr = malloc(0);
    expr.size = 0;
    stack_t stack;
    stackInit(&stack);
    int brokes;

    while (1) {
        TokenType tt = prog->tokens[startIndex].type;
        if (tt == INT || tt == FLOAT || tt == STRING || tt == VAR || (tt == TYPE && prog->tokens[startIndex].numericData.ivalue == (long long)NULL_IND))
            exprAdd(&expr, prog->tokens[startIndex]);
        else if (tt == BR_O) {
            brokes++;
            stackPuch(&stack, prog->tokens[startIndex]);
        }
        else if (tt == OPER) {
            if (stackEmpty(&stack) || stackTop(&stack).type == BR_O || (stackTop(&stack).type == OPER && lowerOper(stackTop(&stack), prog->tokens[startIndex])))
                stackPuch(&stack, prog->tokens[startIndex]);
            else {
                exprAdd(&expr, stackPop(&stack));
                continue;
            }
        }
        else if (tt == BR_C && brokes) {
            token_t tok;
            while ((tok = stackPop(&stack)).type != BR_O)
                exprAdd(&expr, tok);
        }
        else {
            while (!stackEmpty(&stack))
                exprAdd(&expr, stackPop(&stack));
            break;
        }

        startIndex++;
    }

    stackFree(&stack);
    return expr;
}

void exprAdd(pfExpr_t * expr, token_t tok) {
    expr->size++;
    expr->expr = realloc(expr->expr, sizeof(token_t) * expr->size);
    expr->expr[expr->size - 1] = tok;
}

int findOperInd(OperatorsInd * operSet, OperatorsInd oper) {
    int index = 0;
    while (1) {
        if (operSet[index] == oper)
            return index;
        index++;
    }
}
bool lowerOper(token_t oper1, token_t oper2) {
    OperatorsInd oper1Ind = (OperatorsInd)oper1.numericData.ivalue;
    OperatorsInd oper2Ind = (OperatorsInd)oper2.numericData.ivalue;
    OperatorsInd sortedOpers[] = {  MUL_IND,    DIV_IND,    ADD_IND,    SUB_IND,
                                    CON_IND,    LT_IND,     GT_IND,     LE_IND,
                                    GE_IND,     EQ_IND,     NE_IND  };
    if (findOperInd(sortedOpers, oper1Ind) > findOperInd(sortedOpers, oper2Ind))
        return true;
    return false;   
}