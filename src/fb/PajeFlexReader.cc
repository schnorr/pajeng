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
  int resultado = yyparse();
  if (resultado != 4){
    throw PajeDecodeException("Can't parse the header");
  }
  hasData = true;
}

bool PajeFlexReader::hasMoreData()
{
  return hasData;
}

void PajeFlexReader::readNextChunk()
{
  int token = yychar;
  do{
    if (token == TK_END){
      hasData = false;
    }else if (token == TK_INT){
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
      } while (token != TK_BREAK && token != TK_END);

      if (token == TK_END){
	hasData = false;
	return;
      }

      PajeTraceEvent *event = new PajeTraceEvent (def, &line);
      if (event != NULL){
        PajeComponent::outputEntity (event);
        delete event;
      }

      //clean-up paje_line
      {
	int i;
	for (i = 0; i < line.word_count; i++){
	  free (line.word[i]);
	}
      }
      counter++;
    }
  } while ((token = yylex()) && token != TK_END);
}

PajeFlexReader::~PajeFlexReader()
{
  /* print number of events */
  printf ("%lld events and %lld lines\n", counter, yylineno);
}

