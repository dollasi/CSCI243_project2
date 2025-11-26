//tree_node.c
//#author: alexandria jones
//tree node construction for parse trees

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_node.h"
//creates the leaf node for the int and symb
tree_node_t * make_leaf(exp_type_t exp_type, char* token){
	tree_node_t* n = malloc(sizeof(tree_node_t));
	if(!n){
		return NULL;
	}

	n->type = LEAF;
	n->token = malloc(strlen(token) + 1);
	
	if(!n->token){
		free(n);
		return NULL;
	}


	strcpy(n->token, token);

	leaf_node_t * lf = malloc(sizeof(leaf_node_t));
	if(!lf){
		free(n->token);
		free(n);
		return NULL;
	}

	lf->exp_type = exp_type;
	n->node = (void*)lf;

	return n;
}
//creates interior node for operation
tree_node_t * make_interior(op_type_t op, char * token, tree_node_t* left, tree_node_t* right){
	tree_node_t* n = malloc(sizeof(tree_node_t));
	if(!n){
		return NULL;
	}

	n->type = INTERIOR;
	n->token = malloc(strlen(token) +1);
	

	if(!n->token){
		free(n);
		return NULL;
	}
	strcpy(n->token, token);
	interior_node_t* inner = malloc(sizeof(interior_node_t));
	if(!inner){

		free(n->token);
		free(n);
		return NULL;
	}

	inner->op = op;
	inner->left = left;
	inner->right = right;
	n->node = (void*)inner;


	return n;



}
//recursively frees tree memory
void cleanup_tree(tree_node_t* node){
	if(!node) return;
	if(node->type == INTERIOR){
		interior_node_t* inner = (interior_node_t*)node->node;
		cleanup_tree(inner->left);
		cleanup_tree(inner->right);
		free(inner);
	} else {
		leaf_node_t* lf = (leaf_node_t*)node->node;
		free(lf);
	}

	free(node->token);
	free(node);
}
