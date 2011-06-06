#include "machine.h"
#include "exec.h"
#include "debug.h"
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
	// mettre PC a 0 ou pas ?
	//pmach->_pc = 0;  
}

/**
 * Fonction d'affichage des registres
 * */
void print_cpu(Machine *pmach);
{
   int i;
   printf("CPU:\n\n");
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
   printf("SP : 0x%x %d\n",pmach->_sp, pmach->_sp);
}

void read_program(Machine *mach, const char *programfile)
{
  
}

void dump_memory(Machine *pmach)
{
  int i;
  for(i =0; i < pmach->_textsize; i++)
    {
      printf("%x\n", pmach->_text[i]->_raw);
    }
  for(i =0; i < pmach->_datasize; i++)
    {
      printf("%x\n", pmach->_data[i]);
    }
  // TODO mettre les memes données dans le fichier demandé
}


void print_program(Machine *pmach){
  int i;
  printf("Instructions:\n");
  for(i =0; i < pmach->_textsize; i++)
    {
      printf("0x%x : 0x%x   ", i, pmach->_text[i]->instr_absolute->_address);
      print_instruction(pmach->_text[i],pmach->_text[i]->instr_absolute->_address);
      printf("\n");
    }
}


void print_data(Machine *pmach)
{
  int i;
  printf("Data:\n");
  for(i =0; i < pmach->_datasize; i++)
    {
      printf("0x%x : 0x%x %d \n", i, pmach->_data[i], pmach->_data[i]);
    }
}

void simul(Machine *pmach, bool debug)
{
  bool debugActive = debug;
  while (pmach->_pc < pmach->_textsize){
	if (debugActive)
		debugActive = debug_ask(Machine *pmach);
	decode_execute(pmach, pmach->_text[pmach->_pc]);
	pmach->_pc++;	
  }
}
