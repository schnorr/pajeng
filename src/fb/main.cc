/*
   main.c

   Arquivo principal do analisador sintático.
*/
#include <stdio.h>
#include <search.h>

#include "PajeEventDefinition.h"
#include "parser.hh"
#include "main.h"

extern "C"
{
  extern int yylineno;
  int yylex(void);
  void yyerror (char const *mensagem)
  {
    fprintf (stderr, "%s on line %d\n", mensagem, yylineno);
  }
  int yyparse ();
  extern char *yytext;
  extern int yychar;
  extern PajeEventDef **defsv;
}

int main (int argc, char **argv)
{
  long long counter = 0;

  /* read the header */
  int resultado = yyparse();
  if (resultado != 4){
    return 1;
  }

  /* read the events */
  int token = yychar;
  do{
    if (token == TK_INT){
      PajeEventDef *def = defsv[atoi(yytext)];

      //the line to be read
      paje_line line;

      //the line number
      line.lineNumber = yylineno;

      //read the fields until TK_BREAK
      line.word_count = 0;
      do {
        line.word[line.word_count++] = yytext;
	token = yylex();
      } while (token != TK_BREAK);

      //verify number of fields (expected and real)
      if (def->fields.size() != line.word_count){
        printf ("number of fields of event %s at line %d doesn't match. expected: %d, found: %d\n",
                line.word[0], line.lineNumber, def->fields.size(), line.word_count);
      }
      //printf ("%d %d %p\n", line.lineNumber, line.word_count, &line);

      counter++;
    }else{
      printf ("token <%d> at %d (%s)\n", token, yylineno, yytext);
      exit(1);
    }
  } while ((token = yylex()) && token != TOKEN_ERRO);

  /* print number of events */
  printf ("%lld events and %lld lines\n", counter, yylineno);

  /* exit */
  if (token == TOKEN_ERRO){
    return 1;
  }else{
    return 0;
  }
  return resultado;

}

