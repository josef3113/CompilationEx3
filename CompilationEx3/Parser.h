#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"


void parse_PROGRAM();
int parse_VAR_DEFINITIONS();
int parse_VAR_DEFINITIONS_t(); // is A'
int parse_VAR_DEFINITION();
enum type  parse_TYPE();
int parse_VARIABLES_LIST(enum action, enum type);
int parse_VARIABLES_LIST_t(enum action, enum type);
void parse_VARIABLE(enum action, enum type);
int parse_VARIABLE_t(); // is B'
void parse_FUNC_DEFINITIONS();
void parse_FUNC_DEFINITIONS_t();
void parse_FUNC_DEFINITION();
enum Type parse_RETURNED_TYPE();
int parse_PARAM_DEFINITIONS();
enum Type parse_STATEMENTS();
enum Type parse_STATEMENTS_t(); // is C'
enum Type parse_STATEMENT();
enum Type parse_STATEMENT_t(); // is D'
void parse_STATEMENT_t2(struct symbol * ); // is E'
enum Type parse_BLOCK();
int parse_PARAMETERS_LIST();
enum Type parse_EXPRESSION();
enum Type parse_EXPRESSION_t(struct symbol*); // is F'

// semantic
enum Type check_Use(struct symbol * , int, Token*);
struct symbolList* get_List_Parameters(struct symbolTable *);