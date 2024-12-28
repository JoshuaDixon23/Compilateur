#include "ts.h"

int ts_ajouter_id(ts tabsymb, char * context, char * id, int type, ast * p, int param) {
    int i = 0;

    // Trouver la première position libre
    while (tabsymb[i].adresse != -1 && i < 128) {
        i++;
    }

    if (i >= 128) {
        fprintf(stderr, "Erreur : table des symboles pleine.\n");
        return -1;
    }

    strcpy(tabsymb[i].context, context);
    strcpy(tabsymb[i].id, id);
    tabsymb[i].type = type;
    tabsymb[i].adresse = i;
    tabsymb[i].p = p;
    tabsymb[i].param = param;
    return i;
}

int ts_recherche_param(ts tabsymb, char *context, int n) {
    int i = 0, count = 0;

    while (tabsymb[i].adresse != -1) {
        if (strcmp(tabsymb[i].context, context) == 0 && tabsymb[i].param == 1) {
            if (count == n) {
                return i; 
            }
            count++;
        }
        i++;
    }

    return -1; 
}


int ts_recherche_param(ts tabsymb, char * context, char * id) {
    int i = 0;

    while (tabsymb[i].adresse != -1) {
        // Vérification du contexte et de l'identifiant
        if (strcmp(tabsymb[i].context, context) == 0 && 
            strcmp(tabsymb[i].id, id) == 0 && tabsymb[i].param == 1){
            return i;
        }
        i++;
    }

    return -1; // Non trouvé
}



void PrintTS(ts tabsymb){
    int i = 0;
    printf("\n");
    while(tabsymb[i].adresse != -1 && i < 128){
        printf("Context : %s\n",tabsymb[i].context);
        printf("Id : %s\n",tabsymb[i].id);
        printf("Adresse : %d\n",tabsymb[i].adresse);
        printf("Type : %d\n",tabsymb[i].type);
        printf("Valeur : %d\n",tabsymb[i].valeur);
        printf("Param : %d\n\n",tabsymb[i].param);
        i++;
    }
}
