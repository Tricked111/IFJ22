#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include "../compError.h"
#include "../data/data.h"
#include "../expressions/expr.h"

void varAssignment(generator_t * gen, FILE * progFile, program_t prog);
void expression(generator_t * gen, FILE * progFile, program_t prog);
void functionDef(generator_t * gen, FILE * progFile, program_t prog);
void whileDef(generator_t * gen, FILE * progFile, program_t prog);
void ifDef(generator_t * gen, FILE * progFile, program_t prog);
void constructionSkip(generator_t * gen, program_t prog);
void comandSkip(generator_t * gen, program_t prog);


int generateProgram(program_t prog) {
    generator_t gen;
    gen.counter = 0;
    gen.inWhile = false;
    gen.global = true;
    bstInit(&(gen.globalVars));
    bstInit(&(gen.localVars));
    bstInit(&(gen.functions));
    token_t tok;

    FILE * ifjCodeProg;
    if ((ifjCodeProg = fopen(OUTPUT_FILE_NAME, "w")) == NULL)
        return INTERN_ERR;

    fprintf(ifjCodeProg, "%s\n", IFJ_PROG_START);
    fprintf(ifjCodeProg, "%s %s\n%s %s\n%s %s\n", DEFVAR, TMP1, DEFVAR, TMP2, DEFVAR, TMP3);
    fprintf(ifjCodeProg, "%s __main\n", JUMP);
    
    gen.currentPosition = PROG_START;
    while (gen.currentPosition < prog.tokenCount) {
        tok = prog.tokens[gen.currentPosition];
        if (tok.type == KW && tok.numericData.ivalue == KW_FUNC_IND)
            functionDef(&gen, ifjCodeProg, prog);
        gen.currentPosition++;
    }
    
    fprintf(ifjCodeProg, "%s __main\n%s\n%s\n", LABEL, CREATEFRAME, PUSHFRAME);
    gen.currentPosition = PROG_START;
    while (gen.currentPosition < prog.tokenCount) {
        tok = prog.tokens[gen.currentPosition];
        switch (tok.type) {
            case VAR:
                if (prog.tokens[gen.currentPosition + 1].type == ASSIG)
                    varAssignment(&gen, ifjCodeProg, prog);
                else
                    expression(&gen, ifjCodeProg, prog);
                break;
            case KW:
                switch ((KeyWordsInd)tok.numericData.ivalue) {
                    case KW_FUNC_IND:
                        constructionSkip(&gen, prog);
                        break;
                    case KW_WHILE_IND:
                        whileDef(&gen, ifjCodeProg, prog);
                        break;
                    case KW_IF_IND:
                        ifDef(&gen, ifjCodeProg, prog);
                        break;
                    default:
                        break;
                }
                break;
            case INT:
                expression(&gen, ifjCodeProg, prog);
                break;
            case FLOAT:
                expression(&gen, ifjCodeProg, prog);
                break;
            case STRING:
                expression(&gen, ifjCodeProg, prog);
                break;
            case TYPE:
                if (tok.numericData.ivalue == (long long)NULL_IND)
                    expression(&gen, ifjCodeProg, prog);
                break;
            default:
                break;
        }

        gen.currentPosition++;
    }
    fprintf(ifjCodeProg, "%s", POPFRAME);
    
    if (fclose(ifjCodeProg))
        return INTERN_ERR;
    return 0;
}

void constructionSkip(generator_t * gen, program_t prog) {
    int cbOpen = 0;
    token_t tok;
    while (true) {
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == CB_O) {
            cbOpen++;
        }
        else if (tok.type == CB_C) {
            cbOpen--;
            if (cbOpen == 0)
                break;
        }
        gen->currentPosition++;
    }
}

void comandSkip(generator_t * gen, program_t prog) {
    token_t tok;
    while (true) {
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == SEMICOLON)
            break;
        gen->currentPosition++;
    }
}

void varAssignment(generator_t * gen, FILE * progFile, program_t prog) {
    string_t * varName = &(prog.tokens[gen->currentPosition].textData);
    key_t key = get_key(stringRead(varName));
    bst_t ** table = gen->global ? &(gen->globalVars) : &(gen->localVars);
    if (!bstSearch(*table, key)) {
        fprintf(progFile, "%s %s%s\n", DEFVAR, LOCAL_VAR, stringRead(varName));
        bstInsert(table, key, NULL);
    }
    gen->currentPosition += 2;
    expression(gen, progFile, prog);
    fprintf(progFile, "%s %s%s\n", POPS, LOCAL_VAR, stringRead(varName));
}

void ifjStackPush(FILE * progFile, token_t tok);
void ifjOperate(FILE * progFile, token_t tok, generator_t * gen);
void expression(generator_t * gen, FILE * progFile, program_t prog) {
    pfExpr_t expr = makeExpression(&prog, gen->currentPosition);
    for (int i = 0; i < expr.size; i++) {
        if (expr.expr[i].type == OPER)
            ifjOperate(progFile, expr.expr[i], gen);
        else
            ifjStackPush(progFile, expr.expr[i]);
        gen->currentPosition++;
    }
}

void ifjStackPush(FILE * progFile, token_t tok) {
    fprintf(progFile, "%s ", PUSHS);
    switch (tok.type) {
        case VAR:
            fprintf(progFile, "%s%s\n", LOCAL_VAR, stringRead(&tok.textData));
            break;
        case INT:
            fprintf(progFile, "%s%lld\n", IFJ_INTEGER, tok.numericData.ivalue);
            break;
        case FLOAT:
            fprintf(progFile, "%s%a\n", IFJ_FLOAT, tok.numericData.fvalue);
            break;
        default:
            fprintf(progFile, "%s0\n", IFJ_INTEGER);
            break;
    }
}

void ifjOperate(FILE * progFile, token_t tok, generator_t * gen) {
    OperatorsInd oper = (OperatorsInd)tok.numericData.ivalue;
    if (oper == ADD_IND || oper == SUB_IND || oper == MUL_IND) {
        fprintf(progFile, NUMERIC_CONVERTER, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter);
        gen->counter++;
        switch (oper) {
            case ADD_IND:
                fprintf(progFile, "%s\n", ADDS);
                break;
            case SUB_IND:
                fprintf(progFile, "%s\n", SUBS);
                break;
            case MUL_IND:
                fprintf(progFile, "%s\n", MULS);
                break;
            default:
                break;
        }
    }
    if (oper == DIV_IND) {
        fprintf(progFile, FLOAT_CONVERTER, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter);
        gen->counter++;
        fprintf(progFile, "%s\n", DIVS);
    }
    if (oper == CON_IND) {
        fprintf(progFile, CONCATENATION, gen->counter, gen->counter, gen->counter, gen->counter);
        gen->counter++;
    }
}   

void functionDef(generator_t * gen, FILE * progFile, program_t prog) {
    fprintf(progFile, "#funDef\n");
    constructionSkip(gen, prog);
}

void whileDef(generator_t * gen, FILE * progFile, program_t prog) {
    fprintf(progFile, "#whileDef\n");
    constructionSkip(gen, prog);
}

void ifDef(generator_t * gen, FILE * progFile, program_t prog) {
    fprintf(progFile, "#ifDef\n");
    constructionSkip(gen, prog);
    gen->currentPosition++;
    constructionSkip(gen, prog);
}
