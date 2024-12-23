#include <stdio.h>

#include "codegen.h"

FILE * out;
ts TABSYMB;

int main(void){
    out = stdout;
    INIT_TS(TABSYMB);
    ast *p1 = CreerFeuilleID("a");
    ts_ajouter_id(TABSYMB, "GLOBAL", "a");
    ast *p2 = CreerFeuilleNB(12);
    ast *p3 = CreerNoeudOP('+', p1, p2);
    PrintAst(p3);
    PrintTS(TABSYMB);
    codegen(p3);
    return 0;
}