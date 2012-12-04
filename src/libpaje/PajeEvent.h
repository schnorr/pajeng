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
#include "PajeContainer.h"
#include "PajeType.h"
#include "PajeEntity.h"
#include "PajeValue.h"

class PajeEvent : public PajeObject
{
private:
  PajeTraceEvent *_event;
  PajeContainer *_container;
  PajeType *_type;

public:
  PajeEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type);

  PajeTraceEvent *event (void);
  PajeContainer *container (void);
  PajeType *type (void);

  virtual PajeValue *value (void) = 0;
  virtual double doubleValue (void) = 0;
  virtual PajeContainer *startContainer (void) = 0;
  virtual PajeContainer *endContainer (void) = 0;
};


class PajeCategorizedEvent : public PajeEvent
{
  PajeValue *_value;

public:
  PajeCategorizedEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value);
  PajeValue *value (void);
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
};

class PajeLinkEvent : public PajeCategorizedEvent
{
private:
  PajeContainer *_linkedContainer;
  std::string _key;

public:
  PajeLinkEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value, PajeContainer *linkedContainer, std::string key);
};

/* Simulator Events */

class PajeSetStateEvent : public PajeStateEvent {};
class PajePushStateEvent : public PajeStateEvent {};
class PajePopStateEvent : public PajeStateEvent {};
class PajeResetStateEvent : public PajeStateEvent {};
class PajeSetVariableEvent : public PajeVariableEvent {};
class PajeAddVariableEvent : public PajeVariableEvent {};
class PajeSubVariableEvent : public PajeVariableEvent {};
class PajeStartLinkEvent : public PajeLinkEvent {};
class PajeEndLinkEvent : public PajeLinkEvent {};

#endif
