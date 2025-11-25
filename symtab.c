// symtab.c
//author Alexxandria Jones
//symbol table using linked list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symtab.h"

static symbol_t * table_head = NULL;

symbol_t * create_symbol(char * name, int val)
	symbol_t* new_sym = malloc(sizeof(symbol_t));
	if(!new_sym){
		fprintf(stderr, "No room in symbol table\n");
		return NULL;
	}
	
	new_sym->var_name = malloc(strlen(name) + 1);
	if(!new_sym->var_name){
		free(new_sym);
		fprintf(stderr, "No room in symbol table\n");
		return NULL;
	}

	strcpy(new_sym->var_name, name);
	new_sym->val = val;
	new_sym->next = table_head;
	table_head = new_sym;

	return new_sym;
}


symbol_t * lookup_table(char * variable){
	symbol_t* current = table_head;
	while(current){
		if(strcmp(current->var_name, variable) == 0){
			return current;
		}
		current = current->next;
	}
	return NULL;
}





void build_table(char* filename){
	if(!filename){
		return;
	}
	FILE * fp = fopen(filename, "r");
	if(!fp){
		perror("Error loaded symbol table");
		exit(EXIT_FAILURE);
	}
	
	char line[BUFLEN];
	while(fgets(line, sizeof(line), fp)){//skip comment line
		if(line[0] == '#'){
			continue;
		}
		char name[BUFLEN];

		int value;

		//parse
		if(sscanf(line, "%s %d", name, &value) == 2){
			if(isalpha(name[0])){
				create_symbol(name, value);
			}

		}

	}

	fclose(fp);

}

//symbol_t * create_symbol(){

//}


void free_table(void){
	symbol_t* current = table_head;
	while(current){
		symbol_t* next = current->next;
		free(current->var_name);
		free(current);
		current = next;
	}
	table_head = NULL;
}


void dump_table(void){
	if(!table_head){
		return;
	}
	
	printf("SYMBOL TABLE:\n");
	symbol_t* current = table_head;
	while(current){
		printf("\tName: %s, Value: %d\n", current->var_name, current->val);
		current = current->next;
	}
}

