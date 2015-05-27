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
#ifndef __PAJEEVENTDEFINITION_H__
#define __PAJEEVENTDEFINITION_H__
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <vector>
#include "PajeDefinitions.h"
#include "PajeEnum.h"

class PajeEventDefinition {
 public:  PajeEventId pajeEventIdentifier;
  int uniqueIdentifier;
  PajeDefinitions *defs;

  std::list<PajeField> fields;
  std::list<PajeFieldType> types;
  std::vector<std::string> userDefinedFieldNames;

public:
  PajeEventDefinition (PajeEventId id, int unique, int line, PajeDefinitions *definitions);
  ~PajeEventDefinition (void);
  void addField (PajeField field, PajeFieldType type, int line);
  void addField (PajeField field, PajeFieldType type, int line, std::string userDefinedFieldName);

  int indexForField (PajeField field);
  int indexForExtraFieldNamed (std::string fieldName);
  bool isValid (void);
  int fieldCount (void); //return number of fields
  void showObligatoryFields (void);
  std::vector<std::string> extraFields (void);

private:
  bool knownFieldNamed (std::string name);
};

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef);
std::ostream &operator<< (std::ostream &output, const paje_line &line);

#endif
