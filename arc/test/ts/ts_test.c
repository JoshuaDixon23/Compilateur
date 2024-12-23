#include "ts.h"

int main(){
    ts tabsymb;
    INIT_TS(tabsymb);
    ts_ajouter_id(tabsymb, "GLOBAL","TESTLA");
    ts_ajouter_id(tabsymb, "GLOBAL","TESTLA2");
    ts_ajouter_id(tabsymb, "GLOBAL","TESTLA3");
    int i = ts_recherche_id(tabsymb,"GLOBAL","TESTLA2" );
    PrintTS(tabsymb);
    printf("%d", i);
    return 0;
}
