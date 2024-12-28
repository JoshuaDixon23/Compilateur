#ifndef TS_H
#define TS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INIT_TS(tab){		      \
    for(int i = 0; i<128; i++){    \
        tab[i].context[0] = '\0';   \
        tab[i].id[0] = '\0';         \
        tab[i].adresse = -1;          \
        tab[i].type = -1;              \
        tab[i].valeur = 0;              \
    }			                         \
  };	
// type 0 : integer
// type 1 : char (pour plus tard...)
// type 2 : fonction	

struct ts_cellule{
    char context[32];
    char id[32];
    int adresse;
    int type;
    int valeur;
};
typedef struct ts_cellule ts[128];
int ts_ajouter_id(ts tabsymb, char * context, char * id, int type);
int ts_ajouter_id(ts tabsymb, char * context, char * id, int type);
void PrintTS(ts tabsymb);

#endif

