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

class PajeEventDecoder : public PajeComponent {
private:
  /* 
     These tables keep the event and field names, field types, the
     obligatory and optional fields of the Paje file format. They are
     initialized in the constructor of this class.
  */
  std::map<const char *,PajeEventId> pajeEventNames;
  std::map<const char *,PajeFieldId> pajeFieldNames;
  std::map<const char *,PajeFieldType> pajeFieldTypes;
  std::map<PajeEventId,std::set<PajeFieldId> > pajeObligatoryFields;
  std::map<PajeEventId,std::set<PajeFieldId> > pajeOptionalFields;

private:
  enum { OUT_DEF, IN_DEF } defStatus;

  PajeEventDefinition *eventBeingDefined;
  std::map<const char *,PajeEventDefinition*> eventDefinitions;


private:
  char *break_line (char *s, paje_line *line);
  void scanDefinitionLine (paje_line *line);

public:
  PajeEventDecoder (void);

  void inputEntity (PajeObject *data);
  bool canEndChunkBefore (PajeObject *data);
};
#endif
