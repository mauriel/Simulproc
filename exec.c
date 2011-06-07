#include "exec.h"
#include "error.h"
#include <stdio.h>

bool load(Machine *pmach, Instruction instr,unsigned addr) {
//	if (instr.instr_generic._regcond==NULL)
//		error(ERR_CONDITION,addr);
	printf("\n--%d--\n",addr);
	if (instr.instr_generic._regcond<0 || instr.instr_generic._regcond>14)
		error(ERR_ILLEGAL,addr);
	if (instr.instr_generic._immediate) { // Si I = 1 : Immediat
		pmach->_registers[instr.instr_generic._regcond]=instr.instr_immediate._value;
	} else {				
		if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
			int addr_data;
			if ((addr_data = pmach->_registers[instr.instr_indexed._rindex] + instr.instr_indexed._offset) < pmach->_datasize)
				error(ERR_SEGDATA,addr);
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[addr_data];	
		} else { // Si I = 0 et X = 0 : Adressage direct
			if (instr.instr_absolute._address < pmach->_datasize)
				error(ERR_SEGDATA,addr);
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[instr.instr_absolute._address];
		}
	}
	return true;
}




bool decode_execute(Machine *pmach, Instruction instr) {
	unsigned addr = pmach->_pc;
	switch (instr.instr_generic._cop) {
		case LOAD:
		//trace("TRACE: Executing",pmach,instr,addr);
		printf("\n");
		return load(pmach,instr,addr);
		default:
		error(ERR_UNKNOWN,addr);
	}
}

void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("%s: 0x%04x: ",msg,addr);
	print_instruction(instr,addr);
}
