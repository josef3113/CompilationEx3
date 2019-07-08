%{
#include"stdio.h"
#include"Token.h"
int line=1;
FILE *outSyntactic = NULL;
%}

DIGIT    [0-9]
ALPHA	 [a-zA-Z]

ID   {ALPHA}(("_"({ALPHA}|{DIGIT}))|({ALPHA}|{DIGIT}))*
INT  [1-9]{DIGIT}*|"0"
REAL {INT}"."{DIGIT}+
COMMENT "--".*

%%
"program" 		{create_and_store_token(TOKEN_KEY_PROGRAM,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_KEY_PROGRAM}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"end" 			{create_and_store_token(TOKEN_KEY_END,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_KEY_END}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"real" 			{create_and_store_token(TOKEN_KEY_REAL,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_KEY_REAL}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"integer"		{create_and_store_token(TOKEN_KEY_INTEGER,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_KEY_INTEGER}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"void"			{create_and_store_token(TOKEN_KEY_VOID,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_KEY_VOID}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"return"		{create_and_store_token(TOKEN_KEY_RETURN,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_KEY_RETURN}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}

"*"				{create_and_store_token(TOKEN_OP_MUL,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_OP_MUL}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"/"				{create_and_store_token(TOKEN_OP_DIV,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_OP_DIV}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"="				{create_and_store_token(TOKEN_OP_ASSIGN,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_OP_ASSIGN}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}

","				{create_and_store_token(TOKEN_SEP_COMMA,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_COMMA}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
";"				{create_and_store_token(TOKEN_SEP_SEMICOLON,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_SEMICOLON}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"["				{create_and_store_token(TOKEN_SEP_L_BRACKET,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_L_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"]"				{create_and_store_token(TOKEN_SEP_R_BRACKET,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_R_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"{"				{create_and_store_token(TOKEN_SEP_L_CURLY_BRACKET,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_L_CURLY_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"}"				{create_and_store_token(TOKEN_SEP_R_CURLY_BRACKET,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_R_CURLY_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
"("				{create_and_store_token(TOKEN_SEP_L_ROUND_BRACKET,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_L_ROUND_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
")"				{create_and_store_token(TOKEN_SEP_R_ROUND_BRACKET,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_SEP_R_ROUND_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}

{COMMENT}       {}

{ID}            {create_and_store_token(TOKEN_ID,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_ID}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
{INT}           {create_and_store_token(TOKEN_INT_NUBMER,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_INT_NUBMER}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}
{REAL}          {create_and_store_token(TOKEN_REAL_NUMBER,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_REAL_NUMBER}' was found at line: {%d}, lexeme: '{%s}'.\n", line, yytext);return 1;}

<<EOF>>			{create_and_store_token(TOKEN_EOF,yytext,line);fprintf(yyout, "Token of kind '{TOKEN_EOF}' was found at line: {%d}.\n", line);return 1;}

\n				{line++;}
\t 				{}
\r 				{}
" " 			{}

.       		{fprintf(yyout,"The character '{%s}' at line: {%d} does not begin any legal token in the language.\n",yytext,line);}


%%

int main(int argc, char *argv[] )
{
	
	yyin = fopen("C:\\temp\\test1.txt", "r");
	yyout = fopen("C:\\temp\\test1_311763718_311303333_lex.txt", "w");
	outSyntactic = fopen("C:\\temp\\test1_311763718_311303333_syntactic.txt", "w");

	parse_PROGRAM();
	match(TOKEN_EOF);

	fclose(yyin);
	fclose(yyout);
	fclose(outSyntactic);
	free_storege();

	yyin = fopen("C:\\temp\\test2.txt", "r");
	yyrestart(yyin);
	line = 1;

	yyout = fopen("C:\\temp\\test2_311763718_311303333_lex.txt", "w");
	outSyntactic = fopen("C:\\temp\\test2_311763718_311303333_syntactic.txt", "w");

	parse_PROGRAM();
	match(TOKEN_EOF);

	fclose(yyin);
	fclose(yyout);
	fclose(outSyntactic);
	free_storege();

	yylex_destroy();
}