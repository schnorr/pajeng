/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJE_ENTITY_H
#define __PAJE_ENTITY_H
#include "PajeObject.h"
#include "PajeType.h"
#include "PajeTraceEvent.h"

extern int _pajeng_quiet;
extern int _paje_user_defined;

class PajeContainer;

/*
 * PajeVirtualEntity
 */
class PajeVirtualEntity : public PajeObject
{
public:
  virtual void addPajeTraceEvent (PajeTraceEvent *event) = 0; //for Paje[Start|End]Link
  virtual PajeContainer *container (void) const = 0;
  virtual PajeType *type (void) const = 0;
  virtual const std::string &name (void) const = 0;
  virtual bool isContainedBy (PajeContainer *container) const = 0;
  virtual bool isContainer (void) const = 0;
  virtual bool isComplete (void) const = 0;
  virtual PajeValue *value (void) const = 0;

  virtual void setDoubleValue (double value) = 0;
  virtual void addDoubleValue (double value) = 0;
  virtual void subtractDoubleValue (double value) = 0;
  virtual double doubleValue (void) const = 0;
  virtual PajeContainer *startContainer (void) const = 0;
  virtual PajeContainer *endContainer (void) const = 0;
  virtual int imbricationLevel (void) const = 0;

  virtual double time (void) const = 0;
  virtual double startTime (void) const = 0;
  virtual void setStartTime (double startTime) = 0;
  virtual double endTime (void) const = 0;
  virtual void setEndTime (double endTime) = 0;
  virtual double firstTime (void) const = 0;
  virtual double lastTime (void) const = 0;
  virtual double duration (void) const = 0;
  virtual std::string key (void) const = 0; //only for links
  virtual std::string description (void) const = 0;
  virtual std::string descriptionHeader (void) const = 0;
  virtual std::string extraDescription (bool printComma) const = 0;
  virtual std::string extraDescriptionHeader (bool printComma) const = 0;
  virtual void dump (std::ostream& output) const = 0;
};

/*
 * PajeEntity: implements generic methods
 */
class PajeEntity : public PajeVirtualEntity
{
private:
  PajeContainer *_container;
  PajeType *_type;
  std::vector<std::string> extraFields;
  std::vector<std::string> extraFieldsNames;

public:
  PajeEntity (PajeContainer *container, PajeType *type, PajeTraceEvent *event);
  ~PajeEntity (void);
  void addPajeTraceEvent (PajeTraceEvent *event);
  PajeContainer *container (void) const;
  PajeType *type (void) const;
  const std::string &name (void) const;
  bool isContainedBy (PajeContainer *container) const;
  bool isContainer (void) const;
  bool isComplete (void) const;
  PajeValue *value (void) const;

  void setDoubleValue (double value);
  void addDoubleValue (double value);
  void subtractDoubleValue (double value);
  double doubleValue (void) const;
  PajeContainer *startContainer (void) const;
  PajeContainer *endContainer (void) const;
  int imbricationLevel (void) const;
  std::string key (void) const;

  struct PajeEntityCompare {
    bool operator() (PajeEntity *e, double t){
      return e->startTime() < t;
    }
    bool operator() (double t, PajeEntity *e){
      return t < e->endTime();
    }
  };

  std::string extraDescription (bool printComma) const; //should always be called after description
  std::string extraDescriptionHeader (bool printComma) const; //should always be called after description
  void dump (std::ostream& output) const;
};

/*
 * PajeSingleTimedEntity
 */
class PajeSingleTimedEntity : public PajeEntity
{
private:
  double _stime;

public:
  PajeSingleTimedEntity (PajeContainer *container, PajeType *type, double time, PajeTraceEvent *event);
  double time (void) const;
  double startTime (void) const;
  double firstTime (void) const;
  void setStartTime (double startTime);
  double endTime (void) const;
  double lastTime (void) const;
  void setEndTime (double endTime);
  double duration (void) const;
  bool isComplete (void) const;
};

/*
 * PajeDoubleTimedEntity
 */
class PajeDoubleTimedEntity : public PajeSingleTimedEntity
{
private:
  double _etime;

public:
  PajeDoubleTimedEntity (PajeContainer *container, PajeType *type, double time, PajeTraceEvent *event);
  double endTime (void) const;
  double lastTime (void) const;
  void setEndTime (double endTime);
  double duration (void) const;
  bool isComplete (void) const;
};

/*
 * PajeValueEntity
 */
class PajeValueEntity : public PajeDoubleTimedEntity
{
private:
  PajeValue *_value;

public:
  PajeValueEntity (PajeContainer *container, PajeType *type, double time, PajeValue *value, PajeTraceEvent *event);
  PajeValue *value (void) const;
  bool isComplete (void) const;
};

/*
 * PajeNamedEntity
 */
class PajeNamedEntity : public PajeDoubleTimedEntity
{
private:
  std::string _name;

public:
  PajeNamedEntity (PajeContainer *container, PajeType *type, double time, std::string name, PajeTraceEvent *event);
  const std::string &name (void) const;
  bool isComplete (void) const;
  std::string key (void) const;
};

/*
 * PajeUserEvent
 */
class PajeUserEvent : public PajeSingleTimedEntity
{
private:
  PajeValue *_value;

public:
  PajeUserEvent (PajeContainer *container, PajeType *type, double time, PajeValue *value, PajeTraceEvent *event);
  PajeValue *value (void) const;
  std::string description (void) const;
  std::string descriptionHeader (void) const;
  bool isComplete (void) const;
};

/*
 * PajeUserState
 */
class PajeUserState : public PajeValueEntity
{
private:
  double imbrication;

public:
  PajeUserState (PajeContainer *container, PajeType *type, double time, PajeValue *value, PajeTraceEvent *event);
  PajeUserState (PajeContainer *container, PajeType *type, double time, PajeValue *value, int imbrication, PajeTraceEvent *event);
  std::string description (void) const;
  std::string descriptionHeader (void) const;
  int imbricationLevel (void) const;
  bool isComplete (void) const;
};

/*
 * PajeUserVariable
 */
class PajeUserVariable : public PajeDoubleTimedEntity
{
private:
  double _value;

public:
  PajeUserVariable (PajeContainer *container, PajeType *type, double time, double value, PajeTraceEvent *event);
  std::string description (void) const;
  std::string descriptionHeader (void) const;

  void setDoubleValue (double value);
  void addDoubleValue (double value);
  void subtractDoubleValue (double value);
  double doubleValue (void) const;
  bool isComplete (void) const;
};

/*
 * PajeUserLink
 */
class PajeUserLink : public PajeValueEntity
{
private:
  std::string mkey;
  PajeContainer *startCont;
  PajeContainer *endCont;

public:
  PajeUserLink (PajeContainer *container, PajeType *type, double time, PajeValue *value, std::string key, PajeContainer *startContainer, PajeTraceEvent *event);
  std::string description (void) const;
  std::string descriptionHeader (void) const;

  void setStartContainer (PajeContainer *startContainer);
  void setEndContainer (PajeContainer *EndContainer);
  PajeContainer *startContainer (void) const;
  PajeContainer *endContainer (void) const;
  bool isComplete (void) const;
  std::string key (void) const;
};

#include "PajeContainer.h"
#endif
