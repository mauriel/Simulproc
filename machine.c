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

/**
 * Fonction d'affichage des registres
 * */
void print_cpu(Machine *pmach);
{
   int i;

   printf("PC : 0x%hu\n",pmach->_pc);

   printf("CC : ")
   switch(pmach->_cc){
	case CC_U :
		putchar('U'); break;
	case CC_Z :
		putchar('Z'); break;
	case CC_P :
		putchar('P'); break;
	case CC_N :
		putchar('N'); break;

   }
  putchar('\n');
  
	
  for(i=0;i<=13;i++){
      printf("R%d : 0x%x %d\n",i,pmach->_registers[i],pmach->_registers[i]);
    }
   printf("SP : %x %d\n",pmach->_sp, pmach->_sp);
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

void simul(Machine *pmach, bool debug)
{
  
}
