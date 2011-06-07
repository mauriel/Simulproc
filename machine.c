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
//  printf("Entree dans read_program\n");
  int bits_read;
  unsigned textsize;
  unsigned datasize;
  unsigned dataend;
  //Ouverture du fichier :
  int handle = open(programfile,O_RDONLY);
  if(handle < 0)
    perror("Erreur d'ouverture du fichier binaire dans <machine.c:read_program>");
/*  else
    printf("Ouverture réussie, fd = %d\n",handle);
*/
  if( (bits_read = read(handle, &textsize, sizeof(textsize))) != sizeof(textsize))
    perror("Erreur de lecture de 'textsize' dans <machine.c:read_program>");// dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(textsize));
/*  else
    printf("Lecture de textsize réussie: %d\n",textsize);
*/
  if( (bits_read = read(handle, &datasize, sizeof(datasize))) != sizeof(pmach->_datasize))
    perror("Erreur de lecture de 'datasize' dans <machine.c:read_program>");//dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(datasize));
/*  else
    printf("Lecture de datasize réussie: %d\n",datasize);
*/
  if( (bits_read = read(handle, &dataend, sizeof(dataend))) != sizeof(dataend))
    perror("Erreur de lecture de 'dataend' dans <machine.c:read_program>");// '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(dataend));
/*  else
    printf("Lecture de dataend réussie: %08x\n",dataend);
*/
  Instruction *text = malloc(textsize * sizeof(Instruction));
  //lecture des instructions :
  printf("Lecture de text:\n");
    for(int i = 0 ; i < textsize ; i++)
    {
      if( (bits_read = read(handle, &text[i], sizeof(text[0]))) != sizeof(text[0]))
	perror("Erreur de lecture de 'text' dans <machine.c:read_program>");
      else
	printf("text[%03d]: 0x%08x\n", i,text[i]);
    }

  Word *data = malloc(datasize * sizeof(Word));
  //lecture des données :
  printf("\nLecture de data:\n");
  for(int i = 0 ; i < datasize ; i++)
    {
      if( (bits_read = read(handle, &data[i], sizeof(data[0]))) != sizeof(data[0]))
	perror("Erreur de lecture de 'data' dans <machine.c:read_program>");
      else
      printf("data[%03d]: 0x%08x\n", i, data[i]);
    }

  //Fermeture du fichier :
  if(close(handle) != 0)
    perror("Erreur de fermeture du fichier binaire dans <machine.c:read_program>");

  load_program(pmach, textsize, text, datasize, data, dataend);
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

  //Mise à zéro des registres :
  for(int i = 0 ; i < NREGISTERS ; i++)
      pmach->_registers[i] = 0;
  
  //Init de SP ;
  pmach->_sp = datasize-1;
  //Mise à zéro de PC et CC :
  pmach->_pc = 0;
  pmach->_cc = CC_U;
}

void dump_memory(Machine *pmach)
{
  int i;

  //Affichage du programme au format binaire:
  for(i = 0 ; i < pmach->_textsize ; i++)
      printf("%08x\n", pmach->_text[i]._raw);

  //Affichage des données au format binaire:
  for(i = 0 ; i < pmach->_datasize ; i++)
      printf("%08x\n", pmach->_data[i]);

  int bits_read=0;
  //Ouverture du fichier en mode écriture seule + troncature
  int handle = open("dump.prog",O_WRONLY|O_TRUNC|O_CREAT);

  if( (bits_read = write(handle, &pmach->_textsize, sizeof(pmach->_textsize))) != sizeof(pmach->_textsize))
    perror("Erreur d'écriture de 'textsize' dans <machine.c:dump_memory>");// dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(textsize));
  
  if( (bits_read = write(handle, &pmach->_datasize, sizeof(pmach->_datasize))) != sizeof(pmach->_datasize))
    perror("Erreur d'écriture de 'datasize' dans <machine.c:dump_memory>");//dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(datasize));
  
  if( (bits_read = write(handle, &pmach->_dataend, sizeof(pmach->_dataend))) != sizeof(pmach->_dataend))
    perror("Erreur d'écriture de 'dataend' dans <machine.c:dump_memory>");// '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(dataend));

  //ecriture des instructions :
    for(int i = 0 ; i < pmach->_textsize ; i++)
    {
      if( (bits_read = write(handle, &pmach->_text[i]._raw, sizeof(pmach->_text[0]._raw))) != sizeof(pmach->_text[0]._raw))
	perror("Erreur d'écriture de 'instruc' dans <machine.c:dump_memory>.");
      else
      printf("instruc[%03d]: 0x%08x\n", i, pmach->_text[i]._raw);
    }

  //ecriture des données :
    for(int i = 0 ; i < pmach->_datasize ; i++)
    {
      if( (bits_read = write(handle, &pmach->_data[i], sizeof(pmach->_data[0]))) != sizeof(pmach->_data[0]))
	perror("Erreur d'écriture de 'data' dans <machine.c:dump_memory>.");
      else
      printf("data[%03d]: 0x%08x\n", i, pmach->_data[i]);
    }

  //Fermeture du fichier :
  if(close(handle) != 0)
    perror("Erreur de fermeture du fichier binaire dans <machine.c:read_program>");
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
  printf("\n*** DATA (size: %d) ***\n",pmach->_datasize);
  for(int i = 0 ; i < pmach->_datasize ; i++)
      printf("0x%04x: 0x%08x %d\n", i, pmach->_data[i], pmach->_data[i]);
}

void simul(Machine *pmach, bool debug)
{
  bool ret = true;
  while (ret)
  {
    printf("TRACE: Executing:");
    print_instruction(pmach->_text[pmach->_pc],pmach->_text[pmach->_pc].instr_absolute._address);
    putchar('\n');
    ret = decode_execute(pmach, pmach->_text[pmach->_pc++]);
    if (debug)
	    debug = debug_ask(pmach);
  }
}
