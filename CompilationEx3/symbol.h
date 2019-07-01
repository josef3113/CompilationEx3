#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include "Token.h"

typedef enum type { VOID, INTEGER, REAL, EMPTY, ERROR}Type;
typedef enum action { TO_USE, TO_DEFINE}Action;
typedef enum kind { VARIABLE, FUNCTION, ARRAY }Kind;
typedef enum use { NOT_USED, USED}Use;

typedef struct symbol {
	char* id;
	Type type;
	int size_arry_or_num_parameters;
	Kind kind;
	Use used;
	int num_line_decler;
	struct symbolList* list_of_parameters;
}Symbol;


void initializeSymbol(Symbol *symbol, char *id, Type type,int size, Kind kind,int num_of_line );
int symbolCheckId(Symbol symbol, char *id);

#endif
