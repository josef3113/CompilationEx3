#include "symbolList.h"
#include <stdlib.h>
#include <string.h>


void symbolList_freeList(struct symbolList *head) {
	if (head == NULL) {
		return;
	}
	SymbolList *next;
	next = head->nextEntry;
	while (head != NULL) {
		//free(&(head->symbol));
		next = head->nextEntry;
		// semantic todo
		//free(head->symbol.id);

		// free the list of parametrs of all symbol in symbol table
		symbolList_freeList(head->symbol.list_of_parameters);
		free(head);
		head = next;
	}
}

SymbolList* initializeEntry(Symbol symbol) {
	SymbolList *entry = (SymbolList*) malloc(sizeof(SymbolList));
	entry->symbol = symbol;
	entry->nextEntry = NULL;
	return entry;
}

SymbolList* symbolList_findEntry(struct symbolList *head, char *id) {
	while (head != NULL && symbolCheckId(head->symbol, id)) {
		head = head->nextEntry;
	}
	return head;
}

struct symbol* symbolList_getSymbol(struct symbolList *head, char *id) {
	SymbolList *entry = symbolList_findEntry(head, id);
	if (entry == NULL) {
		return NULL;
	}
	return &(entry->symbol);
}

SymbolList* symbolList_insertEntry(struct symbolList *head, Symbol symbol) {
	if (symbolList_findEntry(head, symbol.id) != NULL) 
	{
		return head;
	}
	SymbolList *headTemp = initializeEntry(symbol);
	headTemp->nextEntry = head;
	return headTemp;
}
