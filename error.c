#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void error(Error err, unsigned addr){
		perror("ERROR: ");
		switch (err) {
		case  ERR_NOERROR:
			perror("No error");
			printf("\t0x%08x\n",addr);
			break;
		case ERR_UNKNOWN:
			perror("Unknown instruction");
			printf("\t0x%08x\n",addr);
			exit(0);
			break;
		case ERR_ILLEGAL:
			perror("Illegal instruction");
			printf("\t0x%08x\n",addr);
			exit(0);
			break;
		case ERR_CONDITION:
			perror("Illegal condition");
			printf("\t0x%08x\n",addr);
			exit(0);
			break;
		case ERR_IMMEDIATE:
			perror("Immediate value forbidden");
			printf("\t0x%08x\n",addr);
			exit(0);
			break;
		case ERR_SEGTEXT:
			perror("Text index out of bounds");
			printf("\t0x%08x\n",addr);
			exit(0);
			break;
		case ERR_SEGDATA:
			perror("Data index out of bounds");
			printf("\t0x%08x\n",addr);
			exit(0);
			break;
		case ERR_SEGSTACK:
			perror("Stack index out of bounds");
			printf("\t0x%08x\n",addr);
			exit(0);
			break;
		}
}

void warning(Warning warn, unsigned addr){
	printf("WARNING: Program correctly ended by HALT\t0x%08x\n",addr);
}

