#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void error(Error err, unsigned addr) __attribute__((noreturn)){
		switch (err) {
		case  ERR_NOERROR:
			printf("%d: No error",addr);
			break;
		case ERR_UNKNOWN:
			printf("%d: Unknown instruction",addr);
			exit(0);
			break;
		case ERR_ILLEGAL:
			printf("%d: Illegal instruction",addr);
			exit(0);
			break;
		case ERR_IMMEDIATE:
			printf("%d: Immediate value forbidden",addr);
			exit(0);
			break;
		case ERR_SEGTEXT:
			printf("%d: Text index out of bounds",addr);
			exit(0);
			break;
		case ERR_SEGDATA:
			printf("%d: Data index out of bounds",addr);
			exit(0);
			break;
		case ERR_SEGSTACK:
			printf("%d: Stack index out of bounds",addr);
			exit(0);
			break;
	}
}


void error(Error err, unsigned addr){
	switch (err) {
		case  ERR_NOERROR:
			printf("%d: No error",addr);
			break;
		case ERR_UNKNOWN:
			printf("%d: Unknown instruction",addr);
			exit(0);
			break;
		case ERR_ILLEGAL:
			printf("%d: Illegal instruction",addr);
			exit(0);
			break;
		case ERR_IMMEDIATE:
			printf("%d: Immediate value forbidden",addr);
			exit(0);
			break;
		case ERR_SEGTEXT:
			printf("%d: Text index out of bounds",addr);
			exit(0);
			break;
		case ERR_SEGDATA:
			printf("%d: Data index out of bounds",addr);
			exit(0);
			break;
		case ERR_SEGSTACK:
			printf("%d: Stack index out of bounds",addr);
			exit(0);
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

