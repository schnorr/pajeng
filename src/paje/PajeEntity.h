#ifndef __PAJE_ENTITY_H
#define __PAJE_ENTITY_H
#include "PajeObject.h"
#include "PajeType.h"
#include "PajeEvent.h"

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
public:
  std::map<std::string,std::string> extraFields;
  double time;

public:
  PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, PajeEvent *event);
  void setEvent (PajeEvent *event);
};

class PajeUserVariable : public PajeEntity
{
private:
  double val;
  double stime;
  double etime;

public:
  double startTime (void);
  double endTime (void);
  double time (void);
  double doubleValue (void);
  double maxValue (void);
  double minValue (void);

  PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime);
  void setStartTime (double startTime);
  void setEndTime (double endTime);
  void setDoubleValue (double value);
  void addDoubleValue (double value);
  void subtractDoubleValue (double value);

};

class PajeUserState : public PajeUserEvent
{
public:
  double endTime;
  int imbricationLevel;
  double innerDuration;

public:
  PajeUserState (PajeContainer *container, PajeType *type, std::string value, PajeEvent *startEvent);
  void setEndEvent (PajeEvent *event);
};

class PajeUserLink : public PajeUserState
{
public:
  std::string key;
  PajeContainer *startContainer;
  PajeContainer *endContainer;

public:
  PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, PajeEvent *startEvent);
//  PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *endContainer, PajeEvent *endEvent);
};

#include "PajeContainer.h"
#endif
