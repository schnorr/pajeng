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

#define PAJE_MAX_FIELDS 20
typedef struct {
    char *word[PAJE_MAX_FIELDS];
    int word_count;
    long long lineNumber;
} paje_line;

typedef enum {
    PajeDefineContainerTypeEventId,
    PajeDefineEventTypeEventId,
    PajeDefineStateTypeEventId,
    PajeDefineVariableTypeEventId,
    PajeDefineLinkTypeEventId,
    PajeDefineEntityValueEventId,
    PajeCreateContainerEventId,
    PajeDestroyContainerEventId,
    PajeNewEventEventId,
    PajeSetStateEventId,
    PajePushStateEventId,
    PajePopStateEventId,
    PajeResetStateEventId,
    PajeSetVariableEventId,
    PajeAddVariableEventId,
    PajeSubVariableEventId,
    PajeStartLinkEventId,
    PajeEndLinkEventId,
    PajeEventIdCount,
    PajeUnknownEventId,
} PajeEventId;

std::map<std::string,PajeEventId> initPajeEventNamesToID (void);
std::map<PajeEventId,std::string> initPajeEventIDToNames (void);

class PajeEventDefinition {
public:
  PajeEventId pajeEventId; //The known PajeEventId
  std::string number; //Unique identifier
  int fieldCount;
  std::list<std::string> fields;
  std::list<std::string> types;
  std::vector<std::string> extraFieldNames;

public:
  PajeEventDefinition (PajeEventId pajeEventId, std::string number, paje_line *line);
  ~PajeEventDefinition (void);
  void addField (std::string name, std::string type, paje_line *line);

  int indexForFieldId (std::string name);
  bool isValid (void);
  void showObligatoryFields (void);
  std::vector<std::string> extraFields (void);
};

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef);
std::ostream &operator<< (std::ostream &output, const paje_line &line);

#endif
