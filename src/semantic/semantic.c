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

TypesInd typeIndChoice(token_t* token) {
    switch(token->type) {
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

int varToken(const program_t *program, int poss, Symtable *table, token_t* token) {
    SymtableData *data;
    switch(program->tokens[poss].type) {            //fun, (int, float, string); alebo vyraz, var; alebo var vyraz
        case FUN:
            break;

        case VAR:
            break;
        default:
            if(program->tokens[poss + 1].type == SEMICOLON) {
                add_var(data, typeIndChoice(&(program->tokens[poss])));
                return insertSymtable(table, getKey(stringRead(&(token->textData))), data);
            } else {    //vyraz

            }

    }
    return 0;

}

// volanie funkcie, nie definicia
int funCallToken(const program_t *program, int poss, Symtable *table, token_t* token) {     //pridat ci hladat v loaklnej a potom v globalnej tabulke param
    SymtableData *data = symtableGet(table, getKey(stringRead(&(token->textData))));
    TypesInd * funParam = return_param_func(*data);
    size_t count = 0;
    while(program->tokens[poss].type != CB_C) {
        if(program->tokens[poss].type == VAR) {
            uint32_t key = getKey(stringRead(&(program->tokens[poss].textData)));
            if(!symtableSearch(table, key)) {
                return 5;           //nedefinovana premenna
            }
            SymtableData *paramVar = symtableGet(table, key);
            if(paramVar->dtype.var_type != funParam[count]) {
                return 4;
            }
        } else {
            if(typeIndChoice(&(program->tokens[poss])) != funParam[count]) {
                return 4;
            }
        }

        count++;
        poss++;
    }

    if(count != data->dtype.func_type.lenght) {     //chyba 4
        return 4;
    }

    return 0;
}

int semanticControl(const program_t *program) {
    Symtable *globalTable;
    //Symtable *localTable;
    int err;
    for(int i = 0; i < program->tokenCount; i++) {
        token_t* token = &(program->tokens[i]);

        switch(token->type) {
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
        }
    }

    return 0;
}


