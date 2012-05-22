#ifndef __PAJE_ENTITY_H
#define __PAJE_ENTITY_H
#include "PajeObject.h"
#include "PajeType.h"

class PajeContainer;

class PajeEntity : public PajeObject
{
public:
  PajeContainer *container;
  PajeType *type;
  std::string name;

public:
  PajeEntity (PajeContainer *container, PajeType *type, std::string name);
 
  bool virtual isContainer (void);
  std::string virtual value (void);
  PajeContainer virtual *startContainer (void);
  PajeContainer virtual *endContainer (void);
  bool virtual isContainedBy (PajeContainer *container);

  double virtual time (void);
  double virtual startTime (void);
  double virtual endTime (void);
  double virtual firstTime (void);
  double virtual lastTime (void);
  double virtual duration (void);
  double virtual exclusiveDuration (void);

  double virtual doubleValue (void);
  double virtual minValue (void);
  double virtual maxvalue (void);
};

class PajeUserEvent : public PajeEntity
{
private:
  double t;

public:
  PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, double time);
  void setStartTime (double startTime);
  double startTime (void);
  double time (void);
};

class PajeUserState : public PajeUserEvent
{
private:
  double etime;
  int imbricationLevel;
  double innerDuration;

public:
  PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, double endTime);
  void setEndTime (double endTime);
  double endTime (void);
};

class PajeUserVariable : public PajeUserState
{
private:
  double val;

public:
  PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime);
  void setDoubleValue (double value);
  void addDoubleValue (double value);
  void subtractDoubleValue (double value);
  double doubleValue (void);
  double maxValue (void);
  double minValue (void);

};

class PajeUserLink : public PajeUserState
{
private:
  std::string key;
  PajeContainer *startCont;
  PajeContainer *endCont;

public:
  PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, double startTime);
  std::string value (void);

  void setStartContainer (PajeContainer *startContainer);
  void setEndContainer (PajeContainer *EndContainer);
  PajeContainer *startContainer (void);
  PajeContainer *endContainer (void);
};

#include "PajeContainer.h"
#endif
