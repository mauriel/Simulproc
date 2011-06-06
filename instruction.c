#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Affiche le registre.
 */
void print_register(Instruction instr) {
	printf("R%02d, ", (int) instr.instr_generic._regcond);
}

/*
 * Affiche le code condition.
 */
void print_condition(Instruction instr) {
	switch (instr.instr_generic._regcond) {
		case NC:
			printf("NC "); break;
		case EQ:
			printf("EQ "); break;
		case NE:
			printf("NE "); break;
		case GT:
			printf("GT "); break;
		case GE:
			printf("GE "); break;
		case LT:
			printf("LT "); break;
		case LE:
			printf("LE "); break;
	}
}

/*
 * Affiche les opérandes.
 */
void print_op(Instruction instr) {
	if (instr.instr_generic._immediate) { // Si I = 1 : Immediat
		printf("#%d", instr.instr_immediate._value);
	} else {				
		if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
			printf("%+d[", (int) instr.instr_indexed._offset); // Offset sous la forme +/-offset
			printf("R%02d]", (int) instr.instr_indexed._rindex); // Registre pour l'adressage indirect [R..]
		} else { // Si I = 0 et X = 0 : Adressage direct
			printf("@%04x", (int) instr.instr_absolute._address);	
		}
	}
}

void print_instruction(Instruction instr, unsigned addr) {
	switch (instr.instr_generic._cop) {
		case ILLOP:
			//cop_names[ILLOP] = "ILLOP";
			//printf("%s ", cop_names[0]);
			printf("ILLOP "); 
			break;
		case NOP:
			//cop_names[NOP] = "NOP";
			//printf("%s ", cop_names[NOP]);
			printf("NOP "); 
			break;
		case LOAD:
			//cop_names[LOAD] = "LOAD";
			//printf("%s ", cop_names[LOAD]);
			printf("LOAD ");
			print_register(instr);
			print_op(instr);
			break;
		case STORE:
			//cop_names[STORE] = "STORE";
			//printf("%s ", cop_names[STORE]);
			printf("STORE ");
			print_register(instr);
			print_op(instr);
			break;
		case ADD:
			printf("ADD ");
			//cop_names[ADD] = "ADD";
			//printf("%s ", cop_names[ADD]);
			print_register(instr);
			print_op(instr);
			break;
		case SUB:
			printf("SUB ");
			//cop_names[SUB] = "SUB";
			//printf("%s ", cop_names[SUB]);
			print_register(instr);
			print_op(instr);
			break;
		case BRANCH:
			printf("BRANCH ");
			//cop_names[BRANCH] = "BRANCH";
			//printf("%s ", cop_names[BRANCH]);
			print_condition(instr);
			print_op(instr);
			break;
		case CALL:
			printf("CALL ");
			//cop_names[CALL] = "CALL";
			//printf("%s ", cop_names[CALL]);
			print_condition(instr);
			print_op(instr);
			break;
		case RET:
			printf("RET ");
			//cop_names[RET] = "RET";
			//printf("%s ", cop_names[RET]);
			break;
		case PUSH:
			printf("PUSH ");
			//cop_names[PUSH] = "PUSH";
			//printf("%s ", cop_names[PUSH]);
			print_op(instr);
			break;
		case POP:
			printf("POP ");
			//cop_names[POP] = "POP";
			//printf("%s ", cop_names[POP]);
			print_op(instr);
			break;
		case HALT:
			printf("HALT ");
			//cop_names[HALT] = "HALT";
			//printf("%s ", cop_names[HALT]);
			break;
	}
}
