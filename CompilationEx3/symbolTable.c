#include "symbolTable.h"
#include <stdlib.h>


struct symbolTable* symbolTable_getParent(struct symbolTable* symbolTable) {
	return symbolTable->parentSymbolTable;
}

struct symbolTable* symbolTable_addChild(struct symbolTable* symbolTable) {
	struct symbolTable *childSymbolTable = (struct symbolTable*)malloc(sizeof(struct symbolTable));
	childSymbolTable->parentSymbolTable = symbolTable;
	childSymbolTable->symbolListHead = NULL;
	return childSymbolTable;
}

// check this function
struct symbolTable* symbolTable_deleteChild(struct symbolTable* symbolTable) {

	struct symbolTable *parentSymbolTable = symbolTable_getParent(symbolTable);

	// free the symbol list
	symbolList_freeList(symbolTable->symbolListHead);
	free(symbolTable);

	return parentSymbolTable;
}

struct symbol* find(struct symbolTable *symbolTable, char *id) {
	return symbolList_getSymbol(symbolTable->symbolListHead, id);
}

int symbolTable_insertSymbol(struct symbolTable *symbolTable, struct symbol symbol) {
	struct symbolList* head_old = symbolTable->symbolListHead;
	symbolTable->symbolListHead = symbolList_insertEntry(symbolTable->symbolListHead, symbol);

	if (head_old == symbolTable->symbolListHead)
	{
		return 0;
	}
	return 1;
}



int insert(struct symbolTable *symbolTable, char *id, enum type type,int size, enum kind kind,int num_line) {
	struct symbol symbol;
	initializeSymbol(&symbol, id, type,size, kind,num_line);
	return symbolTable_insertSymbol(symbolTable, symbol);
}

struct symbol* lookup(struct symbolTable *symbolTable, char *id) {
	struct symbolTable *symbolTableIter = symbolTable;
	struct symbol *symbol = find(symbolTableIter, id);
	while (symbol == NULL) {
		symbolTableIter = symbolTable_getParent(symbolTableIter);
		if (symbolTableIter == NULL) {
			break;
		}
		symbol = find(symbolTableIter, id);
	}
	if (symbol != NULL)
	{
		symbol->used = USED;
	}
	return symbol;
}

struct symbolTable* enter_scope(struct symbolTable* symbolTable) {
	return symbolTable_addChild(symbolTable);
}

struct symbolTable* exit_scope(struct symbolTable* symbolTable) {

	//semantic - check if exsist id that not used print warning
	struct symbolList* head = symbolTable->symbolListHead;
	while (head != NULL)
	{
		if (head->symbol.used == NOT_USED)
		{
			fprintf(outSemantic,"WARNING at line: %d the id with lexme: %s not used \n", head->symbol.num_line_decler, head->symbol.id);
		}
		head = head->nextEntry;
	}


	return symbolTable_deleteChild(symbolTable);
}
