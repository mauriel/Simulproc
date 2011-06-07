#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* cop_names[] = { "ILLOP", "NOP", "LOAD", "STORE", "ADD", "SUB", "BRANCH", "CALL", "RET", "PUSH", "POP", "HALT" };
const char* condition_names[] = { "NC", "EQ", "NE", "GT", "GE", "LT", "LE" };

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
	printf("%s ", condition_names[instr.instr_generic._regcond]);
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
		case NOP:
		case RET:
		case HALT:
			printf("%s ",cop_names[instr.instr_generic._cop]); 
			break;
		case LOAD:
		case STORE:
		case ADD:
		case SUB:
			printf("%s ", cop_names[instr.instr_generic._cop]);
			print_register(instr);
			print_op(instr);
			break;
		case BRANCH:
		case CALL:
			printf("%s ", cop_names[instr.instr_generic._cop]);
			print_condition(instr);
			print_op(instr);
			break;
		case PUSH:
		case POP:
			printf("%s ", cop_names[instr.instr_generic._cop]);
			print_op(instr);
			break;
	}
}
