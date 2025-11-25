//parser.c
//author: Alexandria ones
//parser for postfix expressions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "symtab.h"

static parse_error_t parse_err = PARSE_NONE;
static eval_error_t eval_err = EVAL_NONE;

// check if token is an operation
static op_type_t get_op_type(const char* token){
	if(strcmp(token, ADD_OP_STR) == 0) return ADD_OP;
	if(strcmp(token, SUB_OP_STR) == 0) return SUB_OP;
	if(strcmp(token, MUL_OP_STR) == 0) return MUL_OP;
	if(strcmp(token, DIV_OP_STR) == 0) return DIV_OP;
	if(strcmp(token, MOD_OP_STR) == 0) return MOD_OP;
	if(strcmp(token, Q_OP_STR) == 0) return Q_OP;
	if(strcmp(token, ASSIGN_OP_STR) == 0) return ASSIGN_OP;
	return NO_OP;
}


static int is_integer(const char * token){
	if(!isdigit(token[0])) return 0;
	for(size_t i = 1; i < strlen(token); i++){
		if(!isdigit(token[i])) return 0;


	}
	return 1;


}
 

static int is_symbol(const char * token){
	if(!isalpha(token[0])) return 0;
	for(size_t i = 1; i < strlen(token); i++){
		if(!isalnum(token[i])) return 0;


	}
	return 1;


}



tree_node_t * parse(stack_t * stack){
	if(empty_stack(stack)){
		parse_err = TOO_FEW_TOKENS;
		return NULL;
	}

	char * token = (char *)top(stack);
	pop(stack);

	op_type_t op = get_op_type(token);
	
	if(op != NO_OP){
		//handle ternary specially
		if(op == Q_OP){
			tree_node_t* false_expr = parse(stack);
			if(!false_expr){
				free(token);
				return NULL;
			}

			tree_node_t * true_expr = parse(stack);
			if(!true_expr){
				cleanup_tree(false_expr);
				free(token);
				return NULL;

			}
			
			tree_node_t * test = parse(stack);
			if(!test){
				cleanup_tree(false_expr);
				cleanup_tree(true_expr);
				free(token);
				return NULL;
			}
			tree_node_t * alt = make_interior(ALT_OP, ":", true_expr, false_expr);
			if(!alt){
				cleanup_tree(test);
				cleanup_tree(true_expr);
				cleanup_tree(false_expr);
				free(token);
				return NULL;
			}
			
			tree_node_t* result = make_interior(Q_OP, token, test, alt);
			free(token);
			return result;
		}
		tree_node_t * right = parse(stack);
		if(!right){
			free(token);
			return NULL;
		}
		tree_node_t * left = parse(stack);
		if(!left){
			cleanup_tree(right);
			free(token);
			return NULL;
		}
		
		tree_node_t * result = make_interior(op, token, left, right);
		free(token);
		return result;
	}
	
	if(is_integer(token)){
		tree_node_t * result = make_leaf(INTEGER, token);
		free(token);
		return result;
	}

	if(is_symbol(token)){
		tree_node_t * result = make_leaf(SYMBOL, token);
		free(token);
		return result;
	}
	
	// illegal token
	parse_err = ILLEGAL_TOKEN;
	free(token);
	return NULL;


}

tree_node_t* make_parse_tree(char* expr){
	parse_err = PARSE_NONE;
	stack_t* stack = make_stack();

	if(!stack) return NULL;
	
	//tokenize expression
	char* token = strtok(expr, " \t\n");
	while(token){
		// allocate copy of token
		char * token_copy = malloc(strlen(token) +1);
		if(!token_copy){
			//cleanup
			while(!empty_stack(stack)){
				free(top(stack));
				pop(stack);
			}
			free_stack(stack);
			return NULL;
		}
		strcpy(token_copy, token);
		
		push(stack, token_copy);
		token = strtok(NULL, " \t\n");
	}
	
	//check if empty
	if(empty_stack(stack)){
		free_stack(stack);
		return NULL;
	}
	tree_node_t* root = parse(stack);
	
	// errors
	if(!root){
		while(!empty_stack(stack)){
			free(top(stack));
			pop(stack);
		}
		free_stack(stack);
		
		if(parse_err == TOO_FEW_TOKENS){
			fprintf(stderr, "Invalid expression, not enough tokens\n");
		}else if(parse_err == ILLEGAL_TOKEN){
			fprintf(stderr, "Illegal token\n");
		}
		return NULL;
	}
	

	if(!empty_stack(stack)){
		parse_err = TOO_MANY_TOKENS;
		fprintf(stderr, "Invalid expression, too many tokens\n");

		//cleanup remaining tokens
		while(!empty_stack(stack)){
			free(top(stack));
			pop(stack);
		}
		free_stack(stack);
		cleanup_tree(root);
		return NULL;
	}

	free_stack(stack);
	return root;

}

