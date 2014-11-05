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
#ifndef __PAJEFLEXREADER_H__
#define __PAJEFLEXREADER_H__
#include "PajeComponent.h"

extern "C"
{
  extern int yylineno;
  int yylex(void);
  void yyerror (char const *mensagem);
  int yyparse ();
  extern char *yytext;
  extern int yyleng;
  extern int yychar;
  extern PajeEventDefinition **defsv;

  extern PajeDefinitions *globalDefinitions;
}

class PajeFlexReader : public PajeComponent {
 private:
  std::string filename;
  long long counter;
  bool hasData;
  PajeDefinitions *defs;
 public:
  PajeFlexReader(std::string f, PajeDefinitions *definitions);
  PajeFlexReader(PajeDefinitions *definitions);
  ~PajeFlexReader();

  bool hasMoreData();
  void readNextChunk();

 private:
  void initialize (PajeDefinitions *definitions);
};

extern "C"
{
  extern PajeFlexReader *flexReader;
}

#endif
