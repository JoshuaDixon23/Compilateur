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
            p->codelen = p->suivant[0]->codelen+ p->suivant[1]->codelen + 4; // value a changer
            break;
        default:
            p->codelen = 1;
            break;
    }
}