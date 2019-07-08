#include "Parser.h"
#include "symbol.h"
#include "symbolList.h"
#include "symbolTable.h"

SymbolTable  *symbolTable;

void parse_PROGRAM()
{
	// semantic 
	// global table - contain scoop main and function
	symbolTable = NULL;
	symbolTable = make_table(symbolTable);

	Token * curr_token = next_token();
	// first program
	switch (curr_token->kind)
	{
	case TOKEN_KEY_PROGRAM:
	{
		//semantic 
		// main scoop insert 
		symbolTable = make_table(symbolTable);

		fprintf(outSyntactic, "Rule (PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS) \n");
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEP_SEMICOLON);
		parse_STATEMENTS(VOID);
		match(TOKEN_KEY_END);

		//semantic 
		// main scoop exit
		symbolTable = pop_table(symbolTable);


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

	// go out from global table
	pop_table(symbolTable);

}


int parse_VAR_DEFINITIONS()
{
	Token*	curr_token = next_token();

	switch (curr_token->kind)
	{

	case TOKEN_KEY_REAL:
	case TOKEN_KEY_INTEGER:
	{

		fprintf(outSyntactic, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_t) \n");
		back_token();
		int num_of_VAR_DEFINITION = parse_VAR_DEFINITION();
		int num_of_VAR_DEFINITIONS_t = parse_VAR_DEFINITIONS_t();

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
		Type type_of_TYPE = parse_TYPE();
		return parse_VARIABLES_LIST(TO_DEFINE, type_of_TYPE);
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

Type parse_TYPE()
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

int parse_VARIABLES_LIST(enum action action, enum type type)
{
	Token* curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST -> VARIABLE VARIABLES_LIST_t) \n");
		back_token();
		parse_VARIABLE(action, type);
		int num_of_VARIABLES_LIST_t = parse_VARIABLES_LIST_t(action, type);

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

int parse_VARIABLES_LIST_t(enum action action, enum type type)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_COMMA:
	{
		fprintf(outSyntactic, "Rule (VARIABLES_LIST_t -> ,VARIABLE VARIABLES_LIST_t) \n");
		parse_VARIABLE(action, type);
		int num_of_VARIABLES_LIST_t = parse_VARIABLES_LIST_t(action, type);
		
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

void parse_VARIABLE(enum action action, enum type type)
{
	Token*	curr_token = next_token();

	switch (curr_token->kind)
	{
	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (VARIABLE -> id VARIABLE_t) \n");
		int size_of_VARIABLE_t = parse_VARIABLE_t();



		if (action == TO_USE)
		{
			Symbol* entry_of_id = find(symbolTable, curr_token->lexeme);
			if (entry_of_id == NULL)
			{
				fprintf(outSemantic, "ERROR at line:%d  the variable with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
			}
			else
			{
				check_Use(entry_of_id, size_of_VARIABLE_t, curr_token);
			}
		}

		else // want to define var with name curr_token->lexeme
		{
			// semantic
			int inserted;

			if (size_of_VARIABLE_t == -1)
			{
				inserted = insert(symbolTable, curr_token->lexeme, type, size_of_VARIABLE_t, VARIABLE, curr_token->lineNumber);
			}
			else // its array 
			{
				inserted = insert(symbolTable, curr_token->lexeme, type, size_of_VARIABLE_t, ARRAY, curr_token->lineNumber);
			}


			if (!inserted)
			{
				Symbol* entry_of_variable = lookup(symbolTable, curr_token->lexeme);  // find this in current table
				fprintf(outSemantic, "ERROR at line:%d  the variable with lexme: %s alredy define at line: %d \n",
						curr_token->lineNumber, curr_token->lexeme,entry_of_variable->num_line_decler);
			}
		}

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

	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	{
		fprintf(outSyntactic, "Rule (VARIABLE_t -> [int_number] ) \n");

		//semantic
		Token*  token_for_size = next_token();
		back_token(); // peak for save token of int_number

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
		Type type_of_RETURNED_TYPE = parse_RETURNED_TYPE();
		
		// semantic
		Token* token_of_id = next_token();
		back_token(); // peak 

		match(TOKEN_ID);
		match(TOKEN_SEP_L_ROUND_BRACKET);

		// semantic - do this here becous i want the names of parametrs will be in list symbol of this function
		symbolTable = make_table(symbolTable);

		int num_of_PARAM_DEFINITIONS = parse_PARAM_DEFINITIONS();
		match(TOKEN_SEP_R_ROUND_BRACKET);


		SymbolList* list_of_parametrs = get_List_Parameters(symbolTable);// in this point exsist in table only param of this fanction

		//semantic                     // insert to global table
		int inserted = insert_Function(symbolTable->parentSymbolTable, token_of_id->lexeme, type_of_RETURNED_TYPE, num_of_PARAM_DEFINITIONS, FUNCTION, curr_token->lineNumber,list_of_parametrs);
		if (!inserted)
		{
			Symbol* entry_of_function = lookup(symbolTable->parentSymbolTable, token_of_id->lexeme);
			fprintf(outSemantic, "ERROR at line:%d  the function with lexme:%s alredy define at line: %d \n",
				token_of_id->lineNumber, token_of_id->lexeme, entry_of_function->num_line_decler);
			
			// free the parametrs becous them not insert to table so enyone cam free them 
			symbolList_freeList(list_of_parametrs);
		}
		
		Type type_of_BLOCK = parse_BLOCK(type_of_RETURNED_TYPE);

		//semantic
		symbolTable = pop_table(symbolTable);

		if (type_of_RETURNED_TYPE != VOID && type_of_BLOCK == EMPTY)
		{
			fprintf(outSemantic, "ERROR at line:%d  the function with lexme:%s define type: %s  but no return \n", token_of_id->lineNumber, token_of_id->lexeme,ENUM_TYPE[type_of_RETURNED_TYPE]);
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

Type parse_STATEMENTS(enum type need_type)
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
		Type type_of_STATEMENT = parse_STATEMENT(need_type);
		match(TOKEN_SEP_SEMICOLON);
		Type type_of_STATEMENTS_t = parse_STATEMENTS_t(need_type);

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
		else // two type diff 
		{
			if (type_of_STATEMENT == ERROR)
			{
				return type_of_STATEMENTS_t;
			}
			else if(type_of_STATEMENTS_t == ERROR)
			{
				return type_of_STATEMENT;
			}
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

Type parse_STATEMENTS_t(enum type need_type)
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
		return parse_STATEMENTS(need_type);
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

Type parse_STATEMENT(enum type need_type)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_KEY_RETURN:
	{
		fprintf(outSyntactic, "Rule (STATEMENT -> return STATEMENT_t ) \n");
		Type type_of_STATEMENT_t = parse_STATEMENT_t();

		//semantic
		if (need_type != type_of_STATEMENT_t)
		{
			fprintf(outSemantic, "ERROR at line:%d  missmatch return type : need return type: %s actual: %s \n", curr_token->lineNumber,ENUM_TYPE[need_type],ENUM_TYPE[type_of_STATEMENT_t]);
			return ERROR;
		}
		return type_of_STATEMENT_t;

	}break;

	case TOKEN_ID:
	{
		fprintf(outSyntactic, "Rule (STATEMENT -> id STATEMENT_t2 ) \n");

		// semantic
		Symbol* entry_of_id = find(symbolTable, curr_token->lexeme);
		if (entry_of_id == NULL)
		{
			fprintf(outSemantic, "ERROR at line:%d  the variable / function with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
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
		symbolTable = make_table(symbolTable);

		Type type_of_BLOCK = parse_BLOCK(need_type);

		// semantic 
		symbolTable = pop_table(symbolTable);

		// semantic
		return type_of_BLOCK;

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

void parse_STATEMENT_t2(struct symbol * entry_of_id)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	case TOKEN_OP_ASSIGN:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t2 -> VARIABLE_t = EXPRESSION ) \n");

		back_token();
		int size_of_VARIABLE_t = parse_VARIABLE_t();
		match(TOKEN_OP_ASSIGN);
		Type type_of_EXPRESSION = parse_EXPRESSION();

		//semantic
		check_Use(entry_of_id, size_of_VARIABLE_t, curr_token); // check that kind of id its OK

		if (entry_of_id != NULL && type_of_EXPRESSION != ERROR)// check that types of id and expression OK
		{
			if (entry_of_id->type == INTEGER && type_of_EXPRESSION != INTEGER)
			{
				fprintf(outSemantic, "ERROR at line:%d  missmatch of types ID: %s is INTEGER  \n", curr_token->lineNumber, entry_of_id->id);
			}
		}
	}break;

	case TOKEN_SEP_L_ROUND_BRACKET:
	{
		fprintf(outSyntactic, "Rule (STATEMENT_t2 -> ( PARAMETERS_LIST ) ) \n");
		
		int num_of_PARAMETERS_LIST = parse_PARAMETERS_LIST();
		match(TOKEN_SEP_R_ROUND_BRACKET);


		//semantic
		if (entry_of_id != NULL )
		{
			if (entry_of_id->kind != FUNCTION)
			{
				fprintf(outSemantic, "ERROR at line:%d  the ID: %s is array/val not function \n", curr_token->lineNumber,entry_of_id->id);
			}
			else if (entry_of_id->size_arry_or_num_parameters != num_of_PARAMETERS_LIST)
			{
				fprintf(outSemantic, "ERROR at line:%d  missmatch number of parametrs need : %d and actual : %d \n", curr_token->lineNumber, entry_of_id->size_arry_or_num_parameters, num_of_PARAMETERS_LIST);
			}
			
		}

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


Type parse_BLOCK(enum type need_type)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_CURLY_BRACKET:
	{
		fprintf(outSyntactic, "Rule (BLOCK -> { VAR_DEFINITIONS;STATEMENTS } ) \n");

		parse_VAR_DEFINITIONS();
		match(TOKEN_SEP_SEMICOLON);
		Type type_of_STATEMENTS = parse_STATEMENTS(need_type);
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
		return parse_VARIABLES_LIST(TO_USE,VOID);
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
		Symbol* entry_of_id = find(symbolTable, curr_token->lexeme);
		Type type_to_return;
		if (entry_of_id == NULL)
		{
			fprintf(outSemantic, "ERROR at line:%d  the variable with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
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

Type parse_EXPRESSION_t(struct symbol * entry_of_id)
{
	Token*	curr_token = next_token();
	switch (curr_token->kind)
	{
	case TOKEN_SEP_L_BRACKET:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> VARIABLE_t ) \n");

		back_token();
		int size_of_VARIABLE_t = parse_VARIABLE_t();

		//semantic 
		return check_Use(entry_of_id, size_of_VARIABLE_t, curr_token);
		
		}break;


	case TOKEN_OP_MUL:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> *EXPRESSION ) \n");

		//semantic
		Type type_of_check = check_Use(entry_of_id, -1, curr_token);
		Type type_of_EXPRESSION = parse_EXPRESSION();

		if (type_of_check == ERROR || type_of_EXPRESSION == ERROR)
		{
			return ERROR;
		}
		return type_of_EXPRESSION;

	}break;
	case TOKEN_OP_DIV:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> /EXPRESSION ) \n");

		//semantic
		Type type_of_check = check_Use(entry_of_id, -1, curr_token);
		Type type_of_EXPRESSION = parse_EXPRESSION();

		if (type_of_check == ERROR || type_of_EXPRESSION == ERROR)
		{
			return ERROR;
		}
		return type_of_EXPRESSION;

	}break;

	case TOKEN_SEP_SEMICOLON:
	{
		fprintf(outSyntactic, "Rule (EXPRESSION_t -> EPSILON ) \n");

		back_token();
		
		//semantic
		return check_Use(entry_of_id, -1, curr_token);
		
		
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

Type check_Use(struct symbol* entry_of_id, int num_of_VARIABLE_t, Token* curr_token)
{
	if (entry_of_id != NULL)
	{ 
		if (entry_of_id->kind == FUNCTION)
		{
			fprintf(outSemantic, "ERROR at line:%d  the ID: %s is function \n", curr_token->lineNumber,entry_of_id->id);
			return ERROR;
		}
		else if (entry_of_id->kind == ARRAY && num_of_VARIABLE_t == -1)
		{
			fprintf(outSemantic, "ERROR at line:%d  the ID: %s is array not val \n", curr_token->lineNumber,entry_of_id->id);
			return ERROR;
		}
		else if (entry_of_id->kind == VARIABLE && num_of_VARIABLE_t != -1)
		{
			fprintf(outSemantic, "ERROR at line:%d  the ID: %s is val not array \n", curr_token->lineNumber,entry_of_id->id);
			return ERROR;
		}
		else if (entry_of_id->kind == ARRAY && num_of_VARIABLE_t >= entry_of_id->size_arry_or_num_parameters)
		{
			fprintf(outSemantic, "ERROR at line:%d  out of bounds array : the ID: %s is array in size: %d  \n", curr_token->lineNumber,entry_of_id->id, entry_of_id->size_arry_or_num_parameters);
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
}

SymbolList* get_List_Parameters(struct symbolTable  * symbolTable)
{
	SymbolList* list_of_parametrs = NULL;
	SymbolList* temp = symbolTable->symbolListHead;
	while (temp != NULL)
	{
		list_of_parametrs = symbolList_insertEntry(list_of_parametrs, temp->symbol);
		temp = temp->nextEntry;
	}
	return list_of_parametrs;
}