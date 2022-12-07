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
void ifDef(generator_t * gen, FILE * progFile, program_t prog);\
void funCall(generator_t * gen, FILE * progFile, program_t prog);
void detectConstruction(generator_t * gen, FILE * progFile, program_t prog);
void constructionSkip(generator_t * gen, program_t prog);
void comandSkip(generator_t * gen, program_t prog);
void defAllVarsInConstr(generator_t * gen, FILE * progFile, program_t prog);


int generateProgram(program_t prog) {
    generator_t gen;
    gen.counter = 0;
    gen.constrCnt = 0;
    gen.global = true;
    bstInit(&(gen.globalVars));
    bstInit(&(gen.localVars));
    bstInit(&(gen.functions));
    token_t tok;

    FILE * ifjCodeProg;
    if ((ifjCodeProg = fopen(OUTPUT_FILE_NAME, "w")) == NULL)
        return INTERN_ERR;

    fprintf(ifjCodeProg, "%s\n", IFJ_PROG_START);
    fprintf(ifjCodeProg, "%s %s\n%s %s\n%s %s\n%s %s\n", DEFVAR, TMP1, DEFVAR, TMP2, DEFVAR, TMP3, DEFVAR, ARG_CNT);
    fprintf(ifjCodeProg, "%s __main\n", JUMP);
    
    fprintf(ifjCodeProg, WRITE_FUN);
    fprintf(ifjCodeProg, READS_FUN);
    fprintf(ifjCodeProg, READI_FUN);
    fprintf(ifjCodeProg, READF_FUN);
    fprintf(ifjCodeProg, SUBSTRING_FUN);

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
        detectConstruction(&gen, ifjCodeProg, prog);
        gen.currentPosition++;
    }
    
    fprintf(ifjCodeProg, "POPFRAME");
    if (fclose(ifjCodeProg))
        return INTERN_ERR;
    return 0;
}

void detectConstruction(generator_t * gen, FILE * progFile, program_t prog) {
    token_t tok = prog.tokens[gen->currentPosition];
    switch (tok.type) {
        case VAR:
            if (prog.tokens[gen->currentPosition + 1].type == ASSIG)
                varAssignment(gen, progFile, prog);
            else
                expression(gen, progFile, prog);
            break;
        case KW:
            switch ((KeyWordsInd)tok.numericData.ivalue) {
                case KW_FUNC_IND:
                    constructionSkip(gen, prog);
                    break;
                case KW_WHILE_IND:
                    whileDef(gen, progFile, prog);
                    break;
                case KW_IF_IND:
                    ifDef(gen, progFile, prog);
                    break;
                case KW_RET_IND:
                    if (gen->global == false) {
                        if (prog.tokens[gen->currentPosition + 1].type != SEMICOLON) {
                            gen->currentPosition++;
                            expression(gen, progFile, prog);
                        }
                        fprintf(progFile, "POPFRAME\n%s\n", RETURN);
                    }
                    else {
                        fprintf(progFile, "%s\n", EXIT);
                        comandSkip(gen, prog);
                    }
                default:
                    break;
            }
            break;
        case INT:
            expression(gen, progFile, prog);
            break;
        case FLOAT:
            expression(gen, progFile, prog);
            break;
        case STRING:
            expression(gen, progFile, prog);
            break;
        case FUN:
            expression(gen, progFile, prog);
            break;
        case TYPE:
            if (tok.numericData.ivalue == (long long)NULL_IND)
                expression(gen, progFile, prog);
            break;
        default:
            break;
    }
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
    if (prog.tokens[gen->currentPosition].type == FUN) {
        funCall(gen, progFile, prog);
        return;
    }
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
        case STRING:
            fprintf(progFile, "%s%s\n", IFJ_STRING, stringRead(&tok.textData));
            break;
        default:
            fprintf(progFile, "nil@nil\n");
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
    else if (oper == DIV_IND) {
        fprintf(progFile, FLOAT_CONVERTER, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter);
        gen->counter++;
        fprintf(progFile, "%s\n", DIVS);
    }
    else if (oper == CON_IND) {
        fprintf(progFile, CONCATENATION, gen->counter, gen->counter, gen->counter, gen->counter);
        gen->counter++;
    }
    else if (oper == EQ_IND || oper == NE_IND) {
        fprintf(progFile, EQVIVAL, gen->counter, gen->counter, gen->counter, gen->counter);
        gen->counter++;
        if (oper == NE_IND)
            fprintf(progFile, "%s\n", NOTS);
    }
    else {
        fprintf(progFile, NUMERIC_CONVERTER, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter);
        gen->counter++;
        switch (oper) {
            case GT_IND:
                fprintf(progFile, "GTS\n");
                break;
            case LT_IND:
                fprintf(progFile, "LTS\n");
                break;
            case GE_IND:
                fprintf(progFile, "GTS\n");
                fprintf(progFile, "PUSHS GF@__TMP1\nPUSHS GF@__TMP2\n");
                fprintf(progFile, EQVIVAL, gen->counter, gen->counter, gen->counter, gen->counter);
                gen->counter++;
                fprintf(progFile, "ORS\n");
                break;
            case LE_IND:
                fprintf(progFile, "LTS\n");
                fprintf(progFile, "PUSHS GF@__TMP1\nPUSHS GF@__TMP2\n");
                fprintf(progFile, EQVIVAL, gen->counter, gen->counter, gen->counter, gen->counter);
                gen->counter++;
                fprintf(progFile, "ORS\n");
                break;
            default:
                break;
        }
    }
}   

