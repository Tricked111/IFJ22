#ifndef PARSER_IFJ22
#define PARSER_IFJ22

#include <stdio.h>
#include "../str/ifj_string.h"
#include "../scanner/scanner.h"
#include "../compError.h"
#include "../str/ifj_string.h"
#include "../bst/bst.h"
#include <stdbool.h>
#include <string.h>


typedef enum return_codes {
    PARSE_SUCCESS = 0,
    /* LEXICAL_ERROR_ = 1,
    SYNTAX_ERROR  = 2,
    SEMANTIC_ERROR_DEFINITION = 3,
    SEMANTIC_ERROR_ASSIGNMENT = 4,
    SEMANTIC_ERROR_PARAMETERS = 5,
    SEMANTIC_ERROR_EXPRESSION = 6,
    SEMANTIC_ERROR_OTHER      = 7,
    UNEXPECTED_NIL_ERROR      = 8,
    DIVISION_BY_ZERO_ERROR    = 9,
    INTERNAL_ERROR_ = 99 */
} return_codes_t;




typedef struct rulee {
    int (* rule_function)(scanner_t * sc);
    token_t rule_first;
    bool is_attr;
} rulee_t;



int parseProgram();

int parse_declare(scanner_t * );



int st_list_instruct(scanner_t * sc);
bool compare_token(token_t token,TokenType expecting);
bool compare_token_attr(token_t t,TokenType exp_type,char * exp_attr);


int block_instruct(scanner_t *sc,token_t token);

rulee_t * determinate_rule(token_t token,rulee_t *(*ruleset_f)(size_t));


rulee_t* get_block_rule(size_t index);

int parse_if(scanner_t * sc);

int parse_while(scanner_t *sc);

int parse_function(scanner_t *sc);

int parse_var_ID(scanner_t *sc);

int parse_fun_ID(scanner_t *sc);


int return_instruct(scanner_t *sc);
#endif