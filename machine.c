#include "machine.h"
#include "exec.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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

  if( (bits_read = read(handle, &textsize, sizeof(textsize))) != sizeof(textsize))
    perror("Erreur de lecture de 'textsize' dans <machine.c:read_program>");// dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(textsize));

  if( (bits_read = read(handle, &datasize, sizeof(datasize))) != sizeof(pmach->_datasize))
    perror("Erreur de lecture de 'datasize' dans <machine.c:read_program>");//dans '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(datasize));

  if( (bits_read = read(handle, &dataend, sizeof(dataend))) != sizeof(dataend))
    perror("Erreur de lecture de 'dataend' dans <machine.c:read_program>");// '%s': %d bits lus au lieu de %d",programfile,bits_read,sizeof(dataend));

  Instruction *text = malloc(textsize * sizeof(Instruction));
  //lecture des instructions :
  if( (read(handle, text, textsize * sizeof(Instruction))) != (textsize * sizeof(Instruction)) )
    perror("Erreur de lecture de 'text' dans <machine.c:read_program>");

  Word *data = malloc(datasize * sizeof(Word));
  //lecture des données :
  if( (read(handle, data, datasize * sizeof(Word))) != (datasize * sizeof(Word)))
    perror("Erreur de lecture de 'data' dans <machine.c:read_program>");

  //Fermeture du fichier :
  if(close(handle) != 0)
    perror("Erreur de fermeture du fichier binaire dans <machine.c:read_program>");

  //On charge le programme dans la machine :
  load_program(pmach, textsize, text, datasize, data, dataend);

  //On libère la mémoire :
  free(text);
  free(data);
}

//! Chargement d'un programme
/*!
 * La machine est réinitialisée et ses segments de texte et de données sont
 * remplacés par ceux fournis en paramètre.
 *
 * \param pmach la machine en cours d'exécution
 * \param textsize taille utile du segment de texte
 * \param text le contenu du segment de texte
 * \param datasize taille utile du segment de données
 * \param data le contenu initial du segment de texte
 */
void load_program(Machine *pmach,
                  unsigned textsize, Instruction text[textsize],
                  unsigned datasize, Word data[datasize],  unsigned dataend)
{
  //Recopie des tableaux text...
  /*pmach->_text = malloc(textsize * sizeof(Instruction));
  memcpy(pmach->_text, text, textsize*sizeof(text));
  //...et data :
  pmach->_data = malloc(datasize * sizeof(Word));
  memcpy(pmach->_data, data, datasize*sizeof(data));*/

  pmach->_text = text;
  pmach->_data = data;

  //Init de textsize..
  pmach->_textsize = textsize;
  //.. datasize..
  pmach->_datasize = datasize;
  //.. et dataend :
  pmach->_dataend = dataend;

  //Mise à zéro des registres :
  for(int i = 0 ; i < NREGISTERS ; i++)
      pmach->_registers[i] = 0;

  //Mise à zéro de PC..
  pmach->_pc = 0;
  //.. et CC :
  pmach->_cc = CC_U;

  //Init de SP ;
  pmach->_sp = datasize-1;
}

//! Affichage du programme et des données
/*!
 * On affiche les instruction et les données en format hexadécimal, sous une
 * forme prête à être coupée-collée dans le simulateur.
 *
 * Pendant qu'on y est, on produit aussi un dump binaire dans le fichier
 * dump.prog. Le format de ce fichier est compatible avec l'option -b de
 * test_simul.
 *
 * \param pmach la machine en cours d'exécution
 */
void dump_memory(Machine *pmach)
{
  int i = 0;
  
  //Affichage du programme au format binaire:
  printf("Instruction text[] = {\n");
  for(i = 0 ; i < pmach->_textsize ; i+=4)
  {
      printf("\t0x%08x, ", pmach->_text[i]._raw);
      if( (i+1) < pmach->_textsize)
	printf("0x%08x, ", pmach->_text[i+1]._raw);
      else
	putchar('\n');
      if( (i+2) < pmach->_textsize)
	printf("0x%08x, ", pmach->_text[i+2]._raw);
      else
	putchar('\n');
      if( (i+3) < pmach->_textsize)
	printf("0x%08x,\n", pmach->_text[i+3]._raw);
      else
	putchar('\n');
  }
  printf("};\n");
  printf("unsigned textsize = %d;\n", pmach->_textsize);

  printf("\nWord data[] = {\n");
  //Affichage des données au format binaire:
  for(i = 0 ; i < pmach->_datasize ; i+=4)
  {
      printf("\t0x%08x, ", pmach->_data[i]);
      if( (i+1) < pmach->_datasize)
	printf("0x%08x, ", pmach->_data[i+1]);
      else
      {
	putchar('\n');
	break;
      }
      if( (i+2) < pmach->_datasize)
	printf("0x%08x, ", pmach->_data[i+2]);
      else
      {
	putchar('\n');
	break;
      }      if( (i+3) < pmach->_datasize)
	printf("0x%08x,\n", pmach->_data[i+3]);
      else
      {
	putchar('\n');
	break;
      }
  }  
  printf("};\n");
  printf("unsigned datasize = %d;\n", pmach->_datasize);
  printf("unsigned dataend = %d;\n", pmach->_dataend);

  int bits_read=0;
  //Ouverture/creation du fichier en mode écriture seule + troncature
  int handle = open("dump.bin",O_WRONLY|O_TRUNC|O_CREAT);

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
    }

  //ecriture des données :
    for(int i = 0 ; i < pmach->_datasize ; i++)
    {
      if( (bits_read = write(handle, &pmach->_data[i], sizeof(pmach->_data[0]))) != sizeof(pmach->_data[0]))
	perror("Erreur d'écriture de 'data' dans <machine.c:dump_memory>.");
    }

  //Fermeture du fichier :
  if(close(handle) != 0)
    perror("Erreur de fermeture du fichier binaire dans <machine.c:read_program>");
}

