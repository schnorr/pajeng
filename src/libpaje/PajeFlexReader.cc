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
#include "PajeFlexReader.h"
#include "PajeException.h"
#include "parser.hh"

void yyerror (char const *mensagem)
{
  fprintf (stderr, "%s on line %d\n", mensagem, yylineno);
}

extern FILE *yyin;

PajeFlexReader::PajeFlexReader(std::string f, PajeDefinitions *definitions)
{
  filename = f;
  yyin = fopen (filename.c_str(), "r");
  if (yyin == NULL){
    throw PajeFileReadException (f);
  }
  initialize (definitions);
}

PajeFlexReader::PajeFlexReader(PajeDefinitions *definitions)
{
  initialize (definitions);
}

void PajeFlexReader::initialize (PajeDefinitions *definitions)
{
  defs = globalDefinitions = definitions;
  flexReader = this;
  hasData = true;
}

bool PajeFlexReader::hasMoreData()
{
  return hasData;
}

void PajeFlexReader::readNextChunk()
{
  int resultado = yyparse();
  hasData = false;
}

PajeFlexReader::~PajeFlexReader()
{
  /* print number of events */
  printf ("%lld events and %lld lines\n", counter, yylineno);
}

