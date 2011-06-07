#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void error(Error err, unsigned addr){
		switch (err) {
		case  ERR_NOERROR:
			printf("%d: No error",addr);
			break;
		case ERR_UNKNOWN:
			printf("%d: ",addr);
			perror("Unknown instruction");
			break;
		case ERR_ILLEGAL:
			printf("%d: ",addr);
			perror("Illegal instruction");
			break;
		case ERR_CONDITION:
			printf("%d: ",addr);
			perror("Illegal condition");
			break;
		case ERR_IMMEDIATE:
			printf("%d: ",addr);
			perror("Immediate value forbidden");
			break;
		case ERR_SEGTEXT:
			printf("%d: ",addr);
			perror("Text index out of bounds");
			break;
		case ERR_SEGDATA:
			printf("%d: ",addr);
			perror("Data index out of bounds");
			break;
		case ERR_SEGSTACK:
			printf("%d: ",addr);
			perror("Stack index out of bounds");
			break;
		}
}

void warning(Warning warn, unsigned addr){
		switch (warn) {
			case  WARN_HALT:
				printf("%d: Warning",addr);
				break;
		}
}

