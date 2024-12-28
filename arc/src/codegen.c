#include "codegen.h"

extern ts TABSYMB;
int ligne_act = 0;
int temp = 0;
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
static void codegenAPPEL(ast * p);

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
        case AST_APPEL:
            codegenAPPEL(p);
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
    fprintf(out, "LOAD #%d\n", nbVars + NB_REGISTRE - 1);
    fprintf(out, "STORE 3\n");
    ligne_act = 2;
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
    ligne_act = ligne_act + 7;
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
    temp = p->suivant[1]->codelen;
    fprintf(out,"Debut TQ :\n\n");
    codegen(p->suivant[0]); // Condition
    fprintf(out,"corps TQ :\n\n");
    codegen(p->suivant[1]); // Corps de la boucle
    fprintf(out, "JUMP %d\n", debut_tq); 
    ligne_act = ligne_act + 1;
    fprintf(out, "%d\n", ligne_act);
}

static void CodegenSI(ast *p) {
    temp = p->suivant[1]->codelen;
    codegen(p->suivant[0]);

    codegen(p->suivant[1]);
    fprintf(out, "JUMP %d\n", ligne_act + p->suivant[2]->codelen);
    ligne_act++;
    if (p->suivant[2]) {
        codegen(p->suivant[2]);
    }

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
    //fprintf(out, "%d\n", ligne_act);
    //fprintf(out, "%d\n", p->codelen);
    DEPILER();                 // 2
    fprintf(out, "DEC 3 \n");
    fprintf(out, "SUB @3\n");  // 2 - 1 = 1
    switch (p->op) {
            case '<': 
                fprintf(out, "JUMG %d\n", 6+ligne_act+temp);  // Saut si ACC > 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            case '>': 
                fprintf(out, "JUML %d\n", 6+ligne_act+temp);  // Saut si ACC < 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            case '=': 
                fprintf(out, "JUMG %d\n", 6+ligne_act+temp);  // Saut si ACC > 0
                fprintf(out, "JUML %d\n", 6+ligne_act+temp);  // Saut si ACC < 0
                break;
            case '!': 
                fprintf(out, "JUMZ %d\n", 6+ligne_act+temp);  // Saut si ACC == 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            default:
                fprintf(stderr, "Opérateur inconnu : %c\n", p->op);
                break;
        }
    ligne_act = ligne_act + 6;
}

static void codegenAPPEL(ast * p){
    //ts tab_temp;
    //INIT_TS(tab_temp);
    //tab_temp = TABSYMB;
    fprintf(out, "LOAD 3\n");
    fprintf(out, "STORE 4\n"); // store la valeur de la pile avant l'appel

    int index_fonction = ts_recherche_id(TABSYMB,"GLOBAL",p->id);
    int count = 0;
    ast *param = p->suivant[0]; 
    while (param != NULL) {
        ast *param_id = param->suivant[0]; 
        if (param_id->type == AST_ID) {
            char *nom_id = param_id->id;
            int index_global = ts_recherche_id(TABSYMB, CTXT, nom_id);
            printf("Valeur de %s : %d\n", nom_id, index);
            if(index == -1){
                fprintf(stderr, "Erreur : Variable impossible : %s\n",nom_id);
            }
            // l'adresse de la variable de parametre de la fonction 
            // soit a la meme adresse que la variable d appel
            int index_local =ts_recherche_param(TABSYMB, TABSYMB[index_fonction].id,count);
            fprintf(out, "LOAD %d\n", index_global + 9); // met la valeur de index_global dans index_local
            fprintf(out, "STORE %d\n", index_local + 9);
            
        } else {
            fprintf(stderr, "Erreur : type inattendu dans L_ID.type : %s\n", param_id->type_str);
        }
        param = param->suivant[1]; // Passe au prochain noeud de L_EXP
        count++;
    }
    


    codegen(TABSYMB[index_fonction].p);
    // remettre tout a l'etat d'origine
}