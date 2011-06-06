#include "instruction.h"
#include <stdio.h>

void print_instruction(Instruction instr, unsigned addr) {
	//printf("%x ", addr);
	switch(instr.instr_generic._cop) {
		case ILLOP:
			printf("ILLOP ");
			break;
		case NOP:
			printf("NOP ");
			break;
		case LOAD:
			printf("LOAD ");
			printf("R%d, ", instr.instr_generic._regcond);
			if (instr.instr_generic._immediate) { // I = 1 
				printf("%d", instr.instr_immediate._value);
			} else { // I = 0
				if (instr.instr_generic._indexed) { // X = 1
					printf("@%d", instr.instr_absolute._address);
				} else { // X = 0
					printf("@%d", instr.instr_absolute._address);
				}
			}
			break;
		case STORE:
			printf("STORE ");
			printf("R%d, ", instr.instr_generic._regcond);
			break;
		case ADD:
			printf("ADD ");
			printf("R%d, ", instr.instr_generic._regcond);
			break;
		case SUB:
			printf("SUB ");
			printf("R%d, ", instr.instr_generic._regcond);
			if (instr.instr_generic._immediate) { // I = 1	
				printf("#%d", instr.instr_immediate._value);				
			} else {
				if (instr.instr_generic._indexed) {
					printf("@%d", instr.instr_absolute._address);
				}
			}
			break;
		case BRANCH:
			printf("BRANCH ");
			printf("%d", instr.instr_generic._regcond);
			print_condition();
			break;
		case CALL:
			printf("CALL ");
			printf("%d", instr.instr_generic._regcond);
			print_condition();
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
