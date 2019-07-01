#ifndef SYMBOLLIST_H
#define SYMBOLLIST_H

#include "symbol.h"

typedef struct symbolList {
	Symbol symbol;
	struct symbolList *nextEntry;
}SymbolList;

void symbolList_freeList(struct symbolList *head);
struct symbolList* symbolList_findEntry(struct symbolList *head, char *id);
struct symbol* symbolList_getSymbol(struct symbolList *head, char *id);
struct symbolList* symbolList_insertEntry(struct symbolList *head, Symbol symbol);

#endif
