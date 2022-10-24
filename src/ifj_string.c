/******************************************************************************
 *                                  IFJ22
 *                              ifj_string.c
 * 
 *      Authors: Nikita Kotvitskiy (xkotvi01)
 *      Purpose: Declaration of operations on strings
 * 
 *                        Last change: 24. 10. 2022
 *****************************************************************************/

#include "ifj_string.h"
#include <stdio.h>
#include <stdlib.h>

void stringInit(string_t * str) {
    if ((str->str = malloc(sizeof(char) * (DEFAULT_STRING_LENGTH + 1))) == NULL)
        fprintf(stderr, "String allocation failure\n");
    str->len = 0;
    str->allocated = DEFAULT_STRING_LENGTH;
    str->str[0] = '\0';
}

void stringFree(string_t * str) {
    free(str->str);
}

void stringAppend(string_t * str, char newChar) {
    if (str->len == str->allocated) {
        if ((str->str = realloc(str->str, sizeof(char) * (str->allocated + DEFAULT_STRING_LENGTH + 1))) == NULL)
            fprintf(stderr, "String allocation failure\n");
        str->allocated += DEFAULT_STRING_LENGTH;
    }
    str->str[str->len] = newChar;
    str->len++;
    str->str[str->len] = '\0';
}

void stringClear(string_t * str)
{
    stringFree(str);
    stringInit(str);
}

void stringCopy(string_t * strDest, string_t * strSource)
{
    if ((strDest->str = malloc(sizeof(char) * strSource->allocated + 1)) == NULL)
        fprintf(stderr, "String allocation failure\n");
    strDest->len = strSource->len;
    strDest->allocated = strSource->allocated;
    strcpy(strDest->str, strSource->str);
}

char * stringRead(string_t * str)
{
    return str->str;
}

int stringLength(string_t * str)
{
    return str->len;
}