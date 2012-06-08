/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJEEVENTDECODER_H__
#define __PAJEEVENTDECODER_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeComponent.h"
#include "PajeEventDefinition.h"
#include "PajeEvent.h"


class PajeEventDecoder : public PajeComponent {
private:
  enum { OUT_DEF, IN_DEF } defStatus;
  PajeEventDefinition *eventBeingDefined;
  std::map<std::string,PajeEventDefinition*> eventDefinitions;

  char *break_line (char *s, paje_line *line);
  void scanDefinitionLine (paje_line *line);
  PajeEvent *scanEventLine (paje_line *line);
  long long currentLineNumber;

public:
  PajeEventDecoder (void);
  ~PajeEventDecoder (void);

  void inputEntity (PajeObject *data);
  bool canEndChunkBefore (PajeObject *data);
};
#endif
