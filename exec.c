#include "exec.h"

bool decode_execute(Machine *pmach, Instruction instr) {
	return false;
}

void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("TRACE: Executing: %d", (int) addr);
}
