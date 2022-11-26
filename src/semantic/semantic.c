/******************************************************************************
 *                                  IFJ22
 *                                symtable.c
 * 
 *      Authors: Alžbeta Hricovová (xhrico00)
 *      Purpose: Semantic control
 * 
 *                        Last change: 23. 11. 2022
 *****************************************************************************/
#include "semantic.h"

//Source: https://en.wikipedia.org/wiki/Jenkins_hash_function

uint32_t getKey(const char *str)
{   
    uint32_t hash = 0;
    size_t i = 0;
    while (str[i]) {
        hash += str[i];
        hash += hash << 10;
        hash ^= hash >> 6;
        i++;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
} 

TypesInd typeIndChoice(token_t token) {
    switch(token.type) {
        case INT:
            return INT_IND;
        case FLOAT:
            return FLOAT_IND;
        case STRING:
            return STRING_IND;
        default:
            return NULL_IND;    //?
    }
}

int varToken(const program_t *program, int poss, Symtable *table, token_t token) {
    SymtableData data;
    switch(program->tokens[poss].type) {            //fun, (int, float, string); alebo vyraz, var; alebo var vyraz
        case FUN:
            break;

        case VAR:
            if(program->tokens[poss+1].type == SEMICOLON) {
                if(!symtableSearch(table, getKey(program->tokens[poss].textData.str))) {
                    return 5;
                } else {
                    
                    add_var(&data, (symtableGet(table, getKey(program->tokens[poss].textData.str)))->dtype.var_type);
                    if(insertSymtable(table, getKey(token.textData.str), &data)) {
                        return 99;
                    }
                }                
            } else {    //vyraz
                
            }
            break;
        default:
            if(program->tokens[poss+1].type == SEMICOLON) {
                add_var(&data, typeIndChoice(program->tokens[poss]));
                if(insertSymtable(table, getKey(token.textData.str), &data)) {
                    return 99;
                };
                
            } else {    //vyraz
                
            }
            break;

    }
    return 0;

}

// volanie funkcie, nie definicia
int funCallToken(const program_t *program, int poss, Symtable *table, token_t token) {     //pridat ci hladat v loaklnej a potom v globalnej tabulke param
    SymtableData *data = symtableGet(table, getKey(token.textData.str));
    TypesInd *funParam = return_param_func(*data);
    size_t count = 0;

    while(program->tokens[poss].type != BR_C) {

        if((count + 1) > data->dtype.func_type.lenght) {     //chyba 4
            return 4;
        }

        if(program->tokens[poss].type == VAR) {
            uint32_t key = getKey(program->tokens[poss].textData.str);
            if(!symtableSearch(table, key)) {
                return 5;           //nedefinovana premenna
            }
            
            SymtableData *paramVar = symtableGet(table, key);
            if(paramVar->dtype.var_type != funParam[count]) {
                return 4;
            }
        } else {
            if(typeIndChoice(program->tokens[poss]) != funParam[count]) {
                return 4;
            }                    //docasne odkomentovane
        }

        count++;
        poss++;
    }

    if(count != data->dtype.func_type.lenght) {     //chyba 4
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

    Symtable funcTable;
    int getFunTableRet = getFunTable(program, &funcTable);
    if (getFunTableRet)
        return getFunTableRet;

    /**
    SymtableData data;
    add_func(&data);
    add_func_param(&data, INT_IND);
    uint32_t k = getKey("ahoj");
    insertSymtable(&globalTable, k, &data); **/


    //Symtable *localTable;
    int err;
    for(int i = 0; i < program->tokenCount; i++) {

        //printf("\n %s", token->textData.str);
        /**switch(token->type) {
            //printf("\n som tu %d", i);
            case VAR:
                if(program->tokens[i + 1].type == ASSIG) {     //priradenie $a = ...
                    err = varToken(program, i + 2, globalTable, token);
                    if(err == 1) {         //chyba 99?
                        return 99;
                    }
                } else {    
                    if(symtableSearch(globalTable, getKey(stringRead(&(token->textData))))) {         //... = .. + $a;
                    }
                }
                break;
            case FUN:           //dalej key words a vyraz bez priradenia (string + int)
                err = funCallToken(program, i + 2, globalTable, token);
                if(err != 0) {
                    return err;
                }      //i+1 = (

                break;
            case KW:
                break;
            
            default:
                break;
        } **/


        token_t tok = program->tokens[i];
        switch (tok.type) {

            case VAR:   
                if(program->tokens[i+1].type == ASSIG) {     //priradenie $a = ...
                    err = varToken(program, i+2, &globalTable, tok);
                    if(err != 0) {
                        return err;
                    }                    
                } else {
                        
                }
                break;

            case FUN:           //dalej key words a vyraz bez priradenia (string + int)
            /**
                err = funCallToken(program, i+2, &globalTable, tok);
                if(err != 0) {
                    return err;
                }      //i+1 = (  **/

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
                string_t * name = &(program->tokens[i].textData);
                key_t paramKey = get_key(stringRead(name));
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