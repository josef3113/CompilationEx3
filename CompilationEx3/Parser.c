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
		enum type type = parse_TYPE();
		parse_VARIABLES_LIST(type);
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

enum type parse_TYPE()
{
	Token* curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_REAL:
	{
		fprintf(outSyntactic, "Rule (TYPE -> real) \n");

		//semantic
		return REAL;
	}break;
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (TYPE -> integer) \n");

		//semantic
		return INTEGER;
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

		//semantic 
		return ERROR;
	}
	}
}

void parse_VARIABLES_LIST(enum type type)
{
	Token* curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST -> VARIABLE VARIABLES_LIST_t) \n");
		back_token();
		parse_VARIABLE(type);
		parse_VARIABLES_LIST_t(type);
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

void parse_VARIABLES_LIST_t(enum type type)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_COMMA:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST_t -> ,VARIABLE VARIABLES_LIST_t) \n");
		parse_VARIABLE(type);
		parse_VARIABLES_LIST_t(type);
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

void parse_VARIABLE(enum type type)
{
	Token*	curr_token = next_token();
	int size_of_var;

	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (VARIABLE -> id VARIABLE_t) \n");
		size_of_var = parse_VARIABLE_t();
		
		// semantic                          
		insert(symbolTable, curr_token->lexeme, type, size_of_var, variable);
		printf("lexme:%s type:%d  size:%d \n", curr_token->lexeme, type,size_of_var);

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

int parse_VARIABLE_t()
{
	Token*	curr_token = next_token();
	Token*  token_for_size = NULL;

	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VARIABLE_t -> [int_number] ) \n");

		//semantic
		token_for_size = next_token();
		back_token();

		match(TOKEN_INT_NUBMER);
		match(TOKEN_SEP_R_BRACKET);

		// semantic
		return atoi(token_for_size->lexeme);
	}break;

	//follow
	case TOKEN_SEP_COMMA:
	case TOKEN_SEP_SEMICOLON:
	case TOKEN_OP_ASSIGN:
	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VARIABLE_t -> EPSILON ) \n");
		back_token();

		//semantic
		return -1;
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

		//semantic todo
		return -2;
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

		// semantic
		struct symbol* entry_of_id = lookup(symbolTable, curr_token->lexeme);
		parse_STATEMENT_t2(entry_of_id);
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

void parse_STATEMENT_t2(symbol* entry_of_id)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	case TOKEN_OP_ASSIGN:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t2 -> VARIABLE_t = EXPRESSION ) \n");

		//semantic 
		if (entry_of_id != NULL) {
			int size_of_id = entry_of_id->size;
			int size_of_VARIABLE_t;

			back_token();
			size_of_VARIABLE_t = parse_VARIABLE_t();

			//semantic 
			if (size_of_id > -1 && size_of_VARIABLE_t == -1)
			{
				fprintf(outSemantic, "ERROR at line:%d - can't to assign to array \n", curr_token->lineNumber);
				//return ERROR;
			}
			else if (size_of_id == -1 && size_of_VARIABLE_t != -1)
			{
				fprintf(outSemantic, "ERROR at line:%d - you try use in val like array \n", curr_token->lineNumber);
			}
			else if (size_of_id > -1 && size_of_VARIABLE_t > size_of_id)
			{
				fprintf(outSemantic, "ERROR at line:%d - out of bound array \n", curr_token->lineNumber);
			}
		}
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
		parse_VARIABLES_LIST(ERROR); // semantic need to change this !-!-!
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