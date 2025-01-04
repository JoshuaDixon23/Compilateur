#ifndef CODEGEN_H
#define CODEGEN_H

#include "ts.h"
#include "ast.h"


#define NB_REGISTRE 9
/*
### PILE : ####

|  ACC          | 0

|  TMP          | 1
|  REG          | 2
|  PILE         | 3     REGISTRES
|  PILE_APPEL   | 4
|               | 5
|               | 6 
|               | 8     

|               | 9     VARIABLES
| ...           | 10
*/

extern FILE * out;
extern int PILE;

void codegen(ast * p);
void codegenINIT();
void codegenEND();

#define EMPILER(){                    \
    fprintf(out, "STORE @%d\n", 3);  /* Sauvegarde ACC au sommet de la pile */ \
    fprintf(out, "INC %d\n", 3);    /* Incrémente le sommet de la pile */     \
}


#define ADR_SOMMET_PILE(){ \
    fprintf(out, "STORE @%d\n", 3); \
}

#define DEPILER(){                    \
    fprintf(out, "DEC %d\n", 3);    /* Décrémente le sommet de la pile */     \
    fprintf(out, "LOAD @%d\n", 3);  /* Charge la valeur du sommet dans ACC */ \
}
   

#endif