#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_register(Instruction instr) {
	if ((int) instr.instr_generic._regcond < 10) {
		printf("R0%d, ", (int) instr.instr_generic._regcond);
	} else {
		printf("R%d, ", (int) instr.instr_generic._regcond);
	}
}

void print_condition(Instruction instr) {
	switch (instr.instr_generic._regcond) {
		case NC:
			printf("NC ");
			break;
		case EQ:
			printf("EQ ");
			break;
		case NE:
			printf("NE ");
			break;
		case GT:
			printf("GT ");
			break;
		case GE:
			printf("GE ");
			break;
		case LT:
			printf("LT ");
			break;
		case LE:
			printf("LE ");
			break;
	}
}

void print_instruction(Instruction instr, unsigned addr) {
	switch (instr.instr_generic._cop) {
		case ILLOP:
			printf("ILLOP ");
			break;
		case NOP:
			printf("NOP ");
			break;
		case LOAD:
			printf("LOAD ");
			print_register(instr);
			if (instr.instr_generic._immediate) { // I = 1 
				printf("%d", instr.instr_immediate._value);
			} else { // I = 0
				if (instr.instr_generic._indexed) { // X = 1
					printf("@%d", (int) instr.instr_absolute._address);
				} else { // X = 0
					printf("@%d", (int) instr.instr_absolute._address);
				}
			}
			break;
		case STORE:
			printf("STORE ");
			print_register(instr);
			break;
		case ADD:
			printf("ADD ");
			print_register(instr);
			break;
		case SUB:
			printf("SUB ");
			print_register(instr);
			if (instr.instr_generic._immediate) { // I = 1	
				printf("#%d", instr.instr_immediate._value);				
			} else {
				if (instr.instr_generic._indexed) { // X = 1
					printf("@%d", (int) instr.instr_absolute._address);
				} else { // X = 0
					printf("@%d", (int) instr.instr_absolute._address);
				}
			}
			break;
		case BRANCH:
			printf("BRANCH ");
			print_condition(instr);
			break;
		case CALL:
			printf("CALL ");
			print_condition(instr);
			break;
		case RET:
			printf("RET ");
			break;
		case PUSH:
			printf("PUSH ");
			break;
		case POP:
			printf("POP ");
			break;
		case HALT:
			printf("HALF ");
			break;
	}
}
