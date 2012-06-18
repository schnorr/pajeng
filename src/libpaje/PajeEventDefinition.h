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
#ifndef __PAJEEVENTDEFINITION_H__
#define __PAJEEVENTDEFINITION_H__
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>

#define PAJE_MAX_FIELDS 20
typedef struct {
    char *word[PAJE_MAX_FIELDS];
    int word_count;
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

  /* std::list<PajeFieldId> extraFieldNames; */
  /* int extraFieldCount; */
  /* NSArray *extraFieldNames; */

public:
  PajeEventDefinition (PajeEventId pajeEventId, std::string number);
  ~PajeEventDefinition (void);
  void addField (std::string name, std::string type);

  int indexForFieldId (std::string name);
  bool isValid (void);
  void showObligatoryFields (void);
};

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef);

#endif
