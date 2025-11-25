//test_symtab.c
//author: alexandria jones
//testing symbol table implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

int main(void){
	printf("Testing symbol table...\n");

	//test creating symbols
	symbol_t* s1 = create_symbol("x", 10);
	if(!s1){
		printf("FAIL: couldn't create symbol\n");
		return 1;
	}
	printf("PASS: created symbol x = 10\n");

	symbol_t* s2 = create_symbol("y", 20);
	if(!s2){
		printf("FAIL: couldn't create second symbol\n");
		return 1;
	}
	printf("PASS: created symbol y = 20\n");

	//test lookup
	symbol_t* found = lookup_table("x");
	if(!found){
		printf("FAIL: couldn't find symbol x\n");
		return 1;
	}
	if(found->val != 10){
		printf("FAIL: x should be 10, got %d\n", found->val);
		return 1;
	}
	printf("PASS: lookup found x with correct value\n");

	//test lookup non-existent
	found = lookup_table("z");
	if(found){
		printf("FAIL: shouldn't find symbol z\n");
		return 1;
	}
	printf("PASS: lookup returns NULL for missing symbol\n");

	//test dump
	printf("\nDumping table:\n");
	dump_table();
	//test update
	found = lookup_table("x");
	found->val = 99;
	found = lookup_table("x");
	if(found->val != 99){
		printf("FAIL: update didn't work\n");
		return 1;
	}
	printf("\nPASS: symbol update works\n");
	//cleanup
	free_table();

	printf("\nAll symbol table tests passed!\n");
	return 0;
}
