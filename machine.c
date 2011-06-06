#include "machine.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void load_program(Machine *pmach,
                  unsigned textsize, Instruction text[textsize],
                  unsigned datasize, Word data[datasize],  unsigned dataend){
	pmach->_text = text;
	pmach->_textsize = textsize;
	pmach->_data = datas;
	pmach->_datasize = datasize;
	pmach->_dataend = dataend;
}

void read_program(Machine *mach, const char *programfile)
{
  
}

void dump_memory(Machine *pmach)
{
  
}


void print_program(Machine *pmach)
{
  
}


void print_data(Machine *pmach)
{
  
}


void print_cpu(Machine *pmach)
{
  
}


void simul(Machine *pmach, bool debug)
{
  
}