//test_stack.c
//author: alexandria jones
//testing stack implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

int main(void){
	printf("Testing stack...\n");

	stack_t* s = make_stack();
	if(!s){
		printf("FAIL: couldn't create stack\n");
		return 1;
	}
	printf("PASS: created stack\n");

	//test empty
	if(!empty_stack(s)){
		printf("FAIL: new stack should be empty\n");
		return 1;
	}
	printf("PASS: new stack is empty\n");

	//test push
	char* token1 = malloc(10);
	strcpy(token1, "hello");
	push(s, token1);

	if(empty_stack(s)){
		printf("FAIL: stack should not be empty after push\n");
		return 1;
	}
	printf("PASS: stack not empty after push\n");

	//test top
	char* t = (char*)top(s);
	if(strcmp(t, "hello") != 0){
		printf("FAIL: top should be 'hello', got '%s'\n", t);
		return 1;
	}
	printf("PASS: top returns correct value\n");

	//test multiple pushes
	char* token2 = malloc(10);
	strcpy(token2, "world");
	push(s, token2);

	t = (char*)top(s);
	if(strcmp(t, "world") != 0){
		printf("FAIL: top should be 'world', got '%s'\n", t);
		return 1;
	}
	printf("PASS: LIFO order works\n");

	//test pop
	pop(s);
	t = (char*)top(s);
	if(strcmp(t, "hello") != 0){
		printf("FAIL: after pop, top should be 'hello'\n");
		return 1;
	}
	printf("PASS: pop works correctly\n");
	
	//cleanup
	free(token1);
	pop(s);
	free_stack(s);
	
	printf("\nAll stack tests passed!\n");
	return 0;
}
