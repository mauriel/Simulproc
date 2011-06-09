//! Dans cet exemple, on voit qu'aucune erreur concernant le numéro de registre ne peut être levée.
//! En effet, si le programme voit une valeur supérieure à 4 bits, il ne considère que les 4 bits les plus faibles.
//! Ainsi, si on indique R17, le programme l'interprétera comme R01.
Instruction text[] = {
//   type		 cop	imm	ind	regcond	operand
//-------------------------------------------------------------
    {.instr_immediate = {LOAD,	true,	false,	0,	2	}},
    {.instr_immediate = {LOAD,	true,	false,	0,	5	}},
    {.instr_indexed   = {ADD,	false,	true,	0,	17,	+2	}},
    {.instr_generic   = {HALT }}
};

//! Taille utile du programme
const unsigned textsize = sizeof(text) / sizeof(Instruction);

//! Premier exemple de segment de données initial
Word data[20] = {
    10, // 0: premier opérande
    5,  // 1: second opérande
    20, // 2: résultat
    0,  // 3
};

//! Fin de la zone de données utiles
const unsigned dataend = 5;

//! Taille utile du segment de données
const unsigned datasize = sizeof(data) / sizeof(Word);  