//! Affichage des instructions du programme
/*!
 * Les instructions sont affichées sous forme symbolique, précédées de leur adresse.
.* 
 * \param pmach la machine en cours d'exécution
 */
void print_program(Machine *pmach)
{
  printf("\n*** PROGRAM (size: %d) ***\n",pmach->_textsize);
  for(int i = 0 ; i < pmach->_textsize ; i++)
    {
      printf("0x%04x: 0x%08x\t", i, pmach->_text[i]._raw);
      print_instruction(pmach->_text[i],pmach->_text[i].instr_absolute._address);
      putchar('\n');
    }
}

//! Affichage des registres du CPU
/*!
 * Les registres généraux sont affichées en format hexadécimal et décimal.
 *
 * \param pmach la machine en cours d'exécution
 */
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
  putchar('\n');
  /*for(int i = 0 ; i < NREGISTERS ; i+=3)
  {
      printf("R%02d: 0x%08x\t%d\t", i, pmach->_registers[i], pmach->_registers[i]);
      if( (i+1) < NREGISTERS)
	printf("R%02d: 0x%08x\t%d\t", i+1, pmach->_registers[i+1], pmach->_registers[i+1]);
      else
	putchar('\n');
      if( (i+2) < NREGISTERS)
	printf("R%02d: 0x%08x\t%d\n", i+2, pmach->_registers[i+2], pmach->_registers[i+2]);
      else
	putchar('\n');
  }*/
  
  for (int i = 0; i < NREGISTERS; i++) {
  	printf("R%02d: 0%08x\t%d\t", i, pmach->_registers[i], pmach->_registers[i]);
  	if (i % 3 == 2) {
  		printf("\n");
  	}
  }
  
  
  
}

//! Affichage des données du programme
/*!
 * Les valeurs sont affichées en format hexadécimal et décimal.
 *
 * \param pmach la machine en cours d'exécution
 */
void print_data(Machine *pmach)
{
  printf("\n*** DATA (size: %d, end = 0x%08x %d) ***\n",pmach->_datasize, pmach->_dataend, pmach->_dataend);
  for(int i = 0 ; i < pmach->_datasize ; i+=3)
  {
      printf("0x%04x: 0x%08x %d\t", i, pmach->_data[i], pmach->_data[i]);
      if( (i+1) < pmach->_datasize)
	printf("0x%04x: 0x%08x %d\t", i+1, pmach->_data[i+1], pmach->_data[i+1]);
      else
	putchar('\n');
      if( (i+2) < pmach->_datasize)
	printf("0x%04x: 0x%08x %d\n", i+2, pmach->_data[i+2], pmach->_data[i+2]);
      else
	putchar('\n');
  }
  putchar('\n');
}

//! Simulation
/*!
 * La boucle de simualtion est très simple : recherche de l'instruction
 * suivante (pointée par le compteur ordinal \c _pc) puis décodage et exécution
 * de l'instruction.
 *
 * \param pmach la machine en cours d'exécution
 * \param debug mode de mise au point (pas à apas) ?
 */
void simul(Machine *pmach, bool debug)
{
  //decode_execute retourne false si on est à la fin du programme :
  while (decode_execute(pmach, pmach->_text[pmach->_pc++]))
  {
    if (pmach->_pc >= pmach->_textsize) {
    	error(ERR_SEGTEXT, pmach->_pc - 1);
    }
    //On trace l'exécution courrante :
    trace("Executing", pmach, pmach->_text[pmach->_pc], pmach->_pc);
    //Si on est en mode debug on ne fait qu'une ligne a la fois
    if (debug)
	    debug = debug_ask(pmach);
  }
}
