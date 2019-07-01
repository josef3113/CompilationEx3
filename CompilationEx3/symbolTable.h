#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "symbolList.h"

struct symbolTable {
	struct symbolList *symbolListHead;
	struct symbolTable *parentSymbolTable;
};

struct symbolTable* symbolTable_getParent(struct symbolTable* symbolTable);
struct symbolTable* symbolTable_addChild(struct symbolTable* symbolTable);
struct symbolTable* symbolTable_deleteChild(struct symbolTable* symbolTable);
int symbolTable_insertSymbol(struct symbolTable *symbolTable, struct symbol symbol);

int insert(struct symbolTable *symbolTable, char *id, enum type type,int size, enum kind kind, int num_line);
int insert_Function(struct symbolTable *symbolTable, char *id, enum type type, int size, enum kind kind, int num_line, struct symbolList* list_param);
struct symbol* find(struct symbolTable *symbolTable, char *id);
struct symbol* lookup(struct symbolTable *symbolTable, char *id);

struct symbolTable* make_table(struct symbolTable* symbolTable);
struct symbolTable* pop_table(struct symbolTable* symbolTable);
enum type get_type(Symbol* entry);
void set_type(Symbol* entry, enum type type);
#endif
