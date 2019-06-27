#include "symbolList.h"
#include <stdlib.h>
#include <string.h>


void symbolList_freeList(struct symbolList *head) {
	if (head == NULL) {
		return;
	}
	struct symbolList *next;
	next = head->nextEntry;
	while (head != NULL) {
		//free(&(head->symbol));
		next = head->nextEntry;
		// semantic todo
		//free(head->symbol.id);
		free(head);
		head = next;
	}
}

static struct symbolList* initializeEntry(struct symbol symbol) {
	struct symbolList *entry = (struct symbolList*) malloc(sizeof(struct symbolList));
	entry->symbol = symbol;
	entry->nextEntry = NULL;
	return entry;
}

struct symbolList* symbolList_findEntry(struct symbolList *head, char *id) {
	while (head != NULL && symbolCheckId(head->symbol, id)) {
		head = head->nextEntry;
	}
	return head;
}

struct symbol* symbolList_getSymbol(struct symbolList *head, char *id) {
	struct symbolList *entry = symbolList_findEntry(head, id);
	if (entry == NULL) {
		return NULL;
	}
	return &(entry->symbol);
}

struct symbolList* symbolList_insertEntry(struct symbolList *head, struct symbol symbol) {
	if (symbolList_findEntry(head, symbol.id) != NULL) 
	{
		return head;
	}
	struct symbolList *headTemp = initializeEntry(symbol);
	headTemp->nextEntry = head;
	return headTemp;
}
