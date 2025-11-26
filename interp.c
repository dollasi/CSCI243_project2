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
//main func
int main(int argc, char* argv[]){
	if(argc >2){//checks command line args
		fprintf(stderr, "Usage: interp [sym-table]\n");
		return EXIT_FAILURE;
	}
	//builds table from file
	if(argc == 2){
		build_table(argv[1]);

	}else{ build_table(NULL);
	}
//print initial table
	dump_table();

	printf("Enter postfix expressions (CTRL-D to exit):\n");
//read-eval-print loop
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
		rep(line);//process expression

//print final table/ cleanup
	}
	printf("\n");
	dump_table();
	free_table();
	return EXIT_SUCCESS;
}
