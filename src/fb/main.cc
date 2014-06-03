/*
   main.c

   Arquivo principal do analisador sintático.
*/
#include <stdio.h>
#include <search.h>

#include "PajeEventDefinition.h"
#include "PajeTraceEvent.h"
#include "PajeException.h"
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
  extern PajeEventDefinition **defsv;
}

int main (int argc, char **argv)
{
  long long counter = 0;

  try{
  /* read the header */
  int resultado = yyparse();
  if (resultado != 4){
    return 1;
  }

  /* read the events */
  int token = yychar;
  do{
    if (token == TK_INT){
      PajeEventDefinition *def = defsv[atoi(yytext)];

      //the line to be read
      paje_line line;

      //the line number
      line.lineNumber = yylineno;

      //read the fields until TK_BREAK
      line.word_count = 0;
      do {
        line.word[line.word_count++] = strdup(yytext);
	token = yylex();
      } while (token != TK_BREAK);

      PajeTraceEvent *event = new PajeTraceEvent (def, &line);
      delete event;

      //clean-up paje_line
      {
	int i;
	for (i = 0; i < line.word_count; i++){
	  free (line.word[i]);
	}
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
  }catch (PajeException &e){
    e.reportAndExit();
  }
}

