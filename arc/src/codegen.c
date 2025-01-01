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
            break;
        default:
            fprintf(stderr, "Type AST inconnu : %d\n", p->type);
            fprintf(stderr, "Adresse AST : %p\n", (void *)p);
            fprintf(stderr, "Codelen : %d\n", p->codelen);
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

    out = fopen("a.out", "w"); // Ouvrir le fichier a.out en mode écriture
    if (out == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier a.out\n");
        return;
    }

    //fprintf(out, "Initialisation de la pile\n\n");
    //fprintf(out, "%d %d\n", nbVars, NB_REGISTRE);
    fprintf(out, "LOAD #%d\n", nbVars + NB_REGISTRE);
    fprintf(out, "STORE 3\n");
    ligne_act = 2;
}

void codegenEND() {
    fprintf(out, "NOP\n");
    fclose(out);
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
    //fprintf(out, "var : %s\n", TABSYMB[index].id); 
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
    //fprintf(out,"Debut TQ :\n\n");
    codegen(p->suivant[0]); // Condition
    //fprintf(out,"corps TQ :\n\n");
    codegen(p->suivant[1]); // Corps de la boucle
    fprintf(out, "JUMP %d\n", debut_tq); 
    ligne_act = ligne_act + 1;
}

static void CodegenSI(ast *p) {
    temp = p->suivant[1]->codelen;
    codegen(p->suivant[0]);

    codegen(p->suivant[1]);
    fprintf(out, "JUMP %d\n", ligne_act + p->suivant[2]->codelen + 1);
    ligne_act = ligne_act + 1;
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

    codegen(p->suivant[0]);
    codegen(p->suivant[1]);
    
    // ex : 1 < 2
    DEPILER();                 // 2
    fprintf(out, "DEC 3 \n");
    fprintf(out, "SUB @3\n");  // 2 - 1 = 1
    switch (p->op) {
            case '<': 
                //fprintf(out,"%d %d\n", ligne_act+4, temp);
                fprintf(out, "JUML %d\n", 7+ligne_act+temp);  // Saut si ACC < 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            case '>': 
                fprintf(out, "JUMG %d\n", 7+ligne_act+temp);  // Saut si ACC > 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            case '=': 
                fprintf(out, "JUMG %d\n", 7+ligne_act+temp);  // Saut si ACC > 0
                fprintf(out, "JUML %d\n", 7+ligne_act+temp);  // Saut si ACC < 0
                break;
            case '!': 
                fprintf(out, "JUMZ %d\n", 7+ligne_act+temp);  // Saut si ACC == 0
                fprintf(out, "NOP\n"); // Pour compenser le = 
                break;
            default:
                fprintf(stderr, "Opérateur inconnu : %c\n", p->op);
                break;
        }
    ligne_act = ligne_act + 6;
}

static void codegenAPPEL(ast *p) {
    // SAUVEGARDE //
    // sauvegarde de la ts
    ts tabsymb_backup;       
    memcpy(tabsymb_backup, TABSYMB, sizeof(ts));

    // Sauvegarde de la pile avant l'appel
    fprintf(out, "LOAD 3\n");
    fprintf(out, "STORE 4\n");
    ligne_act = ligne_act + 2;

    // Sauvegarde du contexte actuel
    char context_debut[100];
    strcpy(context_debut, CTXT);

    // --------- //

    // Recherche de la fonction dans la table des symboles
    int index_fonction = ts_recherche_id(TABSYMB, CTXT, p->id);
    if (index_fonction == -1) {
        fprintf(stderr, "Erreur : Fonction %s introuvable dans le contexte %s\n", p->id, CTXT);
        exit(EXIT_FAILURE);
    }

    // Stocker les paramètres dans une liste
    ast *params[100]; // 100 paramètres max
    int param_count = 0;
    ast *param = p->suivant[0];
    while (param != NULL) {
        params[param_count++] = param;
        param = param->suivant[1];
    }

    // Parcourir les paramètres dans l'ordre inverse
    int count = 0;
    for (int i = param_count - 1; i >= 0; i--) {
        param = params[i];
        ast *param_id = param->suivant[0];
        if (param_id->type == AST_ID) {
            char *nom_id = param_id->id;
            int index_global = ts_recherche_id(TABSYMB, CTXT, nom_id);
            if (index_global == -1) {
                fprintf(stderr, "Erreur : Variable inconnue %s dans le contexte %s\n", nom_id, CTXT);
                exit(EXIT_FAILURE);
            }
            
            int index_local = ts_recherche_param(TABSYMB, TABSYMB[index_fonction].id, count);
            printf("index_local : %d\n", index_local);
            if (index_local == -1) {
                fprintf(stderr, "Erreur : Paramètre %d introuvable dans la fonction %s\n", count, TABSYMB[index_fonction].id);
                exit(EXIT_FAILURE);
            }
            printf("index_local : %s\n", TABSYMB[index_local].id);
            printf("index_global : %s\n", TABSYMB[index_global].id);
            int index_final = ts_ajouter_id(TABSYMB, TABSYMB[index_fonction].id, TABSYMB[index_local].id, 0 ,NULL, 0);
            TABSYMB[index_final].adresse = TABSYMB[index_global].adresse;
        } else {
            fprintf(stderr, "Erreur : type inattendu\n");
            exit(EXIT_FAILURE);
        }
        count++;
    }

    // Mise à jour du contexte pour exécuter la fonction
    strcpy(CTXT, TABSYMB[index_fonction].id);

    // Génère le code pour le corps de la fonction
    codegen(TABSYMB[index_fonction].p);

    // Restaure le contexte initial
    strcpy(CTXT, context_debut); 

    // Restauration de la table des symboles
    memcpy(TABSYMB, tabsymb_backup, sizeof(ts)); 
    strcpy(CTXT, context_debut); 

    // Restauration de la pile
    fprintf(out, "LOAD 4\n");
    fprintf(out, "STORE 3\n"); 
    ligne_act = ligne_act + 2;
}
