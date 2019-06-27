#include "Parser.h"
#include "symbol.h"
#include "symbolList.h"
#include "symbolTable.h"

struct symbolTable *symbolTable;

void parse_PROGRAM()
{
	// semantic 
	// global table - contain scoop main and function
	symbolTable = (struct symbolTable*)malloc(sizeof(struct symbolTable));
	symbolTable->symbolListHead = NULL;
	symbolTable->parentSymbolTable = NULL;


	Token * curr_token = next_token();
	// first program
	switch (curr_token->kind)
	{
	case TOKEN_KEY_PROGRAM:
	{
		//semantic 
		// main scoop insert 
		symbolTable = enter_scope(symbolTable);

		fprintf(outSyntactic, "Rule (PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS) \n");
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEP_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_KEY_END);

		//semantic 
		// main scoop exit
		symbolTable = exit_scope(symbolTable);


		parse_FUNC_DEFINITIONS();

		// go out from global table
		exit_scope(symbolTable);

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


int parse_VAR_DEFINITIONS()
{
	Token*	curr_token = next_token();

	switch (curr_token->kind)
	{

	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		// semantic 
		int num_of_VAR_DEFINITION, num_of_VAR_DEFINITIONS_t;

		fprintf(outSyntactic, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_t) \n");
		back_token();
		num_of_VAR_DEFINITION = parse_VAR_DEFINITION();
		num_of_VAR_DEFINITIONS_t = parse_VAR_DEFINITIONS_t();

		//semantic
		return num_of_VAR_DEFINITION + num_of_VAR_DEFINITIONS_t;

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

		// semantic todo
		return -1;
	}
	}
}

int parse_VAR_DEFINITIONS_t()
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
			return parse_VAR_DEFINITIONS();
		}
		else
		{
			fprintf(outSyntactic, "Rule (VAR_DEFINITIONS_t -> EPSILON) \n");
			back_token();

			//semantic 
			return 0;
		}
	}break;

	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VAR_DEFINITIONS_t -> EPSILON) \n");
		back_token();

		//semantic 
		return 0;
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

		//semantic todo
		return -1;
	}
	}
}

int parse_VAR_DEFINITION()
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
		return parse_VARIABLES_LIST(TO_DEFINE, type);
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

		//semantic todo 
		return -1;
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

int parse_VARIABLES_LIST(enum action action,enum type type)
{
	Token* curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		//semantic 
		int num_of_VARIABLES_LIST_t;

		fprintf(outSyntactic, "Rule (VARIABLES_LIST -> VARIABLE VARIABLES_LIST_t) \n");
		back_token();
		parse_VARIABLE(action, type);
		num_of_VARIABLES_LIST_t = parse_VARIABLES_LIST_t(action, type);

		//semantic
		return num_of_VARIABLES_LIST_t + 1;
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

		//semantic todo
		return -1;
	}
	}
}

int parse_VARIABLES_LIST_t(enum action action,enum type type)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_COMMA:
	{
		//semantic
		int num_of_VARIABLES_LIST_t;

		fprintf(outSyntactic, "Rule (VARIABLES_LIST_t -> ,VARIABLE VARIABLES_LIST_t) \n");
		parse_VARIABLE(action, type);
		num_of_VARIABLES_LIST_t = parse_VARIABLES_LIST_t(action, type);

		//semantic
		return num_of_VARIABLES_LIST_t + 1;
	}break;

	//follow
	case TOKEN_SEP_SEMICOLON:
	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST_t -> EPSILON ) \n");
		back_token();

		//semantic
		return 0;
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

		//semantic todo
		return -1;
	}
	}
}

