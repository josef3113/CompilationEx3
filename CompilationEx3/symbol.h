#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>
#include "Token.h"
// <id,type,kind>

typedef enum type { VOID,INTEGER, REAL ,ERROR}Type;
typedef enum action {TO_USE,TO_DEFINE}Action;
enum kind { variable, function, parameter };

typedef struct symbol {
	const char *id;
	enum type type;
	int size; // size or num parameters
	enum kind kind;
}symbol;

void initializeSymbol(struct symbol *symbol, const char *id, enum type type,int size, enum kind kind);
//void freeSymbol(struct symbol *symbol);
const char* typeToString(enum type type);
const char* kindToString(enum kind kind);
void printSymbol(struct symbol *symbol);
int symbolcmp(struct symbol symbol1, struct symbol symbol2);
int symbolcmp_id(struct symbol symbol1, struct symbol symbol2);
int symbolCheckId(struct symbol symbol, const char *id);

#endif
