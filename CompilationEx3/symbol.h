#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include "Token.h"

typedef enum type { VOID, INTEGER, REAL, EMPTY, ERROR}Type;
typedef enum action { TO_USE, TO_DEFINE}Action;
typedef enum kind { VARIABLE, FUNCTION }Kind;
typedef enum use { NOT_USED, USED}Use;

typedef struct symbol {
	char* id;
	enum type type;
	int size_arry_or_num_parameters;
	enum kind kind;
	enum use used;
	int num_line_decler;
}Symbol;

void initializeSymbol(struct symbol *symbol, char *id, enum type type,int size, enum kind kind,int num_of_line );
int symbolCheckId(struct symbol symbol, char *id);

#endif
