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

public:
  PajeEventDecoder (void);

  void inputEntity (PajeObject *data);
  bool canEndChunkBefore (PajeObject *data);
};
#endif
