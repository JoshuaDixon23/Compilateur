#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  TXT_RED    "\x1b[31m"
#define  TXT_GREEN  "\x1b[32m"
#define  TXT_BLUE   "\x1b[34m"
#define  TXT_BOLD   "\x1b[1m"
#define  TXT_NULL   "\x1b[0m"

#define INIT_NOEUD(p)   if ((p = malloc(sizeof(ast))) == NULL)	\
    ErrorAst("echec allocation mémoire");			\
  else {							     \
    p->type = 0;						\
    p->type_str[0] = '\0';   \
    p->valeur = 0;	          \
    p->op = 0;                 \
    p->suivant[0] = NULL;       \
    p->suivant[1] = NULL;			   \
    p->id[0] = '\0';              \
    p->codelen = 0 ;               \
  }								                  \

enum {AST_NB = 256, AST_OP, AST_LEXP, AST_ID, AST_AFF, AST_TQ, AST_FONCTION, AST_CONDITION} ;

typedef struct ast{
  int  type;
  char type_str[32];
  int valeur;
  int op;
  struct ast * suivant[2];
  char id[32];
  int codelen;
} ast;

ast * CreerNoeudOP(int operateur, ast * p1, ast * p2);
ast * CreerNoeudTQ(ast * p1, ast * p2);
ast * CreerNoeudAFF(char * id, ast * p2);
ast * CreerFeuilleNB(int nb);
ast * CreerNoeudLEXP(ast * p1, ast * p2);
ast * CreerFeuilleID(char * id);
ast * CreerNoeudFonction(char * id, ast * p1, ast * p2);
ast * CreerNoeudAFF(char * id, ast * p1);
ast * CreerNoeudCondition(int operateur, ast * p1, ast * p2);

void FreeAst(ast * p);

void PrintAst(ast * p);
void ErrorAst(const char * errmsg);


#endif
