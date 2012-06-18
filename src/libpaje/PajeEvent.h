/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJEEVENT_H__
#define __PAJEEVENT_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeEventDefinition.h"
#include "PajeObject.h"

class PajeEvent : public PajeObject {
public:
  paje_line *valueLine;
  PajeEventDefinition *pajeEventDefinition;
  long long lineNumber;

public:
  PajeEvent (PajeEventDefinition *def, paje_line *line);
  PajeEventId pajeEventId (void);
  std::string valueForFieldId (std::string name);
};

std::ostream &operator<< (std::ostream &output, const PajeEvent &event);

#endif

