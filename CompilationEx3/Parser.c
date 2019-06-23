#include "Parser.h"
#include "symbol.h"
#include "symbolList.h"
#include "symbolTable.h"

struct symbolTable *symbolTable;

void parse_PROGRAM()
{
	// semantic 
	symbolTable = (struct symbolTable*)malloc(sizeof(struct symbolTable));
	symbolTable->symbolListHead = NULL;
	symbolTable->parentSymbolTable = NULL;
	symbolTable->childSymbolTableListHead = NULL;


	Token * curr_token = next_token();
	// first program
	switch (curr_token->kind)
	{
	case TOKEN_KEY_PROGRAM:
	{
		fprintf(outSyntactic, "Rule (PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS) \n");
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEP_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_KEY_END);
		parse_FUNC_DEFINITIONS();

	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_PROGRAM' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);
		while (curr_token->kind != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		back_token();
	}

	}

}


void parse_VAR_DEFINITIONS()
{
	Token*	curr_token = next_token();

	switch (curr_token->kind)
	{

	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_t) \n");
		back_token();
		parse_VAR_DEFINITION();
		parse_VAR_DEFINITIONS_t();

	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);
		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_VAR_DEFINITIONS_t()
{
	Token*	curr_token = next_token();

	switch (curr_token->kind)
	{

	case TOKEN_SEP_SEMICOLON:
	{
		curr_token = next_token();
		back_token();
		if (curr_token->kind == TOKEN_KEY_REAL || curr_token->kind == TOKEN_KEY_INTEGER)
		{
			fprintf(outSyntactic, "Rule (VAR_DEFINITIONS_t -> ; VAR_DEFINITIONS) \n");
			parse_VAR_DEFINITIONS();
		}
		else
		{
			fprintf(outSyntactic, "Rule (VAR_DEFINITIONS_t -> EPSILON) \n");
			back_token();
		}
	}break;

	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VAR_DEFINITIONS_t -> EPSILON) \n");
		back_token();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_SEP_SEMICOLON' , 'TOKEN_SEP_R_ROUND_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);
		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_VAR_DEFINITION()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (VAR_DEFINITION -> TYPE VARIABLES_LIST) \n");

		back_token();
		parse_TYPE();
		parse_VARIABLES_LIST();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_TYPE()
{
	Token* curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_REAL:
	{
		fprintf(outSyntactic, "Rule (TYPE -> real) \n");
	}break;
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (TYPE -> integer) \n");
	}break;
	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_ID)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_VARIABLES_LIST()
{
	Token* curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST -> VARIABLE VARIABLES_LIST_t) \n");
		back_token();
		parse_VARIABLE();
		parse_VARIABLES_LIST_t();
	}break;
	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_ID' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_VARIABLES_LIST_t()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_COMMA:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST_t -> ,VARIABLE VARIABLES_LIST_t) \n");
		parse_VARIABLE();
		parse_VARIABLES_LIST_t();
	}break;

	//follow
	case TOKEN_SEP_SEMICOLON:
	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST_t -> EPSILON ) \n");
		back_token();
	}break;
	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_SEP_COMMA' , 'TOKEN_SEP_SEMICOLON' , 'TOKEN_SEP_R_ROUND_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_VARIABLE()
{
	Token*	curr_token = next_token();

	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (VARIABLE -> id VARIABLE_t) \n");
		parse_VARIABLE_t();
		
		// semantic 
		struct symbol symbol;
		initializeSymbol(&symbol, curr_token->lexeme, INT, variable);
		symbolTable_insertSymbol(symbolTable, symbol);

	}break;
	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_ID' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET && curr_token->kind != TOKEN_SEP_COMMA)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_VARIABLE_t()
{
	Token*	curr_token = next_token();

	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VARIABLE_t -> [int_number] ) \n");
		match(TOKEN_INT_NUBMER);
		match(TOKEN_SEP_R_BRACKET);
	}break;

	//follow
	case TOKEN_SEP_COMMA:
	case TOKEN_SEP_SEMICOLON:
	case TOKEN_OP_ASSIGN:
	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VARIABLE_t -> EPSILON ) \n");
		back_token();
	}break;
	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_SEP_L_BRACKET' , 'TOKEN_SEP_COMMA' , 'TOKEN_SEP_SEMICOLON' , 'TOKEN_OP_ASSIGN' , 'TOKEN_SEP_R_ROUND_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET && curr_token->kind != TOKEN_SEP_COMMA && curr_token->kind != TOKEN_OP_ASSIGN)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_FUNC_DEFINITIONS() {

	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_VOID:
	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (FUNC_DEFINITIONS -> FUNC_DEFINITION FUNC_DEFINITIONS_t) \n");
		back_token();
		parse_FUNC_DEFINITION();
		parse_FUNC_DEFINITIONS_t();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_VOID' , 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_FUNC_DEFINITIONS_t()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
		// var is nullable
		// first cases:
	case TOKEN_KEY_VOID:
	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (FUNC_DEFINITIONS_t -> FUNC_DEFINITION FUNC_DEFINITIONS_t) \n");
		back_token();
		parse_FUNC_DEFINITION();
		parse_FUNC_DEFINITIONS_t();
	}break;

	//follow cases:
	case TOKEN_EOF:
	{
		fprintf(outSyntactic, "Rule (FUNC_DEFINITIONS_t -> EPSILON ) \n");
		back_token();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_VOID' , 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' , 'TOKEN_EOF' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_FUNC_DEFINITION()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_VOID:
	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK ) \n");

		back_token();
		parse_RETURNED_TYPE();
		match(TOKEN_ID);
		match(TOKEN_SEP_L_ROUND_BRACKET);
		parse_PARAM_DEFINITIONS();
		match(TOKEN_SEP_R_ROUND_BRACKET);
		parse_BLOCK();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_VOID' , 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_KEY_VOID && curr_token->kind != TOKEN_KEY_REAL && curr_token->kind != TOKEN_KEY_INTEGER)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_RETURNED_TYPE()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_VOID:
	{
		fprintf(outSyntactic, "Rule (RETURNED_TYPE -> void ) \n");
	}break;
	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (RETURNED_TYPE -> TYPE ) \n");

		back_token();
		parse_TYPE();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_VOID' , 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_ID)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_PARAM_DEFINITIONS()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
		// first
	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (PARAM_DEFINITIONS -> VAR_DEFINITIONS ) \n");

		back_token();
		parse_VAR_DEFINITIONS();
	}break;

	// follow
	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (PARAM_DEFINITIONS -> EPSILON ) \n");

		back_token();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_REAL' , 'TOKEN_KEY_INTEGER' , 'TOKEN_SEP_R_ROUND_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_STATEMENTS()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_RETURN:
	case TOKEN_SEP_L_CURLY_BRACKET:
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (STATEMENTS -> STATEMENT;STATEMENTS_t ) \n");

		back_token();
		parse_STATEMENT();
		match(TOKEN_SEP_SEMICOLON);
		parse_STATEMENTS_t();
	}break;


	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_RETURN' , 'TOKEN_SEP_L_CURLY_BRACKET' , 'TOKEN_ID' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_KEY_END && curr_token->kind != TOKEN_SEP_R_CURLY_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_STATEMENTS_t()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_RETURN:
	case TOKEN_SEP_L_CURLY_BRACKET:
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (STATEMENTS_t -> STATEMENTS ) \n");

		back_token();
		parse_STATEMENTS();
	}break;

	case TOKEN_KEY_END:
	case TOKEN_SEP_R_CURLY_BRACKET:
	{
		fprintf(outSyntactic, "Rule (STATEMENTS_t -> EPSILON ) \n");

		back_token();
	}break;


	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_RETURN' , 'TOKEN_SEP_L_CURLY_BRACKET' , 'TOKEN_ID' , 'TOKEN_KEY_END' , 'TOKEN_SEP_R_CURLY_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_KEY_END && curr_token->kind != TOKEN_SEP_R_CURLY_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_STATEMENT()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_RETURN:
	{
		fprintf(outSyntactic, "Rule (STATEMENT -> return STATEMENT_t ) \n");

		parse_STATEMENT_t();
	}break;

	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (STATEMENT -> id STATEMENT_t2 ) \n");

		parse_STATEMENT_t2();
	}break;

	case TOKEN_SEP_L_CURLY_BRACKET:
	{
		fprintf(outSyntactic, "Rule (STATEMENT -> BLOCK ) \n");

		back_token();

		// semantic 
		symbolTable = enter_scope(symbolTable);

		parse_BLOCK();

		// semantic 
		symbolTable = exit_scope(symbolTable);

	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_KEY_RETURN' , 'TOKEN_ID' , 'TOKEN_SEP_L_CURLY_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_STATEMENT_t()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	case TOKEN_INT_NUBMER:
	case TOKEN_REAL_NUMBER:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t -> EXPRESSION ) \n");

		back_token();
		parse_EXPRESSION();
	}break;

	case TOKEN_SEP_SEMICOLON:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t -> EPSILON ) \n");

		back_token();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_ID' , 'TOKEN_INT_NUBMER' , 'TOKEN_REAL_NUMBER' , 'TOKEN_SEP_SEMICOLON' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_STATEMENT_t2()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	case TOKEN_OP_ASSIGN:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t2 -> VARIABLE_t = EXPRESSION ) \n");

		back_token();
		parse_VARIABLE_t();
		match(TOKEN_OP_ASSIGN);
		parse_EXPRESSION();
	}break;

	case TOKEN_SEP_L_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t2 -> ( PARAMETERS_LIST ) ) \n");

		parse_PARAMETERS_LIST();
		match(TOKEN_SEP_R_ROUND_BRACKET);
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_SEP_L_BRACKET' , 'TOKEN_OP_ASSIGN' , 'TOKEN_SEP_L_ROUND_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}


