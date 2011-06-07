#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void error(Error err, unsigned addr){
		printf("ERROR: ");
		switch (err) {
		case  ERR_NOERROR:
			printf("No error");
			printf("\tat 0x%08x\n",addr);
			break;
		case ERR_UNKNOWN:
			printf("Unknown instruction");
			printf("\tat 0x%08x\n",addr);
			exit(0);
			break;
		case ERR_ILLEGAL:
			printf("Illegal instruction");
			printf("\tat 0x%08x\n",addr);
			exit(0);
			break;
		case ERR_CONDITION:
			printf("Illegal condition");
			printf("\tat 0x%08x\n",addr);
			exit(0);
			break;
		case ERR_IMMEDIATE:
			printf("Immediate value forbidden");
			printf("\tat 0x%08x\n",addr);
			exit(0);
			break;
		case ERR_SEGTEXT:
			printf("Text index out of bounds");
			printf("\tat 0x%08x\n",addr);
			exit(0);
			break;
		case ERR_SEGDATA:
			printf("Data index out of bounds");
			printf("\tat 0x%08x\n",addr);
			exit(0);
			break;
		case ERR_SEGSTACK:
			printf("Stack index out of bounds");
			printf("\tat 0x%08x\n",addr);
			exit(0);
			break;
		}
}

void warning(Warning warn, unsigned addr){
	printf("WARNING: Program correctly ended by HALT\t0x%08x\n",addr);
}

