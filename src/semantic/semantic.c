/******************************************************************************
 *                                  IFJ22
 *                                symtable.c
 * 
 *      Authors: Alžbeta Hricovová (xhrico00)
 *      Purpose: Semantic control
 * 
 *                        Last change: 27. 11. 2022
 *****************************************************************************/
#include "semantic.h"


int varToken(const program_t *program, int poss, Symtable *globalTable, token_t token, Symtable *localTable, bool local) {
    SymtableData *data = malloc(sizeof(SymtableData));
    if(data == NULL) {
        return 99;
    }
    uint32_t key = getKey(program->tokens[poss].textData.str);
    Symtable *table;
    Symtable *get;
    if(local) {
        table = localTable; 
    } else {
        table = globalTable;
    }
    get = table;

    int err = 0;
    switch(program->tokens[poss].type) {            //fun, (int, float, string); alebo vyraz, var; alebo var vyraz
        case FUN:
            err = funCallToken(program, poss+2, globalTable, program->tokens[poss], localTable, local);
            if(err != 0) {
                return err;
            } 
            add_var(data, symtableGet(globalTable, getKey(program->tokens[poss].textData.str))->dtype.func_type.retype);
            if(insertSymtable(table, getKey(token.textData.str), data)) {
                return 99;
            }
            break;

        case TYPE:
            return 8;
            break;

        case VAR:
            if(program->tokens[poss+1].type == SEMICOLON) {
                if(!symtableSearch(table, getKey(program->tokens[poss].textData.str))) {
                    return 5;
                } else {
                    
                    add_var(data, (symtableGet(table, getKey(program->tokens[poss].textData.str)))->dtype.var_type);
                    if(insertSymtable(table, getKey(token.textData.str), data)) {
                        return 99;
                    }
                }
                
                add_var(data, (symtableGet(get, key))->dtype.var_type);
                if(insertSymtable(table, getKey(token.textData.str), data)) {
                    return 99;
                }
               
                               
            } else {    //vyraz
                
            }
            break;
        default:
            if(program->tokens[poss+1].type == SEMICOLON) {
                add_var(data, typeIndChoice(program->tokens[poss]));
                if(insertSymtable(table, getKey(token.textData.str), data)) {
                    return 99;
                }
                
            } else {    //vyraz
                
            }
            break;

    }
    return 0;

}

// volanie funkcie, nie definicia
int funCallToken(const program_t *program, int poss, Symtable *globalTable, token_t token, Symtable *localTable, bool local) {     //pridat ci hladat v loaklnej a potom v globalnej tabulke param
    Symtable *table;
    if(local) {
        table = localTable; 
    } else {
        table = globalTable;
    }
    SymtableData *data = symtableGet(globalTable, getKey(token.textData.str));
    TypesInd *funParam = return_param_func(*data);
    size_t count = 0;
    
    while(program->tokens[poss].type != BR_C) {

        if((count + 1) > data->dtype.func_type.lenght) {    
            return 4;
        }

        if(program->tokens[poss].type == VAR) {
            uint32_t key = getKey(program->tokens[poss].textData.str);
            SymtableData *paramVar;
            if(!symtableSearch(table, key)) {
                if(local) { 
                    if(!symtableSearch(globalTable, key)) {
                        return 5;
                    } 
                    paramVar = symtableGet(globalTable, key);
                } else {
                    return 5;
                }
            } else {
                paramVar = symtableGet(table, key);
            }
            
            if(paramVar->dtype.var_type != funParam[count]) {
                return 4;
            }
        } else {
            if(typeIndChoice(program->tokens[poss]) != funParam[count]) {
                return 4;
            }                   
        }

        count++;
        poss++;
    }

    if(count != data->dtype.func_type.lenght) {    
        return 4;
    }   

    return 0;
}


/*//TEMPORARY!!!!!!
void printAllFuncParams(Symtable funcTable) {
    if (funcTable == NULL)
        return;
    printAllFuncParams(funcTable->right);
    printAllFuncParams(funcTable->left);
    printf("FUN %lld: \t", (long long)funcTable->key);
    for (int i = 0; i < (int)funcTable->data.dtype.func_type.lenght; i++)
        printf("%d, ", (int)funcTable->data.dtype.func_type.func_params[i]);
    printf("\n\tReturn type: %d\n", (int)funcTable->data.dtype.func_type.retype);
}*/

