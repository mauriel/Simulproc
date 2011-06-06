#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void print_op(Instruction instr) {
	if (instr.instr_generic._immediate) { // I = 1, immediat
		printf("#%d", instr.instr_immediate._value);
	} else { // I = 0				
		if (instr.instr_generic._indexed) { // X = 1, adressage indexe
			if (instr.instr_indexed._rindex < 0) {
				printf("-");
			} else {
				printf("+");
			}
			printf("%d[", (int) instr.instr_indexed._offset);
			if ((int) instr.instr_generic._regcond < 10) {
				printf("R0%d], ", (int) instr.instr_indexed._rindex);
			} else {
				printf("R%d], ", (int) instr.instr_indexed._rindex);
			}
		} else { // X = 0, adressage direct
			printf("@%d", (int) instr.instr_absolute._address);	
		}
	}
}

void print_instruction(Instruction instr, unsigned addr) {
	switch (instr.instr_generic._cop) {
		case ILLOP:
			cop_names[ILLOP] = "ILLOP";
			printf("%s ", cop_names[ILLOP]);
			break;
		case NOP:
			cop_names[NOP] = "NOP";
			printf("%s ", cop_names[NOP]);
			break;
		case LOAD:
			cop_names[LOAD] = "LOAD";
			printf("%s ", cop_names[LOAD]);
			print_register(instr);
			print_op(instr);
			break;
		case STORE:
			cop_names[STORE] = "STORE";
			printf("%s ", cop_names[STORE]);
			print_register(instr);
			break;
		case ADD:
			cop_names[ADD] = "ADD";
			printf("%s ", cop_names[ADD]);
			print_register(instr);
			break;
		case SUB:
			cop_names[SUB] = "SUB";
			printf("%s ", cop_names[SUB]);
			print_register(instr);
			print_op(instr);
			break;
		case BRANCH:
			cop_names[BRANCH] = "BRANCH";
			printf("%s ", cop_names[BRANCH]);
			print_condition(instr);
			break;
		case CALL:
			cop_names[CALL] = "CALL";
			printf("%s ", cop_names[CALL]);
			print_condition(instr);
			break;
		case RET:
			cop_names[RET] = "RET";
			printf("%s ", cop_names[RET]);
			break;
		case PUSH:
			cop_names[PUSH] = "PUSH";
			printf("%s ", cop_names[PUSH]);
			break;
		case POP:
			cop_names[POP] = "POP";
			printf("%s ", cop_names[POP]);
			break;
		case HALT:
			cop_names[HALT] = "HALT";
			printf("%s ", cop_names[HALT]);
			break;
	}
}
