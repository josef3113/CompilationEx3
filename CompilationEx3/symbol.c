#include "symbol.h"
#include <stdlib.h>
#include <string.h>


void initializeSymbol(struct symbol *symbol, char *id, enum type type,int size, enum kind kind,int num_of_line)
{
	symbol->id = id;
	symbol->type = type;
	symbol->size_arry_or_num_parameters = size;
	symbol->kind = kind;
	symbol->num_line_decler = num_of_line;
	symbol->used = NOT_USED;
	symbol->list_of_parameters = NULL;
}


int symbolCheckId(struct symbol symbol, char *id) {
	if (strcmp(symbol.id, id) == 0) {
		return 0;
	}
	return 1;
}
