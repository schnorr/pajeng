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
#ifndef __PAJETRACEEVENT_H__
#define __PAJETRACEEVENT_H__
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeEventDefinition.h"
#include "PajeObject.h"

class PajeTraceEvent : public PajeObject {
 private:
  int line;
  PajeEventDefinition *pajeEventDefinition;
  std::vector<std::string> fields;

public:
  PajeTraceEvent ();
  PajeTraceEvent (int line);
  PajeTraceEvent (PajeEventDefinition *def, paje_line *line);
  ~PajeTraceEvent ();
  PajeEventId pajeEventId (void);
  void addField (char *field);
  void clear (void);
  bool check (paje_line *line);
  std::string valueForField (PajeField field);
  std::string valueForExtraField (std::string fieldName);
  long long getLineNumber (void) const;
  std::string description (void) const;
  PajeEventDefinition *definition (void);
  void setDefinition (PajeEventDefinition *def);
};

std::ostream &operator<< (std::ostream &output, const PajeTraceEvent &event);

#endif