void parse_VARIABLE(enum action action,enum type type)
{
	Token*	curr_token = next_token();
	
	//semantic
	int size_of_var;
	int inserted;

	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (VARIABLE -> id VARIABLE_t) \n");
		size_of_var = parse_VARIABLE_t();

		if (action == TO_USE)
		{
			Symbol* entry_of_id = lookup(symbolTable, curr_token->lexeme);
			if (entry_of_id == NULL)
			{
				fprintf(outSemantic, "ERROR at line: %d the variable with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
			}
		}

		else // want to define var with name curr_token->lexeme
		{
			// semantic                          
			inserted = insert(symbolTable, curr_token->lexeme, type, size_of_var, VARIABLE, curr_token->lineNumber);
			if (!inserted)
			{
				Symbol* entry_of_variable = find(symbolTable, curr_token->lexeme);  // find this in current table
				fprintf(outSemantic, "ERROR at line: %d the variable with lexme: %s alredy define at line: %d \n",
						curr_token->lineNumber, curr_token->lexeme,entry_of_variable->num_line_decler);
			}
		}
		//printf("lexme:%s type:%d  size:%d \n", curr_token->lexeme, type,size_of_var);

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
		//semantic 
		Type type_of_RETURNED_TYPE;
		Type type_of_BLOCK;
		int num_of_PARAM_DEFINITIONS;
		int inserted;


		fprintf(outSyntactic, "Rule (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS) BLOCK ) \n");

		back_token();
		type_of_RETURNED_TYPE = parse_RETURNED_TYPE();
		
		// semantic
		Token* token_of_id = next_token();
		back_token();

		match(TOKEN_ID);
		match(TOKEN_SEP_L_ROUND_BRACKET);

		//semantic
		inserted = insert(symbolTable, token_of_id->lexeme, type_of_RETURNED_TYPE, -1 , FUNCTION, curr_token ->lineNumber);
		if (!inserted)
		{
			Symbol* entry_of_function = find(symbolTable, token_of_id->lexeme);
			fprintf(outSemantic, "ERROR at line: %d the function with lexme:%s alredy define at line: %d \n",
					token_of_id->lineNumber, token_of_id->lexeme, entry_of_function->num_line_decler);
		}

		// semantic - do this here becous i want the names of parametrs will be in list symbol of this function
		symbolTable = enter_scope(symbolTable);

		num_of_PARAM_DEFINITIONS = parse_PARAM_DEFINITIONS();

		//semantic - update the real num of parameters of function
		if (inserted)
		{
			Symbol* entry_of_function = lookup(symbolTable, token_of_id->lexeme);
			entry_of_function->used = NOT_USED; // when i do lookup the system change that i use with this
												// id but actuly i only sertch this to find line number
												// becous in first all declertion i can to chang to NOT_USE
												// becous here i still in definition
			entry_of_function->size_arry_or_num_parameters = num_of_PARAM_DEFINITIONS;
		}
		
		match(TOKEN_SEP_R_ROUND_BRACKET);
		type_of_BLOCK = parse_BLOCK();

		//semantic
		symbolTable = exit_scope(symbolTable);
		
		//semantic
		if (type_of_RETURNED_TYPE == VOID)
		{
			if (type_of_BLOCK != EMPTY && type_of_BLOCK != VOID)
			{
				fprintf(outSemantic, "ERROR at line: %d the function with lexme:%s define void but return other type\n", token_of_id->lineNumber, token_of_id->lexeme);
			}
		}
		else
		{
			if (type_of_BLOCK != type_of_RETURNED_TYPE)
			{
				fprintf(outSemantic, "ERROR at line: %d the function with lexme:%s miss match return type\n", token_of_id->lineNumber, token_of_id->lexeme);
			}
		}
	
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

Type parse_RETURNED_TYPE()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_VOID:
	{
		fprintf(outSyntactic, "Rule (RETURNED_TYPE -> void ) \n");

		//semantic
		return VOID;
	}break;
	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{
		fprintf(outSyntactic, "Rule (RETURNED_TYPE -> TYPE ) \n");

		back_token();
		Type type_of_TYPE = parse_TYPE();

		//semantic
		return type_of_TYPE;
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

		//semantic todo
		return ERROR;
	}
	}
}

int parse_PARAM_DEFINITIONS()
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
		return parse_VAR_DEFINITIONS();
	}break;

	// follow
	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (PARAM_DEFINITIONS -> EPSILON ) \n");

		back_token();

		// semantic 
		return 0;
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

		// semantic todo
		return -1;
	}
	}
}

Type parse_STATEMENTS()
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
		Type type_of_STATEMENT = parse_STATEMENT();
		match(TOKEN_SEP_SEMICOLON);
		Type type_of_STATEMENTS_t = parse_STATEMENTS_t();

		// semantic
		if (type_of_STATEMENT == type_of_STATEMENTS_t)
		{
			return type_of_STATEMENT;
		}
		else if (type_of_STATEMENT == EMPTY)
		{
			return type_of_STATEMENTS_t;
		}
		else if (type_of_STATEMENTS_t == EMPTY)
		{
			return type_of_STATEMENT;
		}
		else // two type diff from empty its iilegal
		{
			return ERROR;
		}
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

		//semantic todo
		return ERROR;
	}
	}
}

