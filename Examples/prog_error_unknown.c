//! Exemple : OPERATEUR_INEXISTANT

Instruction text[] = {
//   type		 cop	imm	ind	regcond	operand
//-------------------------------------------------------------
    {.instr_immediate = {12,	false,	false, 	0, 	0	}},  // 2
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
