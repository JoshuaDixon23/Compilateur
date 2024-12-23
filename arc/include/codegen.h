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
|  ...          | 8     
                        (NB_REGISTRE)
|               | 9     STATIC GLOBAL
| ...           | ?
*/

extern FILE * out;
extern int PILE;

void codegen(ast * p);

#define EMPILER(){                     \
    fprintf(out, "STORE @%d\n", 3); \
    fprintf(out, "INC %d\n", 3);   \
}

#define ADR_SOMMET_PILE(){ \
    fprintf(out, "STORE @%d\n", 3); \
}

#define DEPILER(){       \
    fprintf(out, "DEC %d\n", 3);   \
    fprintf(out, "STORE @%d\n", 3); \
}     

#endif