Type parse_STATEMENTS_t()
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
		return parse_STATEMENTS();
	}break;

	case TOKEN_KEY_END:
	case TOKEN_SEP_R_CURLY_BRACKET:
	{
		fprintf(outSyntactic, "Rule (STATEMENTS_t -> EPSILON ) \n");

		back_token();

		//semantic 
		return EMPTY;
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

		//semantic todo
		return ERROR;
	}
	}
}

Type parse_STATEMENT()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_RETURN:
	{
		fprintf(outSyntactic, "Rule (STATEMENT -> return STATEMENT_t ) \n");

		return parse_STATEMENT_t();
	}break;

	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (STATEMENT -> id STATEMENT_t2 ) \n");

		// semantic
		Symbol* entry_of_id = lookup(symbolTable, curr_token->lexeme);
		if (entry_of_id == NULL)
		{
			fprintf(outSemantic, "ERROR at line: %d the variable / function with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
		}
		parse_STATEMENT_t2(entry_of_id);

		// semantic
		return EMPTY;
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

		// semantic
		return EMPTY;

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

		//semantic todo
		return ERROR;
	}
	}
}

Type parse_STATEMENT_t()
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
		return parse_EXPRESSION();
	}break;

	case TOKEN_SEP_SEMICOLON:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t -> EPSILON ) \n");

		back_token();

		//semantic 
		return VOID;
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
		
		// semantic todo
		return ERROR;
	}
	}
}

void parse_STATEMENT_t2(Symbol* entry_of_id)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	case TOKEN_OP_ASSIGN:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t2 -> VARIABLE_t = EXPRESSION ) \n");

		// semantic
		int size_of_VARIABLE_t;

		back_token();
		size_of_VARIABLE_t = parse_VARIABLE_t();

		//semantic 
		if (entry_of_id != NULL) 
		{
			int size_of_id = entry_of_id->size_arry_or_num_parameters;

			//semantic
			if (entry_of_id->kind == FUNCTION)
			{
				fprintf(outSemantic, "ERROR at line:%d - you try to do assignment to function \n", curr_token->lineNumber);
			} 
			else if (size_of_id > -1 && size_of_VARIABLE_t == -1)
			{
				// role-7 and 5
				fprintf(outSemantic, "ERROR at line:%d - you try use array like val \n", curr_token->lineNumber);
				//return ERROR;
			}
			else if (size_of_id == -1 && size_of_VARIABLE_t != -1)
			{
				// role-8
				fprintf(outSemantic, "ERROR at line:%d - you try use val like array \n", curr_token->lineNumber);
			}
			else if (size_of_id > -1 && size_of_VARIABLE_t > size_of_id)
			{
				// role-9
				fprintf(outSemantic, "ERROR at line:%d - out of bound array \n", curr_token->lineNumber);
			}
		}


		match(TOKEN_OP_ASSIGN);
		Type type_of_EXPRESSION = parse_EXPRESSION();

		//semantic
		if (entry_of_id != NULL && type_of_EXPRESSION != ERROR)
		{
			if (entry_of_id->type == INTEGER && type_of_EXPRESSION != INTEGER)
			{
				fprintf(outSemantic, "ERROR at line: %d missmatch of types \n", curr_token->lineNumber);
			}
		}
	}break;

	case TOKEN_SEP_L_ROUND_BRACKET:
	{
		// semantic
		int num_of_PARAMETERS_LIST;

		fprintf(outSyntactic, "Rule (STATEMENT_t2 -> ( PARAMETERS_LIST ) ) \n");

		num_of_PARAMETERS_LIST = parse_PARAMETERS_LIST();

		//semantic
		if (entry_of_id != NULL)
		{
			if (entry_of_id->size_arry_or_num_parameters != num_of_PARAMETERS_LIST)
			{
				fprintf(outSemantic, "ERROR at line:%d - missmatch number of parametrs need : %d and actual : %d \n", curr_token->lineNumber, entry_of_id->size_arry_or_num_parameters, num_of_PARAMETERS_LIST);
			}
		}

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


Type parse_BLOCK()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_CURLY_BRACKET:
	{
		fprintf(outSyntactic, "Rule (BLOCK -> { VAR_DEFINITIONS;STATEMENTS } ) \n");

		parse_VAR_DEFINITIONS();
		match(TOKEN_SEP_SEMICOLON);
		Type type_of_STATEMENTS = parse_STATEMENTS();
		match(TOKEN_SEP_R_CURLY_BRACKET);

		//semantic
		return type_of_STATEMENTS;
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

		//semantic todo
		return ERROR;
	}
	}
}


