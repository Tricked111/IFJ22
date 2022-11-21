/******************************************************************************
 *                                  IFJ22
 *                                scanner.c
 * 
 *      Authors: Daniil Kniazkin (xkniaz00)
 *      Purpose: Definition of operations on scanner and token structures
 * 
 *                        Last change: 18. 11. 2022
 *****************************************************************************/
#include "symtable.h"


void init_table(symtable *table){
    (*table) = NULL;
}







void insert_symbol(symtable *table,const char *key,sym_data_t new_data){

    while(*table){
        if(strcmp((*table)->key,key) == 0){
            //data_free(&(*table)->data);
            (*table)->data = new_data;
            break;
        }
        else if(strcmp((*table)->key,key) > 0){
            table = &(*table)->lptr;
        }
        else{
            table = &(*table)->rptr;
        }
    }

    if((*table) == NULL){
        *table = (sym_tree_t *)malloc(sizeof(sym_tree_t));
        if(*table == NULL){
            return;
        }
        if(mem_for_key(&(*table)->key,key,strlen(key))){
            *table = NULL;
            return;
        }

        (*table)->data = new_data;
        (*table)->lptr = NULL;
        (*table)->rptr = NULL;

    }


}

int mem_for_key(char **str,const char *str2,size_t len){
    *str = (char *)malloc(sizeof(char) * (len +1));
    if(!(*str)) return EXIT_FAILURE;

    for(size_t i = 0;i <= len;i++){
        (*str)[i] = str2[i];
    }
    return EXIT_SUCCESS;
}



