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
#ifndef __PAJE_EVENT_H__
#define __PAJE_EVENT_H__
#include "PajeTraceEvent.h"

class PajeContainer;
class PajeType;
class PajeValue;

class PajeEvent : public PajeObject
{
private:
  PajeTraceEvent *_event;
  PajeContainer *_container;
  PajeType *_type;
  double _time;

public:
  PajeEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type);

  PajeTraceEvent *traceEvent (void);
  PajeContainer *container (void);
  PajeType *type (void);
  double time (void);

  virtual PajeValue *value (void);
  virtual double doubleValue (void);
  virtual PajeContainer *startContainer (void);
  virtual PajeContainer *endContainer (void);
  virtual const std::string &key (void);
  virtual const std::string &kind (void) = 0;
};


class PajeCategorizedEvent : public PajeEvent
{
  PajeValue *_value;

public:
  PajeCategorizedEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value);
  PajeValue *value (void);
  const std::string &kind (void);
};

class PajeStateEvent : public PajeCategorizedEvent
{
public:
  PajeStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value);
};

class PajeEventEvent : public PajeCategorizedEvent
{
public:
  PajeEventEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value);
};

class PajeVariableEvent : public PajeEvent
{
private:
  double _doubleValue;

public:
  PajeVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value);
  double doubleValue (void);
  const std::string &kind (void);
};

class PajeLinkEvent : public PajeCategorizedEvent
{
protected:
  PajeContainer *_linkedContainer;
private:
  std::string _key;
public:
  PajeLinkEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value, PajeContainer *linkedContainer, std::string key);
  const std::string &key (void);
};

/* Simulator Events */

class PajeNewEventEvent : public PajeEventEvent
{
public:
  PajeNewEventEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value);
};

class PajeSetStateEvent : public PajeStateEvent
{
public:
  PajeSetStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value);
};

class PajePushStateEvent : public PajeStateEvent
{
public:
  PajePushStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value);
};

class PajePopStateEvent : public PajeStateEvent
{
public:
  PajePopStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type);
};

class PajeResetStateEvent : public PajeStateEvent
{
public:
  PajeResetStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type);
};

class PajeSetVariableEvent : public PajeVariableEvent
{
public:
  PajeSetVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value);
};

class PajeAddVariableEvent : public PajeVariableEvent
{
public:
  PajeAddVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value);
};

class PajeSubVariableEvent : public PajeVariableEvent
{
public:
  PajeSubVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value);
};

class PajeStartLinkEvent : public PajeLinkEvent
{
public:
  PajeStartLinkEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value, PajeContainer *linkedContainer, std::string key);
  PajeContainer *startContainer (void);
};

class PajeEndLinkEvent : public PajeLinkEvent
{
public:
  PajeEndLinkEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value, PajeContainer *linkedContainer, std::string key);
  PajeContainer *endContainer (void);
};

class PajeDestroyContainerEvent : public PajeEvent
{
public:
  PajeDestroyContainerEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type);
  const std::string &kind (void);
};

#include "PajeContainer.h"
#include "PajeType.h"
#include "PajeValue.h"
#endif