int parse_PARAMETERS_LIST()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (PARAMETERS_LIST -> VARIABLES_LIST ) \n");

		back_token();
		return parse_VARIABLES_LIST(TO_USE,VOID); // semantic need to change this !-!-!
	}break;

	case TOKEN_SEP_R_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (PARAMETERS_LIST -> EPSILON ) \n");

		back_token();

		//semantic 
		return 0;
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

		//semantic todo
		return -1;
	}
	}
}

Type parse_EXPRESSION()
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION -> id EXPRESSION_t ) \n");

		//semantic
		Symbol* entry_of_id = lookup(symbolTable, curr_token->lexeme);
		Type type_to_return;
		if (entry_of_id == NULL)
		{
			fprintf(outSemantic, "ERROR at line: %d the variable with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
			type_to_return = ERROR;
		}

		Type type_of_EXPRESSION_t = parse_EXPRESSION_t(entry_of_id);

		//semantic
		if (entry_of_id != NULL && type_of_EXPRESSION_t != ERROR)
		{
			if (entry_of_id->type == INTEGER && type_of_EXPRESSION_t == INTEGER)
			{
				type_to_return = INTEGER;
			}
			else
			{
				type_to_return = REAL;
			}
		}
		else
		{
			type_to_return = ERROR;
		}
		return type_to_return;
	}break;

	case TOKEN_REAL_NUMBER:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION -> real_number ) \n");

		//semantic
		return REAL;
	}break;
	case TOKEN_INT_NUBMER:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION -> int_number ) \n");

		//semantic
		return INTEGER;
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
		return ERROR;
	}
	}
}

Type parse_EXPRESSION_t(Symbol* entry_of_id)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> VARIABLE_t ) \n");

		//semantic
		int size_of_VARIABLE_t;

		back_token();
		size_of_VARIABLE_t = parse_VARIABLE_t();

		//semantic 
		if (entry_of_id != NULL)
		{
			int size_of_id = entry_of_id->size_arry_or_num_parameters;

			//semantic 
			if (entry_of_id->kind == FUNCTION)
			{
				fprintf(outSemantic, "ERROR at line:%d - you try to use function - in this grammer is ban \n", curr_token->lineNumber);
				return ERROR;
			}
			else if (size_of_id > -1 && size_of_VARIABLE_t == -1)
			{
				// role-7 and 5
				fprintf(outSemantic, "ERROR at line:%d - you try use array like val \n", curr_token->lineNumber);
				return ERROR;
			}
			else if (size_of_id == -1 && size_of_VARIABLE_t != -1)
			{
				// role-8
				fprintf(outSemantic, "ERROR at line:%d - you try use val like array \n", curr_token->lineNumber);
				return ERROR;
			}
			else if (size_of_id > -1 && size_of_VARIABLE_t > size_of_id)
			{
				// role-9
				fprintf(outSemantic, "ERROR at line:%d - out of bound array \n", curr_token->lineNumber);
				return ERROR;
			}
			else
			{
				return entry_of_id->type;
			}
		}
		else
		{
			return ERROR;
		}
		}break;


	case TOKEN_OP_MUL:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> *EXPRESSION ) \n");

		return parse_EXPRESSION();
	}break;
	case TOKEN_OP_DIV:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> /EXPRESSION ) \n");

		return parse_EXPRESSION();
	}break;

	case TOKEN_SEP_SEMICOLON:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> EPSILON ) \n");

		back_token();
		//semantic todo - need to check this id is not function
		if (entry_of_id != NULL)
		{
			if (entry_of_id->kind == FUNCTION)
			{
				fprintf(outSemantic, "ERROR at line:%d - you try to use function - in this grammer is ban \n", curr_token->lineNumber);
				return ERROR;
			}
			else
			{
				if (entry_of_id->size_arry_or_num_parameters == -1)
				{
					return entry_of_id->type;
				}
				else
				{
					// role-7 and 5
					fprintf(outSemantic, "ERROR at line:%d - you try use array like val \n", curr_token->lineNumber);
					return ERROR;
				}
			}
			
		}
		return ERROR;
		
		
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
		
		//semantic todo
		return ERROR;
	}
	}
}