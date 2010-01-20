/* main.cc */

#include "heading.h"
#include <stdlib.h>
#include "../global/GlobalVariables.h"

// prototype of bison-generated parser function
extern int yyparse();
extern int yy_scan_string(const char *);

void initVariables();
void releaseVariables();


void initVariables(){
	executeStructures = new ExecuteStructures();
}

void destroyVariables(){
	delete executeStructures;
}


STATUS_CODE parse_main(char *buff) {
	initVariables();

	yy_scan_string(buff);
	if(!yyparse()){
		return PARSE_FAILED;
	}

	return 0;
}

int main(int argc,char **argv){

	char *buff = "CREATE TABLE HELLO(col_one INTEGER)";
	parse_main(buff);

  return 0;
}

