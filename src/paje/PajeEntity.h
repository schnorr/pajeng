#ifndef __PAJE_ENTITY_H
#define __PAJE_ENTITY_H
#include "PajeObject.h"
#include "PajeType.h"

class PajeContainer;

class PajeEntity : public PajeObject
{
private:
  PajeContainer *entityContainer;
  PajeType *entityType;
  std::string entityName;

public:
  PajeEntity (PajeContainer *container, PajeType *type, std::string name);
  PajeContainer *container (void) const;
  PajeType *type (void) const;
  std::string name (void) const;
  bool isContainedBy (PajeContainer *container);
  virtual bool isContainer (void);
  virtual std::string value (void);
  virtual void setDoubleValue (double value);
  virtual void addDoubleValue (double value);
  virtual void subtractDoubleValue (double value);
  virtual double doubleValue (void);
  virtual PajeContainer *startContainer (void);
  virtual PajeContainer *endContainer (void);

  virtual double time (void) = 0;
  virtual double startTime (void) = 0;
  virtual void setStartTime (double startTime) = 0;
  virtual double endTime (void) = 0;
  virtual void setEndTime (double endTime) = 0;
  virtual double firstTime (void) = 0;
  virtual double lastTime (void) = 0;
  virtual double duration (void) = 0;

  struct PajeEntityCompare {
    bool operator() (PajeEntity *e, double t){
      return e->startTime() < t;
    }
  };
};

class PajeUserEvent : public PajeEntity
{
private:
  double t;

public:
  PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, double time);
  double time (void);
  double startTime (void);
  void setStartTime (double startTime);
  double endTime (void);
  void setEndTime (double endTime);
  double firstTime (void);
  double lastTime (void);
  double duration (void);
};

class PajeUserState : public PajeUserEvent
{
private:
  double etime;

public:
  PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, double endTime);
  double endTime (void);
  void setEndTime (double endTime);
  double lastTime (void);
  double duration (void);
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
};

class PajeUserLink : public PajeUserState
{
private:
  std::string key;
  PajeContainer *startCont;
  PajeContainer *endCont;

public:
  PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, double startTime);

  void setStartContainer (PajeContainer *startContainer);
  void setEndContainer (PajeContainer *EndContainer);
  PajeContainer *startContainer (void);
  PajeContainer *endContainer (void);
};

#include "PajeContainer.h"
#endif
