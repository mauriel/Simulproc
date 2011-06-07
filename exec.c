#include "exec.h"

bool decode_execute(Machine *pmach, Instruction instr) {
	
	
	
	return false;
}

void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("%s: 0x%04x: ",msg,addr);
	print_instruction(instr);
}
