//stack.c
//#author: Alexandria Jones
// linked list stack implimentation

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
//creates an empty stack
stack_t* make_stack(void){
	stack_t* s = malloc(sizeof(stack_t));
	if(!s){
		return NULL;
	}
	s->top = NULL;
	return s;


}
//adds data to top of stack
void push (stack_t* stack, void*data){
	if(!stack) return;
	stack_node_t* new_node =  malloc(sizeof(stack_node_t));
	
	if(!new_node){
		fprintf(stderr, "Error: failed to allocate stack node\n");
		return;

	}
	new_node->data = data;
	new_node->next = stack->top;
	stack->top = new_node;


}
//returns top element w/o removing
void * top(stack_t *stack){
	if(empty_stack(stack)){
		fprintf(stderr, "Error: attempt to the top of an empty stack\n");
		exit(EXIT_FAILURE);

	}
	return stack->top->data;

}
// removes top elem from stack
void pop(stack_t * stack){
	if(empty_stack(stack)){
		fprintf(stderr, "Error: attempted to pop from empty stack\n");
		exit(EXIT_FAILURE);

	}
	stack_node_t * temp= stack-> top;
	stack->top = stack->top->next;
	free(temp);


}
//checks if stack is empty
int empty_stack(stack_t * stack){
	return(stack == NULL || stack->top == NULL); 


}
//frees all stack memory
void free_stack(stack_t * stack){
	if(!stack) return;
	while(!empty_stack(stack)){
		pop(stack);

	}
	free(stack);

}
