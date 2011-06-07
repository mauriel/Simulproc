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
	if (instr.instr_generic._regcond<0 || instr.instr_generic._regcond>NREGISTERS-2)
		error(ERR_ILLEGAL,addr);
}

/**
* Verifie que sp ne dépasse pas la pile
**/
void check_stack(Machine *pmach,unsigned addr) {
	if (pmach->_sp < 0 || pmach->_sp > pmach->_datasize)
		error(ERR_SEGSTACK,addr);
}


/**
* Verifie que l'instruction n'est pas immediate
**/
void check_immediate(Instruction instr,unsigned addr) {
	if (instr.instr_generic._immediate)
		error(ERR_IMMEDIATE,addr);
}

/**
* Verifie qu'il n'y a pas d'erreurs sur les conditions (condition inconnue)
**/
void check_condition(Instruction instr,unsigned addr) {
	if (instr.instr_generic._regcond<0 || instr.instr_generic._regcond>7)
		error(ERR_CONDITION,addr);
}

/**
* Verifie si la condition imposee est respectee
**/
bool allowed_condition(Machine *pmach,Instruction instr) {
	switch (instr.instr_generic._regcond) {
		case NC: //Pas de condition
		return true;
		case EQ: //Egal à 0
		return (pmach->_cc == CC_Z);
		case NE: //Different de zero
		return (pmach->_cc != CC_Z);
		case GT: //Strictement positif
		return (pmach->_cc == CC_P);
		case GE: //Positif ou nul
		return (pmach->_cc == CC_P || pmach->_cc == CC_Z);
		case LT: //Strictement négatif
		return (pmach->_cc == CC_N);
		default: //Négatif ou nul
		return (pmach->_cc == CC_N || pmach->_cc == CC_Z);
	}
}


/**
* Permet de récupérer l'adresse pour un registre indexe avec offset
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
		pmach->_registers[instr.instr_generic._regcond]+=instr.instr_immediate._value;
	} else {				
		if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
			unsigned int addr_data = get_addr(pmach,instr);
			check_data_addr(pmach,addr_data,addr);
			pmach->_registers[instr.instr_generic._regcond]+=pmach->_data[addr_data];	
		} else { // Si I = 0 et X = 0 : Adressage direct
			check_data_addr(pmach,instr.instr_absolute._address,addr);
			pmach->_registers[instr.instr_generic._regcond]+=pmach->_data[instr.instr_absolute._address];
		}
	}
	refresh_cc(pmach,pmach->_registers[instr.instr_generic._regcond]);
	return true;
}

/**
* Decode et execute la fonction SUB
**/
bool sub(Machine *pmach, Instruction instr,unsigned addr) {
	check_register(instr,addr);
	if (instr.instr_generic._immediate) { // Si I = 1 : Immediat
		pmach->_registers[instr.instr_generic._regcond]-=instr.instr_immediate._value;
	} else {				
		if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
			unsigned int addr_data = get_addr(pmach,instr);
			check_data_addr(pmach,addr_data,addr);
			pmach->_registers[instr.instr_generic._regcond]-=pmach->_data[addr_data];	
		} else { // Si I = 0 et X = 0 : Adressage direct
			check_data_addr(pmach,instr.instr_absolute._address,addr);
			pmach->_registers[instr.instr_generic._regcond]-=pmach->_data[instr.instr_absolute._address];
		}
	}
	refresh_cc(pmach,pmach->_registers[instr.instr_generic._regcond]);
	return true;
}

/**
* Decode et execute la fonction BRANCH
**/
bool branch(Machine *pmach, Instruction instr,unsigned addr) {
	check_immediate(instr,addr);	
	check_condition(instr,addr);	
	if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
		unsigned int addr_data = get_addr(pmach,instr);
		check_data_addr(pmach,addr_data,addr);
		if (allowed_condition(pmach,instr))
		pmach->_pc=addr_data;
	} else { // Si I = 0 et X = 0 : Adressage direct
		check_data_addr(pmach,instr.instr_absolute._address,addr);
		if (allowed_condition(pmach,instr))
		pmach->_pc=instr.instr_absolute._address;
	}
	return true;
}