void functionDef(generator_t * gen, FILE * progFile, program_t prog) {
    gen->currentPosition++;
    fprintf(progFile, "%s %s\n", LABEL, stringRead(&(prog.tokens[gen->currentPosition].textData)));
    gen->global = false;
    token_t tok;

    while(true) {
        gen->currentPosition++;
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == VAR) {
            fprintf(progFile, "%s %s%s\n%s %s%s\n", DEFVAR, TMP_VAR, tok.textData.str, POPS, TMP_VAR, tok.textData.str);
            key_t key = get_key(tok.textData.str);
            bstInsert(&(gen->localVars), key, NULL);
        }
        if (tok.type == BR_C)
            break;
    }

    while(true) {
        gen->currentPosition++;
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == CB_O)
            break;
    }

    fprintf(progFile, "%s\n", PUSHFRAME);
    while (true) {
        detectConstruction(gen, progFile, prog);
        gen->currentPosition++;
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == CB_C)
            break;
    }
    fprintf(progFile, FUNEND);
    gen->global = true;
    bstDestroy(&(gen->localVars));
}

void whileDef(generator_t * gen, FILE * progFile, program_t prog) {
    if (gen->constrCnt == 0)
        defAllVarsInConstr(gen, progFile, prog);
    int constrInd = gen->counter;
    gen->counter++;
    gen->currentPosition += 2;
    fprintf(progFile, "LABEL __WHILE_%d_START\n", constrInd);
    expression(gen, progFile, prog);
    while (true)
    {
        gen->currentPosition++;
        if (prog.tokens[gen->currentPosition].type == CB_O)
            break;
    }
    gen->constrCnt++;

    fprintf(progFile, LOGIC_CHECK, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter);
    gen->counter++;
    fprintf(progFile, "%s bool@false\nJUMPIFEQS __WHILE_%d_END\n", PUSHS, constrInd);
    token_t tok = prog.tokens[gen->currentPosition];
    while(true) {
        detectConstruction(gen, progFile, prog);
        gen->currentPosition++;
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == CB_C)
            break;
    }
    printf("%d, %lld\n", prog.tokens[gen->currentPosition].type, prog.tokens[gen->currentPosition].numericData.ivalue);
    fprintf(progFile, "JUMP __WHILE_%d_START\nLABEL __WHILE_%d_END\n", constrInd, constrInd);
    gen->constrCnt--;
}

void ifDef(generator_t * gen, FILE * progFile, program_t prog) {
    if (gen->constrCnt == 0)
        defAllVarsInConstr(gen, progFile, prog);
    int constrInd = gen->counter;
    gen->counter++;
    gen->currentPosition += 2;
    expression(gen, progFile, prog);
    while (true)
    {
        gen->currentPosition++;
        if (prog.tokens[gen->currentPosition].type == CB_O)
            break;
    }
    gen->constrCnt++;
    
    fprintf(progFile, LOGIC_CHECK, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter, gen->counter);
    gen->counter++;
    fprintf(progFile, "%s bool@false\nJUMPIFEQS __IF_%d_ELSE\n", PUSHS, constrInd);\
    token_t tok = prog.tokens[gen->currentPosition];
    while(true) {
        detectConstruction(gen, progFile, prog);
        gen->currentPosition++;
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == KW && tok.numericData.ivalue == (long long)KW_ELSE_IND)
            break;
    }
    fprintf(progFile, "JUMP __IF_%d_END\n", constrInd);
    fprintf(progFile, "%s __IF_%d_ELSE\n", LABEL, constrInd);
    while(true) {
        detectConstruction(gen, progFile, prog);
        gen->currentPosition++;
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == CB_C)
            break;
    }
    printf("%d, %lld\n", prog.tokens[gen->currentPosition].type, prog.tokens[gen->currentPosition].numericData.ivalue);
    fprintf(progFile, "LABEL __IF_%d_END\n", constrInd);
    gen->constrCnt--;
}

void funCall(generator_t * gen, FILE * progFile, program_t prog) {
    string_t funName = prog.tokens[gen->currentPosition].textData;
    fprintf(progFile, "%s\n", CREATEFRAME);
    token_t tok;
    gen->currentPosition++;
    int argCnt = 0;
    while (true) {
        if (prog.tokens[gen->currentPosition].type == BR_C)
            break;
        gen->currentPosition++;
    }
    while (true) {
        tok = prog.tokens[gen->currentPosition];
        if (tok.type == INT || tok.type == FLOAT || tok.type == STRING || tok.type == VAR || (tok.type == KW && tok.numericData.ivalue == (long long)NULL_IND)) {
            ifjStackPush(progFile, tok);
            argCnt++;
        }
        if (tok.type == BR_O)
            break;
        gen->currentPosition--;
    }
    fprintf(progFile, "MOVE GF@__ARG_CNT int@%d\n", argCnt);
    fprintf(progFile, "%s %s\n", CALL, funName.str);
}

void defAllVarsInConstr(generator_t * gen, FILE * progFile, program_t prog) {
    gen->subPosition = gen->currentPosition;
    while(true) {
        if (prog.tokens[gen->subPosition].type == CB_O)
            break;
        gen->subPosition++;
    }
    gen->subPosition++;
    gen->constrCnt++;
    token_t tok;
    bst_t ** table = gen->global ? &(gen->globalVars) : &(gen->localVars);
    while (true) {
        tok = prog.tokens[gen->subPosition];
        if (tok.type == VAR && prog.tokens[gen->subPosition + 1].type == ASSIG) {
            key_t key = get_key(tok.textData.str);
            if (!bstSearch(*table, key)) {
                fprintf(progFile, "DEFVAR LF@%s\n", tok.textData.str);
                bstInsert(table, key, NULL);
            }
        }
        if (tok.type == CB_O)
        {
            gen->constrCnt++;
        }
        if (tok.type == CB_C)
        {
            gen->constrCnt--;
            if (gen->constrCnt == 0)
                break;
        }
        gen->subPosition++;
    }
}