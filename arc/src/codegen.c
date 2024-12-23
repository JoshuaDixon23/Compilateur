#include "codegen.h"

extern ts TABSYMB;
int PILE = 1;
extern char CTXT[32];
static void codegenNB(ast * p);

void codegen(ast * p){
    switch(p->type){
        case AST_NB:
            codegenNB(p);
            break;
        case AST_OP:
            codegenOP(p);
            break;
        case AST_ID:
            codegenID(p);
            break;
        case AST_AFF:
            codegenAFF(p);
            break;
        default:
            // gestion d'erreur exit(1)
        break;
    }
}

void codegenNB(ast * p){
    fprintf(out,"LOAD #%d\n", p->valeur );
}

void codegenOP(ast * p){
    codegen(p->suivant[0]);
    codegen(p->suivant[1]);
    DEPILER();
    switch(p->op){
        case '+':
            fprintf(out,"ADD ");
            break;
        default:
            // gestion nonetype op 
            break;
    }
    ADR_SOMMET_PILE();
    fprintf(out,"LOAD #%s\n", "value a determiner");
}

void codegenID(ast * p){
    int adr, i;
    i = ts_recherche_id(TABSYMB, CTXT, p->id); // si i pas trouver alors p->id pas initialiser
    adr = TABSYMB[i].adresse + NB_REGISTRE;
    fprintf(out,"i: %d adr : %d\n", i, adr);
    fprintf(out, "LOAD %d\n", adr);
    EMPILER();
}

void codegenAFF(ast * p){
    codegen(p->suivant[0]); // Exp dans la pile
    DEPILER();
    int i = ts_recherche_id(TABSYMB, CTXT, p->id);
    fprintf(out,"STORE %d\n", TABSYMB[i].adresse + NB_REGISTRE);
    EMPILER();
}