/**
* Decode et execute la fonction CALL
**/
bool call(Machine *pmach, Instruction instr,unsigned addr) {
	check_immediate(instr,addr);	
	check_condition(instr,addr);	
	if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
		unsigned int addr_data = get_addr(pmach,instr);
		check_data_addr(pmach,addr_data,addr);
		if (allowed_condition(pmach,instr)) {
			pmach->_data[pmach->_sp--]=pmach->_pc;
			pmach->_pc=addr_data;
		}
	} else { // Si I = 0 et X = 0 : Adressage direct
		check_data_addr(pmach,instr.instr_absolute._address,addr);
		if (allowed_condition(pmach,instr)) {
			pmach->_data[pmach->_sp--]=pmach->_pc;
			pmach->_pc=instr.instr_absolute._address;
		}
	}
	return true;
}

/**
* Decode et execute la fonction RET
**/
bool ret(Machine *pmach,Instruction instr,unsigned addr) {
	check_stack(pmach,addr);
	pmach->_pc=pmach->_data[--pmach->_sp];
	return true;
}

/**
* Decode et execute la fonction PUSH
**/
bool push(Machine *pmach, Instruction instr,unsigned addr) {
	if (instr.instr_generic._immediate) { // Si I = 1 : Immediat
		pmach->_data[pmach->_sp--]=instr.instr_immediate._value;
	} else {				
		if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
			unsigned int addr_data = get_addr(pmach,instr);
			check_data_addr(pmach,addr_data,addr);
			pmach->_data[pmach->_sp--]=pmach->_data[addr_data];	
		} else { // Si I = 0 et X = 0 : Adressage direct
			check_data_addr(pmach,instr.instr_absolute._address,addr);
			pmach->_data[pmach->_sp--]=pmach->_data[instr.instr_absolute._address];
		}
	}
	return true;
}

/**
* Decode et execute la fonction POP
**/
bool pop(Machine *pmach, Instruction instr,unsigned addr) {
	check_immediate(instr,addr);				
	if (instr.instr_generic._indexed) { // Si I = 0 et X = 1 : Adressage indexe
		unsigned int addr_data = get_addr(pmach,instr);
		check_data_addr(pmach,addr_data,addr);
		pmach->_data[addr_data]=pmach->_data[++pmach->_sp];	
	} else { // Si I = 0 et X = 0 : Adressage direct
		check_data_addr(pmach,instr.instr_absolute._address,addr);
		pmach->_data[instr.instr_absolute._address]=pmach->_data[++pmach->_sp];
	}
	return true;
}

/**
* Decode et execute une instruction
**/
bool decode_execute(Machine *pmach, Instruction instr) {
	unsigned addr = pmach->_pc-1;
	switch (instr.instr_generic._cop) {
		case LOAD:
		return load(pmach,instr,addr);
		case STORE:
		return store(pmach,instr,addr);
		case ADD:
		return add(pmach,instr,addr);
		case SUB:
		return sub(pmach,instr,addr);
		case BRANCH:
		return branch(pmach,instr,addr);
		case CALL:
		return call(pmach,instr,addr);
		case RET:
		return ret(pmach,instr,addr);
		case PUSH:
		return push(pmach,instr,addr);
		case POP:
		return pop(pmach,instr,addr);
		case HALT:
		warning(WARN_HALT,addr);
		return false;
		case NOP:
		return true;
		case ILLOP:
		error(ERR_ILLEGAL,addr);
		default:
		error(ERR_UNKNOWN,addr);
	}
}

/**
* Affiche la trace d'une instruction
**/
void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("TRACE: %s: 0x%04x: ",msg,addr);
	print_instruction(instr,addr);
	printf("\n");
}
