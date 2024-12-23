#include "codegen.h"

extern ts TABSYMB;
int PILE = 1;
extern char CTXT[32];

// Prototypes des fonctions spécifiques
static void codegenNB(ast * p);
static void codegenOP(ast * p);
static void codegenID(ast * p);
static void codegenAFF(ast * p);
static void codegenLEXP(ast * p);
static void codegenTQ(ast * p);
static void codegenFonction(ast * p);

void codegen(ast * p) {
    switch (p->type) {
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
        case AST_LEXP:
            codegenLEXP(p);
            break;
        case AST_TQ:
            codegenTQ(p);
            break;
        case AST_FONCTION:
            codegenFonction(p);
            break;
        default:
            fprintf(stderr, "Type AST inconnu : %d\n", p->type);
            break;
    }
}

static void codegenNB(ast * p) {
    fprintf(out, "LOAD #%d\n", p->valeur);
}

static void codegenOP(ast * p) {
    codegen(p->suivant[0]);
    codegen(p->suivant[1]);
    DEPILER();
    switch (p->op) {
        case '+':
            fprintf(out, "ADD\n");
            break;
        case '-':
            fprintf(out, "SUB\n");
            break;
        case '*':
            fprintf(out, "MUL\n");
            break;
        case '/':
            fprintf(out, "DIV\n");
            break;
        default:
            fprintf(stderr, "Opérateur inconnu : %c\n", p->op);
            break;
    }
    EMPILER();
}

static void codegenID(ast * p) {
    int index = ts_recherche_id(TABSYMB, CTXT, p->id);
    if (index < 0) {
        fprintf(stderr, "Erreur : Identifiant '%s' non trouvé dans le contexte '%s'\n", p->id, CTXT);
        return;
    }
    int adresse = TABSYMB[index].adresse + NB_REGISTRE;
    fprintf(out, "LOAD %d\n", adresse);
    EMPILER();
}

static void codegenAFF(ast * p) {
    codegen(p->suivant[0]); // Génère le code pour l'expression assignée
    DEPILER();
    int index = ts_recherche_id(TABSYMB, CTXT, p->id);
    if (index < 0) {
        fprintf(stderr, "Erreur : Identifiant '%s' non trouvé dans le contexte '%s'\n", p->id, CTXT);
        return;
    }
    fprintf(out, "STORE %d\n", TABSYMB[index].adresse + NB_REGISTRE);
}

static void codegenLEXP(ast * p) {
    codegen(p->suivant[0]);
    if (p->suivant[1]) {
        codegen(p->suivant[1]);
    }
}

static void codegenTQ(ast * p) {
    // a reprendre 
    codegen(p->suivant[0]); // Condition
    fprintf(out, "JUMP %d\n", 1);
    codegen(p->suivant[1]); // Corps de la boucle
    fprintf(out, "JUMP LABEL%d\n", 2);
    fprintf(out, "LABEL%d:\n", 2);
    //
}

static void codegenFonction(ast * p) {
    fprintf(out, "FUNC %s:\n", p->id);
    codegen(p->suivant[0]); // Paramètres ou déclarations locales
    codegen(p->suivant[1]); // Corps de la fonction
    fprintf(out, "END_FUNC\n");
}
