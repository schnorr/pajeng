/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <search.h>

#include "PajeEventDefinition.h"
#include "PajeTraceEvent.h"
#include "PajeException.h"
extern "C"{
#include "parser.hh"
}
#include "main.h"

extern "C"
{
  extern int yylineno;
  int yylex(void);
  int yyparse ();
  extern char *yytext;
  extern int yychar;
  extern PajeEventDefinition **defsv;
}

int main (int argc, char **argv)
{
  long long counter = 0;

  //the global PajeDefinitions object
  globalDefinitions = new PajeDefinitions (false);

  return yyparse();
}