int semanticControl(const program_t *program) {
    Symtable globalTable = NULL;
    bool local = false;

    Symtable funcTable;
    int getFunTableRet = getFunTable(program, &funcTable);
    if (getFunTableRet)
        return getFunTableRet;

   /** 
    SymtableData *data;
    if ((data = malloc(sizeof(SymtableData))) == NULL) {
                        return 99;
    }
    add_func(data);
    add_func_param(data, INT_IND);
    add_retype(data, INT_IND);
    uint32_t k = getKey("ahoj");
    insertSymtable(&globalTable, k, data); **/


    Symtable localTable = NULL;
    int err;
    SemStates state = S_START;
    SymtableData *data;
    token_t tok;
    
    for (int i = 0; i < program->tokenCount; i++) {     //dalej key words a vyraz bez priradenia (string + int), return, vyraz
        tok = program->tokens[i];
        pfExpr_t expr;
        switch (state) {
            case S_START:                
                if (tok.type == END || tok.type == PHP_END) {
                    state = S_END;
                } else if (tok.type == KW && tok.numericData.ivalue == (long long)KW_FUNC_IND) {    // } poriesit koniec funkcie- vypnut local
                    // free localTable
                    localTable = NULL;
                    state = S_GET_PARAM;
                } else if (tok.type == VAR) {
                    state = S_VAR_ROW;
                } else if (tok.type == FUN) {
                    state = S_FUN_CALL;
                }
                
                continue;

            case S_FUN_CALL:
                err = funCallToken(program, i+1, &globalTable, program->tokens[i-1], &localTable, local);
                if(err != 0) {
                    return err;
                } 
                state = S_ROW_END;
                continue;   

            case S_VAR_ROW:
                if(tok.type == ASSIG) {     //priradenie $a = ...
                    err = varToken(program, i+1, &globalTable, program->tokens[i-1], &localTable, local);
                    if(err != 0) {
                        return err;
                    }                    
                } else {      // $a + $b; bez priradenia poriesit
                    
                    // expr = makeExpression(program, i-1);
                    // for (int i = 0; i < expr.size; i++) {
                    //     printf("%s \n", expr.expr[i].textData.str);
                    // }
                    // TypesInd type;
                    // evaluateExpression(&globalTable ,&expr, &type);
                    // printf("Resolved:%i\n", type);

                }
                state = S_ROW_END;
                continue;
        
            case S_GET_PARAM:
                if (tok.type == TYPE) {
                    if ((data = malloc(sizeof(SymtableData))) == NULL) {
                        return 99;
                    }
                    add_var(data, (TypesInd)tok.numericData.ivalue);
                }
                else if (tok.type == VAR) {
                    if(insertSymtable(&localTable, getKey(tok.textData.str), data)) {
                        return 99;
                    }
                } else if (tok.type == BR_C) {
                    state = S_FUN_RET;
                }
                continue;

            case S_FUN_RET:
                if(tok.type == CB_O) {
                    local = true;
                    state = S_START;
                } 
                continue;
        
            case S_ROW_END:
                if(tok.type == SEMICOLON) {
                    state = S_START;
                }
                continue;

            case S_END:
                break;
            
            default:
                break;
        }
    }
    return 0;
}

int getFunTable(const program_t * program, Symtable * funcTable) {
    Symtable newFuncTable = NULL;
    FunDefsStates state = SF_START;

    uint32_t key; 
    SymtableData * newFuncData;
    bst_t * params;
    bstInit(&params);
    string_t * name;
    key_t paramKey;

    for (int i = 0; i < program->tokenCount; i++) {
        switch (state) {
            case SF_START:
                if (program->tokens[i].type == END || program->tokens[i].type == PHP_END)
                    state = SF_END;
                else if (program->tokens[i].type == KW && program->tokens[i].numericData.ivalue == (long long)KW_FUNC_IND)
                    state = SF_FUN_DEC;
                continue;
            case SF_FUN_DEC:
                key = getKey(stringRead(&(program->tokens[i].textData)));
                if (symtableSearch(&newFuncTable, key))
                    return 3;
                if ((newFuncData = malloc(sizeof(SymtableData))) == NULL)
                    return 99;
                add_func(newFuncData);
                state = SF_SKIP_BR;
                continue;
            case SF_GET_PARAM:
                if (program->tokens[i].type == TYPE) {
                    add_func_param(newFuncData, (TypesInd)program->tokens[i].numericData.ivalue);
                    state = SF_SKIP_NAME;
                }
                else if (program->tokens[i].type == BR_C)
                    state = SF_SKIP_COLON;
                continue;
            case SF_SKIP_NAME:
                name = &(program->tokens[i].textData);
                paramKey = get_key(stringRead(name));
                if (bstSearch(params, paramKey))
                    return 8;
                bstInsert(&params, paramKey, (bstData_t)name);
                state = SF_CHECK_NEXT;
                continue;
            case SF_CHECK_NEXT:
                if (program->tokens[i].type == COMMA)
                    state = SF_GET_PARAM;
                else if (program->tokens[i].type == BR_C)
                    state = SF_SKIP_COLON;
                continue;
            case SF_GET_TYPE:
                if (program->tokens[i].type == QUEST)
                    state = SF_GET_QUEST_TYPE;
                else if (program->tokens[i].type == TYPE) {
                    add_retype(newFuncData, (TypesInd)program->tokens[i].numericData.ivalue);
                    state = SF_FUN_START;
                }
                continue;
            case SF_FUN_START:
                insertSymtable(&newFuncTable, key, newFuncData);
                bstDestroy(&params);
                state = SF_START;
                continue;
            case SF_END:
                break;
            default:
                state++;
        }
    }
    *funcTable = newFuncTable;
    return 0;
}