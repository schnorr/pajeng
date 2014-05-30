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

typedef enum {
  PAJE_Event,
  PAJE_Time,
  PAJE_Name,
  PAJE_Type,
  PAJE_Container,
  PAJE_StartContainerType,
  PAJE_EndContainerType,
  PAJE_StartContainer,
  PAJE_EndContainer,
  PAJE_Value,
  PAJE_Key,
  PAJE_Alias,
  PAJE_Color,
  PAJE_Line,
  PAJE_File,
  PAJE_Extra
} PajeField;

std::map<std::string,PajeEventId> initPajeEventNamesToID (void);
std::map<PajeEventId,std::string> initPajeEventIDToNames (void);
std::map<std::string,PajeField> initPajeFieldNamesToID (void);
std::map<PajeField,std::string> initPajeFieldIDToNames (void);

class PajeEventDefinition {
private:
  bool strictDefinition;

public:
  PajeEventId pajeEventId; //The known PajeEventId
  int number; //unique identifier
  std::list<PajeField> fields;
  std::list<std::string> names;
  std::list<std::string> types;
  std::vector<std::string> userDefinedFieldNames;

public:
  PajeEventDefinition (PajeEventId pajeEventId, int number, paje_line *line, bool strictHeader);
  ~PajeEventDefinition (void);
  void addField (std::string name, std::string type, paje_line *line);

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
