#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"


void parse_PROGRAM();
void parse_VAR_DEFINITIONS();
void parse_VAR_DEFINITIONS_t(); // is A'
void parse_VAR_DEFINITION();
void parse_TYPE();
void parse_VARIABLES_LIST();
void parse_VARIABLES_LIST_t();
void parse_VARIABLE();
void parse_VARIABLE_t(); // is B'
void parse_FUNC_DEFINITIONS();
void parse_FUNC_DEFINITIONS_t();
void parse_FUNC_DEFINITION();
void parse_RETURNED_TYPE();
void parse_PARAM_DEFINITIONS();
void parse_STATEMENTS();
void parse_STATEMENTS_t(); // is C'
void parse_STATEMENT();
void parse_STATEMENT_t(); // is D'
void parse_STATEMENT_t2(); // is E'
void parse_BLOCK();
void parse_PARAMETERS_LIST();
void parse_EXPRESSION();
void parse_EXPRESSION_t(); // is F'