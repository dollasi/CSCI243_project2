//interp.c
//author: Alexandria Jones
//main interpreter for postfix expressions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "symtab.h"
#include "parser.h"
#include "interp.h"
int main(int argc, char* argv[]){
	if(argc >2){
		fprintf(stderr, "Usage: interp [sym-table]\n");
		return EXIT_FAILURE;
	}
	
	if(argc == 2){
		build_table(argv[1]);

	}else{ build_table(NULL);
	}

	dump_table();

	printf("Enter postfix expressions (CTRL-D to exit):\n");

	char line[MAX_LINE +2];
	while(1){
		printf("> ");
		fflush(stdout);

		if(!fgets(line, sizeof(line), stdin)){
			break;
		}

		line[strcspn(line, "\n")] = '\0';

		if(line[0] == '\0' || line[0] == '#'){
			continue;
		}
		// line length
		if(strlen(line)>MAX_LINE){
			fprintf(stderr, "Input line too long\n");
			continue;

		}
		rep(line);


	}
	printf("\n");
	dump_table();
	free_table();
	return EXIT_SUCCESS;
}