int eval_tree(tree_node_t* node){
	eval_err = EVAL_NONE;

	if(!node) return 0;

	if(node->type == LEAF){
		leaf_node_t* lf = (leaf_node_t*)node->node;

		if(lf->exp_type == INTEGER){
			return (int)strtol(node->token, NULL, 10);
		

		}else if(lf->exp_type == SYMBOL){
			symbol_t* sym = lookup_table(node->token);
			if(!sym){
				eval_err = UNDEFINED_SYMBOL;
				fprintf(stderr, "Undefined symbol\n");
				return 0;
			}

			return sym->val;
		}else{
			eval_err = UNKNOWN_EXP_TYPE;
			fprintf(stderr, "Unknown node type\n");
			return 0;


		}
	}

	//int node
	interior_node_t* inner = (interior_node_t *)node->node;
	op_type_t op = inner->op;

	//handle assignment
	if(op == ASSIGN_OP){
		if(inner->left->type != LEAF){
			eval_err = MISSING_LVALUE;
			fprintf(stderr, "Missing l-value\n");
			return 0;
		}
		
		leaf_node_t * left_lf = (leaf_node_t*)inner->left->node;
		if(left_lf->exp_type != SYMBOL){
			eval_err = INVALID_LVALUE;
			fprintf(stderr, "Invalid l-value\n");
			return 0;
		}

		int val = eval_tree(inner->right);
		if(eval_err != EVAL_NONE) return 0;

		symbol_t * sym = lookup_table(inner->left->token);
		if(sym){
			sym->val = val;

		}else{
			if(!create_symbol(inner->left->token, val)){
				eval_err = SYMTAB_FULL;
				return 0;
			}
		}
		return val;


	}
	
	//handel ternary
	if(op == Q_OP){
		int test_val = eval_tree(inner->left);
		if(eval_err != EVAL_NONE) return 0;

		interior_node_t * alt = (interior_node_t *)inner->right->node;
		
		if(test_val){
			return eval_tree(alt->left);
		} else {

			return eval_tree(alt->right);
		}

	}

	//handle ALT_OP 
	if(op == ALT_OP){
		eval_err = UNKNOWN_OPERATION;
		fprintf(stderr, "Unknown operation\n");
		return 0;
	}
	
	//regular binary operations
	int left_val = eval_tree(inner->left);
	if(eval_err != EVAL_NONE) return 0;
	
	int right_val = eval_tree(inner->right);
	if(eval_err != EVAL_NONE) return 0;
	
	switch(op){
		case ADD_OP: return left_val + right_val;
		case SUB_OP: return left_val - right_val;
		case MUL_OP: return left_val * right_val;
		case DIV_OP:
			if(right_val == 0){
				eval_err = DIVISION_BY_ZERO;
				fprintf(stderr, "Division by zero\n");
				return 0;
			}
			return left_val / right_val;
		case MOD_OP:
			if(right_val == 0){
				eval_err = INVALID_MODULUS;
				fprintf(stderr, "Division by zero\n");
				return 0;
			}
			return left_val % right_val;
		

		default:
			eval_err = UNKNOWN_OPERATION;
			fprintf(stderr, "Unknown operation\n");

			return 0;

	}
}

void print_infix(tree_node_t* node){
	if(!node) return;
	
	if(node->type == LEAF){
		printf("%s", node->token);
		return;
	}

	interior_node_t * inner = (interior_node_t*)node->node;

	printf("(");
	print_infix(inner->left);

	printf("%s", node->token);
	print_infix(inner->right);
	printf(")");


}

void rep(char* exp){

	char* comment = strchr(exp, '#');
	if(comment){
		*comment = '\0';
	}

	int has_content = 0;
	for(size_t i = 0; i < strlen(exp); i++){
		if(!isspace(exp[i])){
			has_content = 1;
			break;

		}

	}
	if(!has_content) return;

	char expr_copy[MAX_LINE + 1];//copy for parsing
	strncpy(expr_copy, exp, MAX_LINE);
	expr_copy[MAX_LINE] = '\0';

	tree_node_t* root = make_parse_tree(expr_copy);
	
	if(!root) return;
	int result = eval_tree(root);
	if(eval_err == EVAL_NONE){
		print_infix(root);
		printf(" = %d\n", result);
	}



	cleanup_tree(root);
}
