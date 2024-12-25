#include "codegen.h"

extern ts TABSYMB;
int ligne_act = 0;
extern char CTXT[32];

// Prototypes des fonctions spécifiques
static void codegenNB(ast * p);
static void codegenOP(ast * p);
static void codegenID(ast * p);
static void codegenAFF(ast * p);
static void codegenLEXP(ast * p);
static void codegenTQ(ast * p);
static void CodegenSI(ast *p);
static void codegenFonction(ast * p);
static void codegenCondition(ast * p);

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
        case AST_SI:
            CodegenSI(p);
            break;
        case AST_FONCTION:
            codegenFonction(p);
            break;
        case AST_CONDITION:
            codegenCondition(p);
            break;
        default:
            fprintf(stderr, "Type AST inconnu : %d\n", p->type);
            break;
    }
}

void codegenINIT() {
    int nbVars = 0;
    for (int i = 0; i < 128; i++) {
        if (TABSYMB[i].id[0] != '\0') {
            nbVars++;
        }
    }

    fprintf(out, "Initialisation de la pile\n\n");
    fprintf(out, "LOAD #%d\n", nbVars + NB_REGISTRE);
    fprintf(out, "STORE 3\n");    
}


static void codegenNB(ast * p) {
    fprintf(out, "LOAD #%d\n", p->valeur);
    EMPILER();
    ligne_act = ligne_act + 3;
}

static void codegenOP(ast * p) {
    codegen(p->suivant[0]);
    codegen(p->suivant[1]);
    DEPILER();
    fprintf(out, "DEC 3 \n");
    switch (p->op) {
        case '+':
            fprintf(out, "ADD ");
            break;
        case '-':
            fprintf(out, "SUB ");
            break;
        case '*':
            fprintf(out, "MUL ");
            break;
        case '/':
            fprintf(out, "DIV ");
            break;
        default:
            fprintf(stderr, "Opérateur inconnu : %c\n", p->op);
            break;
    }
    fprintf(out, "@3 \n");
    EMPILER();
    ligne_act = ligne_act + 6;
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
    ligne_act = ligne_act + 3;
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
    ligne_act = ligne_act + 3;
}

static void codegenLEXP(ast * p) {
    codegen(p->suivant[0]);
    if (p->suivant[1]) {
        codegen(p->suivant[1]);
    }
}

static void codegenTQ(ast * p) {
    int debut_tq = ligne_act;
    fprintf(out,"Debut TQ :\n\n");
    codegen(p->suivant[0]); // Condition
    fprintf(out,"corps TQ :\n\n");
    codegen(p->suivant[1]); // Corps de la boucle
    fprintf(out, "JUMP %d\n", debut_tq); 
    ligne_act = ligne_act + 1;
}

static void CodegenSI(ast *p) {
    int adresseSinon = ligne_act + 1;
    int adresseFin = ligne_act + 1;

    codegen(p->suivant[0]);
    DEPILER();
    fprintf(out, "JUMZ %d\n", adresseSinon);
    ligne_act++;

    codegen(p->suivant[1]);
    fprintf(out, "JUMP %d\n", adresseFin);
    ligne_act++;

    adresseSinon = ligne_act;
    if (p->suivant[2]) {
        codegen(p->suivant[2]);
    }
    adresseFin = ligne_act;
}


static void codegenFonction(ast * p) {
    // a reprendre 
    fprintf(out, "FUNC %s:\n", p->id);
    codegen(p->suivant[0]); // Paramètres ou déclarations locales
    codegen(p->suivant[1]); // Corps de la fonction
    fprintf(out, "END_FUNC\n");
    ligne_act = ligne_act + 2;
}

static void codegenCondition(ast * p){
    fprintf(out,"Condition gauche :\n\n");
    codegen(p->suivant[0]);
    fprintf(out,"Condition droite :\n\n");
    codegen(p->suivant[1]);
    // ex : 1 = 2
    DEPILER();                 // 2
    fprintf(out, "DEC 3 \n");
    fprintf(out, "SUB @3\n");  // 2 - 1 = 1
    switch (p->op) {
            case '<': 
                fprintf(out, "JUMG %d\n", p->codelen+ligne_act+6);  // Saut si ACC > 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            case '>': 
                fprintf(out, "JUML %d\n", p->codelen+ligne_act+6);  // Saut si ACC < 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            case '=': 
                fprintf(out, "JUMG %d\n", p->codelen+ligne_act+6);  // Saut si ACC > 0
                fprintf(out, "JUML %d\n", p->codelen+ligne_act+6);  // Saut si ACC < 0
                break;
            case '!': 
                fprintf(out, "JUMZ %d\n", p->codelen+ligne_act+6);  // Saut si ACC == 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            default:
                fprintf(stderr, "Opérateur inconnu : %c\n", p->op);
                break;
        }
    ligne_act = ligne_act + 6;
}