#ifndef __PAJESIMULATOR_H__
#define __PAJESIMULATOR_H__
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeEvent.h"
#include "PajeType.h"
#include "PajeContainer.h"
#include "PajeComponent.h"

#define CALL_MEMBER_PAJE_SIMULATOR(object,ptr) ((object).*(ptr))

class PajeSimulator : public PajeComponent {
private:
  PajeContainerType *rootType;
  PajeContainer *root;
  std::map<std::string,PajeType*> typeMap;
  std::map<std::string,PajeContainer*> contMap;

  double lastKnownTime;

public:
  PajeSimulator();
  
  void inputEntity (PajeObject *data);
  bool canEndChunkBefore (PajeObject *data);

  void startChunk (int chunkNumber);
  void endOfChunkLast (bool last);

private:
  void (PajeSimulator::*invocation[PajeEventIdCount])(PajeEvent*);

  void pajeDefineContainerType (PajeEvent *event);
  void pajeDefineLinkType (PajeEvent *event);
  void pajeDefineEventType (PajeEvent *event);
  void pajeDefineStateType (PajeEvent *event);
  void pajeDefineVariableType (PajeEvent *event);
  void pajeDefineEntityValue (PajeEvent *event);

  void pajeCreateContainer (PajeEvent *event);
  void pajeDestroyContainer (PajeEvent *event);

  void pajeNewEvent (PajeEvent *event);

  void pajeSetState (PajeEvent *event);
  void pajePushState (PajeEvent *event);
  void pajePopState (PajeEvent *event);

  void pajeSetVariable (PajeEvent *event);
  void pajeAddVariable (PajeEvent *event);
  void pajeSubVariable (PajeEvent *event);

  void pajeStartLink (PajeEvent *event);
  void pajeEndLink (PajeEvent *event);

public:

//queries
  double startTime (void);
  double endTime (void);

  double selectionStartTime (void);
  double selectionEndTime (void);

  PajeContainer *rootInstance (void);
  std::map<std::string,PajeType*>::iterator containedTypesForContainerType (PajeType *containerType);
  PajeType *containerTypeForType (PajeType *type);
};
#endif
