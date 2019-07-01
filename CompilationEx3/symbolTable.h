#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "symbolList.h"

typedef struct symbolTable {
	 struct symbolList *symbolListHead;
	struct symbolTable *parentSymbolTable;
}SymbolTable;

struct symbolTable* symbolTable_getParent(struct symbolTable* symbolTable);
struct symbolTable* symbolTable_addChild(struct symbolTable* symbolTable);
struct symbolTable* symbolTable_deleteChild(struct symbolTable* symbolTable);
int symbolTable_insertSymbol(struct symbolTable *symbolTable, Symbol symbol);

int insert(struct symbolTable *symbolTable, char *id, Type type,int size, Kind kind, int num_line);
int insert_Function(struct symbolTable *symbolTable, char *id, Type type, int size, Kind kind, int num_line, struct symbolList* list_param);
Symbol* find(struct symbolTable *symbolTable, char *id);
Symbol* lookup(struct symbolTable *symbolTable, char *id);

struct symbolTable* make_table(struct symbolTable* symbolTable);
struct symbolTable* pop_table(struct symbolTable* symbolTable);
Type get_type(Symbol* entry);
void set_type(Symbol* entry, Type type);
#endif
