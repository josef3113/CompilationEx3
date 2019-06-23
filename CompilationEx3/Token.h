#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern FILE *yyin, *yyout, *outSyntactic, *outSemantic;

typedef enum eTOKENS
{
	TOKEN_KEY_PROGRAM,
	TOKEN_KEY_END,
	TOKEN_KEY_REAL,
	TOKEN_KEY_INTEGER,
	TOKEN_KEY_VOID,
	TOKEN_KEY_RETURN,
	TOKEN_OP_MUL,
	TOKEN_OP_DIV,
	TOKEN_OP_ASSIGN,
	TOKEN_SEP_COMMA,
	TOKEN_SEP_SEMICOLON,
	TOKEN_SEP_L_BRACKET,
	TOKEN_SEP_R_BRACKET,
	TOKEN_SEP_L_CURLY_BRACKET,
	TOKEN_SEP_R_CURLY_BRACKET,
	TOKEN_SEP_L_ROUND_BRACKET,
	TOKEN_SEP_R_ROUND_BRACKET,
	TOKEN_ID,
	TOKEN_INT_NUBMER,
	TOKEN_REAL_NUMBER,
	TOKEN_EOF
}eTOKENS;

static const char *ENUM_STRING[] =
{ "TOKEN_KEY_PROGRAM",
"TOKEN_KEY_END",
"TOKEN_KEY_REAL",
"TOKEN_KEY_INTEGER",
"TOKEN_KEY_VOID",
"TOKEN_KEY_RETURN",
"TOKEN_OP_MUL",
"TOKEN_OP_DIV",
"TOKEN_OP_ASSIGN",
"TOKEN_SEP_COMMA",
"TOKEN_SEP_SEMICOLON",
"TOKEN_SEP_L_BRACKET",
"TOKEN_SEP_R_BRACKET",
"TOKEN_SEP_L_CURLY_BRACKET",
"TOKEN_SEP_R_CURLY_BRACKET",
"TOKEN_SEP_L_ROUND_BRACKET",
"TOKEN_SEP_R_ROUND_BRACKET",
"TOKEN_ID",
"TOKEN_INT_NUBMER",
"TOKEN_REAL_NUMBER",
"TOKEN_EOF"
};

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token *next_token();
Token *back_token();
int match(eTOKENS token);
void free_storege();

#endif