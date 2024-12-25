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
|  PILE_ APPEL  | 4
|  RETURN_FCT   | 5
|  LINE_ACT ?   | 6 ??
|  ...          | 8     
                        (NB_REGISTRE)
|               | 9     STATIC GLOBAL
| ...           | ?
*/

extern FILE * out;
extern int PILE;

void codegen(ast * p);
void codegenINIT();

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