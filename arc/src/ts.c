#include "ts.h"

int ts_recherche_id(ts tabsymb, char * context, char * id){
    int i = 0;
    while(tabsymb[i].adresse != -1){
        if(strcmp(tabsymb[i].context, context) == 0 && strcmp(tabsymb[i].id, id) == 0){
            return i;
        }
        i++;
    }
    return -1;
}
int ts_ajouter_id(ts tabsymb, char * context, char * id){
    int i = 0;
    while(tabsymb[i].adresse != -1 && i < 128){
        i++;
    }
    strcpy(tabsymb[i].context, context);
    strcpy(tabsymb[i].id , id);
    tabsymb[i].adresse = i;
    return 0;
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
