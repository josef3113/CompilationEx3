#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"


void parse_PROGRAM();
int parse_VAR_DEFINITIONS();
int parse_VAR_DEFINITIONS_t(); 
int parse_VAR_DEFINITION();
enum type  parse_TYPE();
int parse_VARIABLES_LIST(enum action , enum type);
int parse_VARIABLES_LIST_t(enum action, enum type);
void parse_VARIABLE(enum action, enum type);
int parse_VARIABLE_t(); 
void parse_FUNC_DEFINITIONS();
void parse_FUNC_DEFINITIONS_t();
void parse_FUNC_DEFINITION();
enum type parse_RETURNED_TYPE();
int parse_PARAM_DEFINITIONS();
enum type parse_STATEMENTS(enum type);
enum type parse_STATEMENTS_t(enum type); 
enum type parse_STATEMENT(enum type);
enum type parse_STATEMENT_t(); 
void parse_STATEMENT_t2(struct symbol *); 
enum type parse_BLOCK(enum type);
int parse_PARAMETERS_LIST();
enum type parse_EXPRESSION();
enum type parse_EXPRESSION_t(struct symbol*); 

// semantic
enum type check_Use(struct symbol *, int, Token*);
struct symbolList* get_List_Parameters(struct symbolTable *);