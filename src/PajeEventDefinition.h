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
    PajeStartTraceEventId,
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
