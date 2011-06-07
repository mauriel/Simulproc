#include "machine.h"
#include "exec.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//! Lecture d'un programme depuis un fichier binaire
/*!
 * Le fichier binaire a le format suivant :
 * 
 *    - 3 entiers non signés, la taille du segment de texte (\c textsize),
 *    celle du segment de données (\c datasize) et la première adresse libre de
 *    données (\c dataend) ;
 *
 *    - une suite de \c textsize entiers non signés représentant le contenu du
 *    segment de texte (les instructions) ;
 *
 *    - une suite de \c datasize entiers non signés représentant le contenu initial du
 *    segment de données.
 *
 * Tous les entiers font 32 bits et les adresses de chaque segment commencent à
 * 0. La fonction initialise complétement la machine.
 *
 * \param pmach la machine à simuler
 * \param programfile le nom du fichier binaire
 *
 */
void read_program(Machine *pmach, const char *programfile)
{
  int bits_read;
  unsigned textsize;
  Instruction text[textsize];
  unsigned datasize;
  Word data[datasize];
  unsigned dataend;
  //Ouverture du fichier :
  int handle = open(programfile,O_RDONLY);

  if( (bits_read = read(handle, &textsize, sizeof(textsize))) != sizeof(textsize))
    perror("Erreur de lecture de 'textsize' dans <machine.c:read_program>");// dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(textsize));
  
  if( (bits_read = read(handle, &datasize, sizeof(datasize))) != sizeof(pmach->_datasize))
    perror("Erreur de lecture de 'datasize' dans <machine.c:read_program>");//dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(datasize));
  
  if( (bits_read = read(handle, &dataend, sizeof(dataend))) != sizeof(dataend))
    perror("Erreur de lecture de 'dataend' dans <machine.c:read_program>");// '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(dataend));

  //lecture des instructions :
    for(int i = 0 ; i < textsize ; i++)
    {
      if( (bits_read = read(handle, &text[i], sizeof(text[textsize]))) != sizeof(text[textsize]))
	perror("Erreur de lecture de 'text' dans <machine.c:read_program>");
      else
      printf("data[%03d]: 0x%08x\n", i, pmach->_text[i]._raw);
    }

  //lecture des données :
    for(int i = 0 ; i < datasize ; i++)
    {
      if( (bits_read = read(handle, &data[i], sizeof(data[datasize]))) != sizeof(data[datasize]))
	perror("Erreur de lecture de 'data' dans <machine.c:read_program>");
      else
      printf("data[%03d]: 0x%08x\n", i, pmach->_data[i]);
    }
    
    load_program(pmach,textsize, text[textsize], datasize, data[datasize], dataend)
}

void load_program(Machine *pmach,
                  unsigned textsize, Instruction text[textsize],
                  unsigned datasize, Word data[datasize],  unsigned dataend)
{
	pmach->_text = text;
	pmach->_textsize = textsize;
	pmach->_data = data;
	pmach->_datasize = datasize;
	pmach->_dataend = dataend;

  for(int i = 0 ; i < NREGISTERS ; i++)
      pmach->_registers[i] = 0;
}

void dump_memory(Machine *pmach)
{
  int i;

  //Affichage du programme au format binaire:
  for(i = 0 ; i < pmach->_textsize ; i++)
      printf("%x\n", pmach->_text[i]._raw);

  //Affichage des données au format binaire:
  for(i = 0 ; i < pmach->_datasize ; i++)
      printf("%x\n", pmach->_data[i]);

  int bits_read=0;
  //Ouverture du fichier en mode écriture seule + troncature
  int handle = open("dump.prog",O_WRONLY|O_TRUNC);

  if( (bits_read = write(handle, &pmach->_textsize, sizeof(pmach->_textsize))) != sizeof(pmach->_textsize))
    perror("Erreur d'écriture de 'textsize' dans <machine.c:dump_memory>");// dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(textsize));
  
  if( (bits_read = write(handle, &pmach->_datasize, sizeof(pmach->_datasize))) != sizeof(pmach->_datasize))
    perror("Erreur d'écriture de 'datasize' dans <machine.c:dump_memory>");//dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(datasize));
  
  if( (bits_read = write(handle, &pmach->_dataend, sizeof(pmach->_dataend))) != sizeof(pmach->_dataend))
    perror("Erreur d'écriture de 'dataend' dans <machine.c:dump_memory>");// '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(dataend));
/*
  //ecriture des instructions :
  unsigned int instruc[textsize];
    for(int i = 0 ; i < textsize ; i++)
    {
      if( (bits_read = write(handle, &instruc[i], sizeof(instruc[textsize]))) != sizeof(instruc[textsize]))
	perror("Erreur d'écriture de 'instruc' dans <machine.c:dump_memory>.");
      else
      printf("instruc[%03d]: 0x%08x\n", i, instruc[i]);
    }

  //ecriture des données :
  unsigned int data[datasize];
    for(int i = 0 ; i < datasize ; i++)
    {
      if( (bits_read = write(handle, &data[i], sizeof(data[datasize]))) != sizeof(data[datasize]))
	perror("Erreur d'écriture de 'data' dans <machine.c:dump_memory>.");
      else
      printf("data[%03d]: 0x%08x\n", i, data[i]);
    }
  */
}

void print_program(Machine *pmach)
{
  printf("\n*** PROGRAM (size: %d) ***\n",pmach->_textsize);
  for(int i = 0 ; i < pmach->_textsize ; i++)
    {
      printf("0x%04x: 0x%08x\t", i, pmach->_text[i].instr_absolute._address);
      print_instruction(pmach->_text[i],pmach->_text[i].instr_absolute._address);
      putchar('\n');
    }
}


void print_cpu(Machine *pmach)
{
   printf("\n*** CPU ***\n");
   printf("PC: 0x%08x\tCC: ",pmach->_pc);
   switch(pmach->_cc){
	case CC_U :
		putchar('U'); break;
	case CC_Z :
		putchar('Z'); break;
	case CC_P :
		putchar('P'); break;
	case CC_N :
		putchar('N'); break;
   };
  putchar('\n');
  for(int i = 0 ; i < NREGISTERS ; i++)
      printf("R%02d: 0x%08x\t%d\n",i,pmach->_registers[i],pmach->_registers[i]);
}

void print_data(Machine *pmach)
{
  printf("\n*** DATA (size: %d, end = 0x%08x (%d)) ***\n",pmach->_datasize,pmach->_datasize-1,pmach->_datasize-1);
  for(int i = 0 ; i < pmach->_datasize ; i++)
      printf("0x%04x: 0x%08x %d\n", i, pmach->_data[i], pmach->_data[i]);
}

void simul(Machine *pmach, bool debug)
{
  while (pmach->_pc < pmach->_textsize){
	if (debug)
		debug = debug_ask(pmach);
	decode_execute(pmach, pmach->_text[pmach->_pc]);
	pmach->_pc++;
  }
}