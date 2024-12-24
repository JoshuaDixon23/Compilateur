#include "semantic.h"

void semantic(ast * p){
    switch(p->type){
        case AST_NB:
            p-> codelen = 2;
            break;
        case AST_ID:
            p-> codelen = 2;
            break;
        case AST_OP:
            semantic(p->suivant[0]);
            semantic(p->suivant[1]);
            p->codelen = p->suivant[0]->codelen + p->suivant[1]->codelen + 6;
            break;
        case AST_LEXP:
            semantic(p->suivant[0]);
            if(p->suivant[1]){
                semantic(p->suivant[1]);
            }   
            break;
        case AST_AFF:
            semantic(p->suivant[0]);
            p->codelen = p->suivant[0]->codelen + 3;
            break;
        case AST_FONCTION:
            // a revoir
            semantic(p->suivant[0]);
            semantic(p->suivant[1]);
            break;
        case AST_TQ:
            semantic(p->suivant[0]);
            semantic(p->suivant[1]);
            p->codelen = p->suivant[0]->codelen + p->suivant[1]->codelen + 1; 
            break;
        case AST_CONDITION:
            semantic(p->suivant[0]);
            semantic(p->suivant[1]);
            p->codelen = p->suivant[0]->codelen+ p->suivant[1]->codelen + 6; 
            break;
        default:
            p->codelen = 1;
            break;
    }
}