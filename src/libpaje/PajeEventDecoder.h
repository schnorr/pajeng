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
#ifndef __PAJEEVENTDECODER_H__
#define __PAJEEVENTDECODER_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeComponent.h"
#include "PajeEventDefinition.h"
#include "PajeTraceEvent.h"
#include "PajeDefinitions.h"

class PajeEventDecoder : public PajeComponent {
private:
  enum { OUT_DEF, IN_DEF } defStatus;
  PajeEventDefinition *eventBeingDefined;
  std::map<std::string,PajeEventDefinition*> eventDefinitions;

  char *break_line (char *s, paje_line *line);
  void scanDefinitionLine (paje_line *line);
  PajeTraceEvent *scanEventLine (paje_line *line);
  long long currentLineNumber;
  PajeDefinitions *defs;

public:
  PajeEventDecoder (PajeDefinitions *definitions);
  ~PajeEventDecoder (void);

  void inputEntity (PajeObject *data);
};
#endif