void parse_BLOCK()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_CURLY_BRACKET:
	{
		fprintf(outSyntactic, "Rule (BLOCK -> { VAR_DEFINITIONS;STATEMENTS } ) \n");

		parse_VAR_DEFINITIONS();
		match(TOKEN_SEP_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_SEP_R_CURLY_BRACKET);
	}break;
	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_SEP_L_CURLY_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_KEY_VOID && curr_token->kind != TOKEN_KEY_REAL && curr_token->kind != TOKEN_KEY_INTEGER && curr_token->kind != TOKEN_SEP_SEMICOLON)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}


void parse_PARAMETERS_LIST()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (PARAMETERS_LIST -> VARIABLES_LIST ) \n");

		back_token();
		parse_VARIABLES_LIST();
	}break;

	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (PARAMETERS_LIST -> EPSILON ) \n");

		back_token();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_ID' , 'TOKEN_SEP_R_ROUND_BRACKET' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_R_ROUND_BRACKET)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_EXPRESSION()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION -> id EXPRESSION_t ) \n");

		parse_EXPRESSION_t();
	}break;

	case TOKEN_REAL_NUMBER:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION -> real_number ) \n");
	}break;
	case TOKEN_INT_NUBMER:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION -> int_number ) \n");
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_ID' , 'TOKEN_REAL_NUMBER' , 'TOKEN_INT_NUBMER' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_EXPRESSION_t()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> VARIABLE_t ) \n");

		back_token();
		parse_VARIABLE_t();
	}break;

	case TOKEN_OP_MUL:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> *EXPRESSION ) \n");

		parse_EXPRESSION();
	}break;
	case TOKEN_OP_DIV:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> /EXPRESSION ) \n");

		parse_EXPRESSION();
	}break;

	case TOKEN_SEP_SEMICOLON:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> EPSILON ) \n");

		back_token();
	}break;

	default:
	{
		fprintf(outSyntactic, "Expected: one of tokens 'TOKEN_SEP_L_BRACKET' , 'TOKEN_OP_MUL' , 'TOKEN_OP_DIV' , 'TOKEN_SEP_SEMICOLON' at line %d ", curr_token->lineNumber);
		fprintf(outSyntactic, "Actual token: '%s' , lexeme “ %s ” \n", ENUM_STRING[curr_token->kind], curr_token->lexeme);

		while (curr_token->kind != TOKEN_EOF && curr_token->kind != TOKEN_SEP_SEMICOLON)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}