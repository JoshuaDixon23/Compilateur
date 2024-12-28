#include "ts.h"

int ts_ajouter_id(ts tabsymb, char * context, char * id, char * type) {
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
    strcpy(tabsymb[i].type, type);
    tabsymb[i].adresse = i;

    return i;
}
int ts_recherche_id(ts tabsymb, char * context, char * id, char * type) {
    int i = 0;

    while (tabsymb[i].adresse != -1) {
        // Vérification du contexte, de l'identifiant, et éventuellement du type
        if (strcmp(tabsymb[i].context, context) == 0 && 
            strcmp(tabsymb[i].id, id) == 0 &&
            strcmp(tabsymb[i].type, type) == 0) {
            return i;
        }
        i++;
    }

    return -1; // Non trouvé
}


void PrintTS(ts tabsymb){
    int i = 0;
    while(tabsymb[i].adresse != -1 && i < 128){
        printf("Context : %s\n",tabsymb[i].context);
        printf("Id : %s\n",tabsymb[i].id);
        printf("Adresse : %d\n",tabsymb[i].adresse);
        printf("Type : %d\n",tabsymb[i].type);
        printf("Valeur : %d\n\n",tabsymb[i].valeur);
        i++;
    }
}
