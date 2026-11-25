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
  file = fopen (f.c_str(), "r");
  if (file == NULL){
    throw PajeFileReadException (f);
  }
  initialize (definitions);
}

PajeFlexReader::PajeFlexReader(PajeDefinitions *definitions)
{
  file = NULL; //read from stdin
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
  //the whole input is parsed at once; the scanner and parser state
  //is global, so it is set up here and released at the end of the
  //parse, letting other readers in the same process start afresh
  flexReader = this;
  globalDefinitions = defs;
  yyin = file ? file : stdin;
  try {
    yyparse();
  } catch (...) {
    finishParsing();
    throw;
  }
  finishParsing();
}

void PajeFlexReader::finishParsing (void)
{
  hasData = false;
  yylex_destroy(); //also resets yyin and yylineno
  paje_bison_parser_free();
  if (file){
    fclose (file);
    file = NULL;
  }
}

PajeFlexReader::~PajeFlexReader()
{
  //only open if the input has not been parsed
  if (file) fclose(file);
}

void PajeFlexReader::closeReading (void)
{
}

