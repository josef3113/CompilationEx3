#include "symbol.h"
#include <stdlib.h>
#include <string.h>


void symbol_initializeSymbol(Symbol *symbol, char *id, Type type,int size, Kind kind,int num_of_line)
{
	symbol->id = id;
	symbol->type = type;
	symbol->size_arry_or_num_parameters = size;
	symbol->kind = kind;
	symbol->num_line_decler = num_of_line;
	symbol->used = NOT_USED;
	symbol->list_of_parameters = NULL;
}


int symbol_symbolCheckId(Symbol symbol, char *id) {
	if (strcmp(symbol.id, id) == 0) {
		return 0;
	}
	return 1;
}
