#include "exec.h"
#include "error.h"
#include <stdio.h>

/**
* Met a jour cc selon la valeur de reg
**/
void refresh_cc(Machine *pmach,unsigned int reg) {
	if(reg < 0)
	        pmach->_cc = CC_N;
    	else if(reg > 0)
        	pmach->_cc = CC_P;
    	else
        	pmach->_cc = CC_Z;
}

/**
* Verifie le numero de registre
**/
void check_register(Instruction instr,unsigned addr) {
	if (instr.instr_generic._regcond<0 || instr.instr_generic._regcond>14)
		error(ERR_ILLEGAL,addr);
}

/**
*
**/
void check_immediate(Instruction instr,unsigned addr) {
	if (instr.instr_generic._immediate)
		error(ERR_IMMEDIATE,addr);
}

/**
*
**/
unsigned int get_addr(Machine *pmach,Instruction instr) {
	return pmach->_registers[instr.instr_indexed._rindex] + instr.instr_indexed._offset;
}

/**
* On vérifie que l'on a pas une erreur de segmentation sur data
**/
void check_data_addr(Machine *pmach,unsigned int data_addr,unsigned addr) {
	if (data_addr > pmach->_datasize)
		error(ERR_SEGDATA,addr);
}


/**
* Decode et execute la fonction LOAD
**/
bool load(Machine *pmach, Instruction instr,unsigned addr) {
	check_register(instr,addr);
	if (instr.instr_generic._immediate) { // Si I = 1 : Immediat
		pmach->_registers[instr.instr_generic._regcond]=instr.instr_immediate._value;
	} else {				
		if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
			unsigned int addr_data = get_addr(pmach,instr);
			check_data_addr(pmach,addr_data,addr);
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[addr_data];	
		} else { // Si I = 0 et X = 0 : Adressage direct
			check_data_addr(pmach,instr.instr_absolute._address,addr);
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[instr.instr_absolute._address];
		}
	}
	refresh_cc(pmach,pmach->_registers[instr.instr_generic._regcond]);
	return true;
}

/**
* Decode et execute la fonction STORE
**/
bool store(Machine *pmach, Instruction instr,unsigned addr) {
	check_immediate(instr,addr);	
	check_register(instr,addr);	
	if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
		unsigned int addr_data = get_addr(pmach,instr);
		check_data_addr(pmach,addr_data,addr);
		pmach->_data[addr_data]=pmach->_registers[instr.instr_generic._regcond];
	} else { // Si I = 0 et X = 0 : Adressage direct
		check_data_addr(pmach,instr.instr_absolute._address,addr);
		pmach->_data[instr.instr_absolute._address]=pmach->_registers[instr.instr_generic._regcond];
	}
	return true;
}

/**
* Decode et execute la fonction ADD
**/
bool add(Machine *pmach, Instruction instr,unsigned addr) {
	check_register(instr,addr);
	if (instr.instr_generic._immediate) { // Si I = 1 : Immediat
		pmach->_registers[instr.instr_generic._regcond]=instr.instr_immediate._value;
	} else {				
		if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
			unsigned int addr_data = get_addr(pmach,instr);
			check_data_addr(pmach,addr_data,addr);
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[addr_data];	
		} else { // Si I = 0 et X = 0 : Adressage direct
			check_data_addr(pmach,instr.instr_absolute._address,addr);
			pmach->_registers[instr.instr_generic._regcond]=pmach->_data[instr.instr_absolute._address];
		}
	}
	return true;
}


bool decode_execute(Machine *pmach, Instruction instr) {
	unsigned addr = pmach->_pc-1;
	switch (instr.instr_generic._cop) {
		case LOAD:
		//trace("TRACE: Executing",pmach,instr,addr);
		return load(pmach,instr,addr);
		case STORE:
		return store(pmach,instr,addr);
		default:
		error(ERR_UNKNOWN,addr);
	}
}

void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("TRACE: %s: 0x%04x: ",msg,addr);
	print_instruction(instr,addr);
	printf("\n");
}
