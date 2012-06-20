/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJE_ENTITY_H
#define __PAJE_ENTITY_H
#include "PajeObject.h"
#include "PajeType.h"
#include "PajeEvent.h"

class PajeContainer;

class PajeEntity : public PajeObject
{
private:
  PajeContainer *entityContainer;
  PajeType *entityType;
  std::string entityName;
  std::map<std::string,std::string> extraFields;

public:
  PajeEntity (PajeContainer *container, PajeType *type, std::string name, PajeEvent *event);
  PajeContainer *container (void) const;
  PajeType *type (void) const;
  std::string name (void) const;
  void setName (std::string newname);
  bool isContainedBy (PajeContainer *container);
  virtual bool isContainer (void);
  virtual std::string value (void);
  virtual void setValue (std::string newvalue);
  virtual void setDoubleValue (double value);
  virtual void addDoubleValue (double value);
  virtual void subtractDoubleValue (double value);
  virtual double doubleValue (void);
  virtual PajeContainer *startContainer (void);
  virtual PajeContainer *endContainer (void);
  virtual int imbricationLevel (void);

  virtual double time (void) = 0;
  virtual double startTime (void) = 0;
  virtual void setStartTime (double startTime) = 0;
  virtual double endTime (void) = 0;
  virtual void setEndTime (double endTime) = 0;
  virtual double firstTime (void) = 0;
  virtual double lastTime (void) = 0;
  virtual double duration (void) = 0;
  virtual std::string description (void) = 0;

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
  PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, double time, PajeEvent *event);
  std::string description (void);
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
  double imbrication;

public:
  PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, PajeEvent *event);
  PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, int imbrication, PajeEvent *event);
  std::string description (void);
  void setValue (std::string newvalue);
  double endTime (void);
  void setEndTime (double endTime);
  double lastTime (void);
  double duration (void);
  int imbricationLevel (void);
};

class PajeUserVariable : public PajeUserState
{
private:
  double val;

public:
  PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime, PajeEvent *event);
  std::string description (void);

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
  PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, double startTime, PajeEvent *event);
  std::string description (void);

  void setStartContainer (PajeContainer *startContainer);
  void setEndContainer (PajeContainer *EndContainer);
  PajeContainer *startContainer (void);
  PajeContainer *endContainer (void);
};

#include "PajeContainer.h"
#endif
