#ifndef PARSER_IFJ22
#define PARSER_IFJ22

#include <stdio.h>
#include "../str/ifj_string.h"
#include "../scanner/scanner.h"
#include "../compError.h"
#include "../str/ifj_string.h"
#include "../bst/bst.h"

typedef struct Program {
    int tokenCount;
    token_t * tokens;
} program_t;

int parseProgram();
int readPogram(program_t * program, scanner_t * scanner);

#endif