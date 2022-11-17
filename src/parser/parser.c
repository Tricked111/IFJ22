#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define KW_COUNT 2
#define RULE_COUNT 3
#define TOK_COUNT 18
#define TYPE_COUNT

#define RULSE_LENGHT 5

bool compare_token(token_t token,TokenType expecting){
    if (token.type == expecting){
        return true;
    }
    return false;
}

bool compare_token_attr(token_t t,TokenType exp_type,char * exp_attr){
    if(t.type == exp_type){
        if(strcmp(stringRead(&t.textData),exp_attr) == 0){
            return true;
        }
    }
    return false;
}



rulee_t* get_block_rule(size_t index){
    if(index >= RULSE_LENGHT){
        return NULL;
    }
    static rulee_t ruleset_inside[RULSE_LENGHT] = {
        {parse_var_ID,      {VAR,   {0},    {NULL,0,0} }, false},
        {parse_fun_ID,      {FUN,   {0},    {NULL,0,0} }, false},
        {parse_if,          {KW,    {0},    {"if",2,2}       }, true},
        {parse_while,       {KW,    {0},    {"while",5,5}    }, true},
        {parse_function,    {KW,    {0},    {"function",8,8} }, true},
    };

    return &ruleset_inside[index];
}

rulee_t* determinate_rule(token_t token,rulee_t *(*ruleset_f)(size_t)){

    rulee_t * rule = NULL;

    for(size_t i = 0; (rule = ruleset_f(i)) != NULL;i++){
        token_t to_be_checked = rule->rule_first;
        //printf("T: %d\n",to_be_checked.type);

        if(rule->is_attr){
            if(compare_token_attr(token,to_be_checked.type,to_be_checked.textData.str)){
                return rule;
            }
        }
        else{
            if(compare_token(token,to_be_checked.type)){
                return rule;
            }
        }


    }
    //printf("R\n");
    return NULL;
}


//main function 

// <prog> -> <begin> <st-list>
int parseProgram() {

    scanner_t scanner;
    scannerInit(&scanner);


    int result = PARSE_SUCCESS;

    //result = parse_declare(&scanner);


    token_t test;
    for(int i = 0;i <=7;i++){
    test = getToken(&scanner);
    //test = getToken(&scanner);
    printf("T: %d\n",test.type);
    }

    result = (result == PARSE_SUCCESS) ? st_list_instruct(&scanner) : result;



    //<st-list>
    return result;

}

//<st-list>
int st_list_instruct(scanner_t * sc){

    token_t token = getToken(sc);

    if(compare_token(token,END) || compare_token(token,PHP_END)){
        printf("END prog!\n");
        return PARSE_SUCCESS;
    }

    if(compare_token_attr(token,KW,"return")){
        return 0;
    }

    int res = block_instruct(sc,token);

    if(res != PARSE_SUCCESS){
        return res;
    }

    return st_list_instruct(sc);
}



int block_instruct(scanner_t *sc,token_t token){

    rulee_t *rule_to_use = determinate_rule(token,get_block_rule);
    if(rule_to_use == NULL) {
        return SYNTAX_ERR;
    }


    int res = rule_to_use->rule_function(sc);
    
    if(res != PARSE_SUCCESS){
        return res;
    }


    return res;

}



int parse_declare(scanner_t * scanner){
    token_t token;

    int list[8] = {PHP_START,KW,BR_O,KW,ASSIG,INT,BR_C,SEMICOLON};

    for(int i = 0;i <= 7;i++){
        token = getToken(scanner);

        if(!compare_token(token,(TokenType)list[i])){
            return -1;
        }
        if (token.type == KW){
            if(!(compare_token_attr(token,KW,"declare") || compare_token_attr(token,KW,"strict_types"))){
                return -1;
            }
        }

    }
    return PARSE_SUCCESS;
}



int parse_if(scanner_t *sc){
    printf("if\n");
    return 0;
}
int parse_while(scanner_t *sc){
    printf("while\n");
    return 0;
}
int parse_function(scanner_t *sc){
    printf("function\n");
    return 0;
}

int parse_var_ID(scanner_t *sc){
    printf("var_ID\n");
    return 0;
}

int parse_fun_ID(scanner_t *sc){
    printf("fun_ID\n");
    return 0;
}



