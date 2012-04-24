#ifndef __PAJESIMULATOR_H__
#define __PAJESIMULATOR_H__
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeComponent.h"
#include "PajeEvent.h"


#define CALL_MEMBER_PAJE_SIMULATOR(object,ptr) ((object).*(ptr))

class PajeContainerType {
private:
  std::string name;
  std::string alias;

public:
  std::list<PajeContainerType*> children;

  PajeContainerType (std::string name, std::string alias);
  PajeContainerType *search (std::string identifier);
  void addChild (PajeContainerType *child);
};

class PajeSimulator : public PajeComponent {
private:
  PajeContainerType *rootType;

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
};
#endif
