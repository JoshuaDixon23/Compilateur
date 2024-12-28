%{
  #include <stdio.h>
  #include <ctype.h>
  #include <string.h>
  
  #include "ast.h"
  #include "ts.h"
  #include "codegen.h"
  #include "semantic.h"
    
  extern int yylex();
  static void print_file_error(char * s, char *errmsg);

  struct ast * ARBRE_ABSTRAIT = NULL;

  void yyerror(const char * s);

  char srcname[64];
  char exename[64] = "a.out";
  FILE * out;
  FILE * exefile;
  ts TABSYMB;
  char CTXT[32] = "GLOBAL";
%}

%union{
  int nb;
  struct ast * arbre;
  char id[32];
 };

%define parse.error detailed
%locations

%type <arbre> EXP
%type <arbre> L_EXP
%type <arbre> L_ID
%type <arbre> L_PARAM
%type <arbre> FONCTION
%type <arbre> L_FONCTION
%type <arbre> STRUCT_TQ
%type <arbre> STRUCT_SI
%type <arbre> CONDITION 
%type <arbre> APPEL_FONCTION
%type <id> FUCNID

%token MAIN '(' ')' ';' ',' ALGO
%token VAR
%token AFFECT
%token DEBUT FIN 
%token TQ FAIRE FINTQ
%token SI ALORS SINON FINSI
%token <nb> NB 
%token <id> ID 
%token '>' '<' '=' '!'
%left '+' '-' 
%left '*' '/'
%start PROGRAMME

%%

PROGRAMME:L_FONCTION
          MAIN '(' ')' 
          DECLA_VAR
          DEBUT 
            L_EXP
          FIN {semantic($7);  PrintAst($1); PrintAst($7);  {codegenINIT();}; codegen($7);}
          ;

DECLA_VAR: %empty   
        | VAR ID';' DECLA_VAR {ts_ajouter_id(TABSYMB, CTXT, $2, 0);}

L_EXP: EXP ';' L_EXP {$$ = CreerNoeudLEXP($1, $3);}
    | EXP ';'{$$ = CreerNoeudLEXP($1, NULL);}
    | STRUCT_TQ L_EXP {$$ = CreerNoeudLEXP($1, $2);}
    | STRUCT_TQ {$$ = CreerNoeudLEXP($1, NULL);}
    | STRUCT_SI L_EXP {$$ = CreerNoeudLEXP($1, $2);}
    | STRUCT_SI {$$ = CreerNoeudLEXP($1, NULL);}
    ;

STRUCT_TQ: TQ CONDITION FAIRE
              L_EXP
          FINTQ {$$ = CreerNoeudTQ($2, $4);}

STRUCT_SI: SI CONDITION ALORS
             L_EXP
           SINON
             L_EXP
           FINSI {$$ = CreerNoeudSI($2, $4, $6);}
         | SI CONDITION ALORS
             L_EXP
           FINSI {$$ = CreerNoeudSI($2, $4, NULL);}
         ;

CONDITION: EXP '>' EXP {$$ = CreerNoeudCondition('>', $1, $3);}
         | EXP '<' EXP {$$ = CreerNoeudCondition('<', $1, $3);}
         | EXP '=' EXP {$$ = CreerNoeudCondition('=', $1, $3);}
         | EXP '!' EXP {$$ = CreerNoeudCondition('!', $1, $3);}
         ;

EXP : EXP '+' EXP {$$ = CreerNoeudOP('+', $1, $3);}
    | EXP '-' EXP {$$ = CreerNoeudOP('-', $1, $3);}
    | EXP '*' EXP {$$ = CreerNoeudOP('*', $1, $3);}
    | EXP '/' EXP {$$ = CreerNoeudOP('/', $1, $3);}
    | ID AFFECT EXP {$$ = CreerNoeudAFF($1, $3);}
    | '('EXP')' {$$ = $2;}
    | NB {$$ = CreerFeuilleNB($1);}
    | ID {$$ = CreerFeuilleID($1);}
    | APPEL_FONCTION {$$ = $1;} 
    ;

APPEL_FONCTION : ID '(' L_PARAM ')' { $$ = CreerNoeudAppel($1, $3); };

L_PARAM : %empty {$$ = NULL;}
      | L_ID {$$ = $1;}
      ;

L_ID : ID {$$ = CreerNoeudLEXP(CreerFeuilleID($1), NULL);}
    | ID ',' L_ID {$$ = CreerNoeudLEXP(CreerFeuilleID($1), $3);}
    ;

FUCNID : ID {strcpy(CTXT, $1); ts_ajouter_id(TABSYMB, CTXT, $1, 2);}

FONCTION: ALGO FUCNID '(' L_PARAM ')'
          DECLA_VAR
          DEBUT
            L_EXP
          FIN { $$ = CreerNoeudFonction($2, $4, $8);}
          ;

L_FONCTION: %empty {$$ = NULL;}
          | FONCTION L_FONCTION {strcpy(CTXT, "GLOBAL");$$ = CreerNoeudLFonction($1,$2);semantic($1);}
          ;
%%

int main( int argc, char * argv[] ) {
  extern FILE *yyin;
  out = stdout;
  
  if (argc > 1){
    strcpy(srcname, argv[1]);
    if ( (yyin = fopen(srcname,"r"))==NULL ){
      char errmsg[256];
      sprintf(errmsg,"fichier \x1b[1m\x1b[33m' %s '\x1b[0m introuvable",srcname);
      print_file_error(argv[0],errmsg);
      exit(1);
    }
  }  else {
    print_file_error(argv[0],"aucun fichier en entree");
    exit(1);
  }
  if (argc == 3){
    strcpy(exename, argv[2]);
  }
  exefile = fopen(exename,"w");
  INIT_TS(TABSYMB);
  yyparse();
  PrintTS(TABSYMB);
  fclose(yyin);
}


static void print_file_error(char * prog, char *errmsg){
  fprintf(stderr,
	  "\x1b[1m%s:\x1b[0m \x1b[31m\x1b[1merreur fatale:\x1b[0m %s\nechec de la compilation\n",
	  prog, errmsg);
}

void yyerror(const char * s)
{
  fprintf(stderr, "\x1b[1m%s:%d:%d:\x1b[0m %s\n", srcname, yylloc.first_line, yylloc.first_column, s);
  exit(0);
}
