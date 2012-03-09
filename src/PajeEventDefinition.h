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

typedef enum {
    PajeIntFieldType,
    PajeHexFieldType,
    PajeDateFieldType,
    PajeDoubleFieldType,
    PajeStringFieldType,
    PajeColorFieldType,
    PajeUnknownFieldType
} PajeFieldType;
    
typedef enum {
    PajeEventIdFieldId,
    PajeTimeFieldId,
    PajeNameFieldId,
    PajeAliasFieldId,
    PajeTypeFieldId,
    PajeContainerFieldId,
    PajeStartContainerTypeFieldId,
    PajeEndContainerTypeFieldId,
    PajeStartContainerFieldId,
    PajeEndContainerFieldId,
    PajeColorFieldId,
    PajeValueFieldId,
    PajeKeyFieldId,
    PajeFileFieldId,
    PajeLineFieldId,
    PajeFieldIdCount,
    NoFieldId,
    PajeUnknownFieldId
} PajeFieldId;


std::map<std::string,PajeEventId> initPajeEventNamesToID (void);
std::map<PajeEventId,std::string> initPajeEventIDToNames (void);
std::map<std::string,PajeFieldId> initPajeFieldNamesToID (void);
std::map<PajeFieldId,std::string> initPajeFieldIDToNames (void);
std::map<std::string,PajeFieldType> initPajeFieldTypesToID (void);
std::map<PajeEventId,std::set<PajeFieldId> > initPajeObligatoryFields (void);
std::map<PajeEventId,std::set<PajeFieldId> > initOptionalFields (void);

class PajeEventDefinition {
public:
  char *eventId;
  PajeEventId pajeEventId;

  std::list<PajeFieldId> fields;
  std::list<PajeFieldType> types;
  int fieldCount;
  std::map<PajeFieldId,int> fieldIndexes;
  //NSArray *fieldNames;
  std::list<PajeFieldId> extraFieldNames;
  int extraFieldCount;
//  NSArray *extraFieldNames;

  PajeEventDefinition (PajeEventId id, const char *id2);
  ~PajeEventDefinition (void);
  void addField (PajeFieldId fieldId, PajeFieldType fieldType);
  int indexForFieldId (PajeFieldId fieldId);
  bool isValid (void);
  void showObligatoryFields (void);
};

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef);

#endif
