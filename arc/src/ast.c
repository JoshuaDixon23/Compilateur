#include "ast.h"

static void PrintNB(ast *p, char * indent);
static void PrintOP(ast *p, char *indent);
static void PrintID(ast *p, char *indent);
static void PrintLEXP(ast *p, char *indent);
static void PrintAFF(ast *p, char *indent);
static void PrintTQ(ast *p, char *indent);
static void PrintSI(ast *p, char *indent);
static void PrintFONCTION(ast *p, char *indent);
static void PrintCONDITION(ast *p, char *indent);
static void PrintLFONCTION(ast *p, char *indent);
static void PrintAPPEL(ast *p, char *indent);
int profondeur = 0;

ast * CreerFeuilleNB(int nb){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_NB;
  strcpy(p->type_str,"NB");
  p->valeur = nb;
  return p;
}

ast * CreerNoeudOP(int operateur, ast * p1, ast * p2){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_OP;
  strcpy(p->type_str,"OP");
  p->op = operateur;
  p->suivant[0] = p1;
  p->suivant[1] = p2;
  return p;
}

ast * CreerNoeudLEXP(ast * p1, ast * p2){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_LEXP;
  strcpy(p->type_str,"LEXP");
  p->suivant[0] = p1;
  p->suivant[1] = p2;
  return p;
}

ast * CreerFeuilleID(char * id){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_ID;
  strcpy(p->type_str,"ID");
  strcpy(p->id, id);
  return p;
}

ast * CreerNoeudFonction(char * id, ast * p1, ast * p2){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_FONCTION;
  strcpy(p->type_str,"FONCTION");
  strcpy(p->id, id);
  p->suivant[0] = p1;
  p->suivant[1] = p2;
  return p;
}

ast * CreerNoeudTQ(ast * p1, ast * p2){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_TQ;
  strcpy(p->type_str,"TQ");
  p->suivant[0] = p1;
  p->suivant[1] = p2;
  return p;
}

ast * CreerNoeudSI(ast *condition, ast *alors, ast *sinon) {
    ast *p;
    INIT_NOEUD(p);
    p->type = AST_SI;
    strcpy(p->type_str, "SI");
    p->suivant[0] = condition;
    p->suivant[1] = alors;
    if (sinon) {
      p->suivant[2] = sinon;
    }
    else {
      p->suivant[2] = NULL;
    }
    return p;
}

ast * CreerNoeudAFF(char * id, ast * p1){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_AFF;
  strcpy(p->type_str,"AFF");
  strcpy(p->id, id);
  p->suivant[0] = p1;
  return p;
}
ast * CreerNoeudCondition(int operateur, ast * p1, ast * p2){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_CONDITION;
  strcpy(p->type_str,"CONDITION");
  p->op = operateur;
  p->suivant[0] = p1;
  p->suivant[1] = p2;
  return p;
}
ast * CreerNoeudLFonction(ast * p1, ast * p2){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_LFONCTION;
  strcpy(p->type_str,"LFONCTION");
  p->suivant[0] = p1;
  p->suivant[1] = p2;
  return p;
}
ast * CreerNoeudAppel(char* id, ast * p1){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_APPEL;
  strcpy(p->type_str,"APPEL");
  strcpy(p->id, id);
  p->suivant[0] = p1;
  return p;
}

void FreeAst(ast * p){
  if (p == NULL) return;
  free(p);
}

void PrintAst(ast * p){
  if (p == NULL) return;
  char indent[32] ="";
  int i = 0;
  for(i = 0; i < profondeur; i++){
    indent[i]='\t';
  }
  indent[i] = '\0';
  switch(p->type){
  case AST_NB:
    PrintNB(p,indent);
    break;
  case AST_OP:
    PrintOP(p, indent);
    break;
  case AST_LEXP:
    PrintLEXP(p, indent);
    break;
  case AST_AFF:
    PrintAFF(p, indent);
    break;
  case AST_ID:
    PrintID(p, indent);
    break;
  case AST_FONCTION:
    PrintFONCTION(p, indent);
    break;
  case AST_LFONCTION:
    PrintLFONCTION(p,indent);
    break;
  case AST_TQ:
    PrintTQ(p, indent);
    break;
  case AST_SI:
    PrintSI(p, indent); 
    break;
  case AST_CONDITION:
    PrintCONDITION(p, indent);
    break;
  case AST_APPEL:
    PrintAPPEL(p, indent);
    break;
  default:
    fprintf(stderr,"[Erreur] type <%d>: %s non reconnu\n",p->type,p->type_str);
    break;
  }
}

void ErrorAst(const char * errmsg){
  fprintf(stderr,"[AST error] %s\n",errmsg);
  exit(1);
}

static void PrintNB(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_GREEN "Feuille:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "Valeur: " TXT_NULL "%d\n",indent, p->valeur);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
}
static void PrintID(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_GREEN "Feuille:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "ID: " TXT_NULL "%s\n",indent, p->id);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
}
static void PrintOP(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "Operateur: " TXT_NULL "%d\n",indent, p->op);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  PrintAst(p->suivant[1]);
  profondeur--;
}
static void PrintAFF(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "ID:   " TXT_NULL "%s\n",indent, p->id);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  profondeur--;
}
static void PrintLEXP(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_RED "LEXP:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  PrintAst(p->suivant[1]);
  profondeur--;
}
static void PrintLFONCTION(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_RED "LFONCTION:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  PrintAst(p->suivant[1]);
  profondeur--;
}
static void PrintTQ(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_BLUE "TQ:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  PrintAst(p->suivant[1]);
  profondeur--;
}
static void PrintSI(ast *p, char *indent) {
    printf("%s" TXT_BOLD TXT_BLUE "SI:  " TXT_NULL "%p\n", indent, p);
    printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n", indent, p->type_str);
    printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n", indent, p->codelen);
    profondeur++;
    printf("%sCONDITION:\n", indent);
    PrintAst(p->suivant[0]);
    printf(TXT_BOLD TXT_BLUE "%sALORS:\n", indent);
    PrintAst(p->suivant[1]);
    if (p->suivant[2]) {
        printf(TXT_BOLD TXT_BLUE "%sSINON:\n", indent);
        PrintAst(p->suivant[2]);
    }
    profondeur--;
}
static void PrintCONDITION(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_BLUE "CONDITION:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "OP:   " TXT_NULL "%c\n",indent, p->op);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  PrintAst(p->suivant[1]);
  profondeur--;
}
static void PrintFONCTION(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_RED "FONCTION:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "ID:   " TXT_NULL "%s\n",indent, p->id);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  PrintAst(p->suivant[1]);
  profondeur--;
}
static void PrintAPPEL(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_RED "APPEL:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "ID:   " TXT_NULL "%s\n",indent, p->id);
  printf("%s" TXT_BOLD "Codelen: " TXT_NULL "%d\n",indent, p->codelen);
  profondeur++;
  PrintAst(p->suivant[0]);
  profondeur--;
}