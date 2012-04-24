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

class PajeContainerType;
class PajeLinkType;

class PajeType {
private:
  std::string name;
  std::string alias;
  PajeType *parent;

public:
  PajeType (std::string name, std::string alias, PajeType *parent);
  bool virtual isContainer (void);
  PajeType virtual *search (std::string identifier);

  friend class PajeContainerType;
  friend class PajeLinkType;
};

class PajeLinkType : public PajeType {
private:
  std::string starttype;
  std::string endtype;

public:
  PajeLinkType (std::string name, std::string alias, std::string start, std::string end, PajeType *parent);
};

class PajeContainerType : public PajeType {

public:
  std::list<PajeType*> children;

  PajeContainerType (std::string name, std::string alias, PajeType *parent);
  PajeType *search (std::string identifier);
  PajeType *getRootType (void);

  bool isContainer (void);
  bool addContainerType (std::string name, std::string alias);
  bool addVariableType (std::string name, std::string alias);
  bool addStateType (std::string name, std::string alias);
  bool addEventType (std::string name, std::string alias);
  bool addLinkType (std::string name, std::string alias, std::string starttype, std::string endtype);
private:
  bool addType (std::string name, std::string alias);
  void addChild (PajeType *type);
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
