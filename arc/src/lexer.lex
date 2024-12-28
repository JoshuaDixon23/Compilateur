%{
  #include <string.h>
  #include "parser.h"

  extern void yyerror(char *);

  char errmsg[256]="";
  const char charerr[] = "\x1b[1m\x1b[31m[erreur lexicale]\x1b[0m caractère \x1b[41m%c\x1b[0m inattendu";
  
  /* MACRO défini 
   * Action executee avant chaque action semantique (meme vide)  
   * et rempli la variable Bison `yylloc` avec la position du token
   */
#define YY_USER_ACTION                                             \
  yylloc.first_line = yylloc.last_line;                            \
  yylloc.first_column = yylloc.last_column;                        \
  if (yylloc.last_line == yylineno)                                \
    yylloc.last_column += yyleng;                                  \
  else {                                                           \
    yylloc.last_line = yylineno;                                   \
    yylloc.last_column = 1;					   \
  }


  

%}
  
%option nounput
%option noinput
%option yylineno

NOMBRE          ([1-9][0-9]*|0)
ID              ([a-zA-Z][a-zA-Z_1-9]*)
CONDITION       ()
%%
"MAIN" {return MAIN;}
"DEBUT" {return DEBUT;}
"FIN" {return FIN;}
"VAR" {return VAR;}
"TQ"  {return TQ;}
"FAIRE" {return FAIRE;}
"FINTQ" {return FINTQ;}
"SI"  {return SI;}
"ALORS" {return ALORS;}
"SINON" {return SINON;}
"FINSI"   {return FINSI;}
"ALGO" {return ALGO;}
"<-" {return AFFECT;}
[-+%*(/);=<>!,] {return yytext[0];}
{NOMBRE}        { yylval.nb = atoi(yytext); return NB; }
{ID} {strcpy(yylval.id, yytext);return ID;}
[ \t\r\n]+ {} 
.               {           
                  sprintf(errmsg,charerr, yytext[0]);
		  yyerror(errmsg);
		  return 1;
                }

%%
 
