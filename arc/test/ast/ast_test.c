#include "ast.h"

int main(void){
  ast * p1 = CreerFeuilleNB(1);
  ast * p2 = CreerFeuilleNB(2);
    
  PrintAst(p1);
  PrintAst(p2);

  FreeAst(p1);
  FreeAst(p2);

  return 0;
}


