#ifndef SYMBOLLIST_H
#define SYMBOLLIST_H

#include "symbol.h"

struct symbolList {
	struct symbol symbol;
	struct symbolList *nextEntry;
};

void symbolList_freeList(struct symbolList *head);
struct symbolList* symbolList_findEntry(struct symbolList *head, char *id);
struct symbol* symbolList_getSymbol(struct symbolList *head, char *id);
struct symbolList* symbolList_insertEntry(struct symbolList *head, struct symbol symbol);

#endif
