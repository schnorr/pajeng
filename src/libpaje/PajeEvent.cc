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
#include "PajeEvent.h"
#include "PajeException.h"

PajeEvent::PajeEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type)
{
  _event = event;
  _container = container;
  _type = type;

  //extracing time
  std::string t = _event->valueForField (PAJE_Time);
  if (t.length() == 0){
    PajeSimulationException ("Can't get time from trace event.");
  }
  _time = atof(t.c_str());
}

PajeTraceEvent *PajeEvent::traceEvent (void)
{
  return _event;
}

PajeContainer *PajeEvent::container (void)
{
  return _container;
}

PajeType *PajeEvent::type (void)
{
  return _type;
}

double PajeEvent::time (void)
{
  return _time;
}

PajeValue *PajeEvent::value (void)
{
  return NULL;
}

double PajeEvent::doubleValue (void)
{
  return 0;
}

PajeContainer *PajeEvent::startContainer (void)
{
  return NULL;
}

PajeContainer *PajeEvent::endContainer (void)
{
  return NULL;
}

const std::string &PajeEvent::key (void)
{
  static const std::string nullstr = std::string();
  return nullstr;
}

PajeCategorizedEvent::PajeCategorizedEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value)
  : PajeEvent (event, container, type)
{
  _value = value;
}

PajeValue *PajeCategorizedEvent::value (void)
{
  return _value;
}

const std::string &PajeCategorizedEvent::kind (void)
{
  static const std::string catstr = "Categorized";
  return catstr;
}

PajeStateEvent::PajeStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value)
  : PajeCategorizedEvent (event, container, type, value)
{
}

PajeEventEvent::PajeEventEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value)
  : PajeCategorizedEvent (event, container, type, value)
{
}

PajeVariableEvent::PajeVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value)
  : PajeEvent (event, container, type)
{
  _doubleValue = value;
}

 const std::string &PajeVariableEvent::kind (void)
{
  static const std::string varstr = "Variable";
  return varstr;
}

double PajeVariableEvent::doubleValue (void)
{
  return _doubleValue;
}

PajeLinkEvent::PajeLinkEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value, PajeContainer *linkedContainer, std::string key)
  : PajeCategorizedEvent (event, container, type, value)
{
  _linkedContainer = linkedContainer;
  _key = key;
}

const std::string &PajeLinkEvent::key (void)
{
  return _key;
}

/* Simulator Events */

PajeNewEventEvent::PajeNewEventEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value)
  : PajeEventEvent (event, container, type, value)
{
}

PajeSetStateEvent::PajeSetStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value)
  : PajeStateEvent (event, container, type, value)
{
}

PajePushStateEvent::PajePushStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value)
  : PajeStateEvent (event, container, type, value)
{
}

PajePopStateEvent::PajePopStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type)
  : PajeStateEvent (event, container, type, NULL)
{
}

PajeResetStateEvent::PajeResetStateEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type)
  : PajeStateEvent (event, container, type, NULL)
{
}

PajeSetVariableEvent::PajeSetVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value)
  : PajeVariableEvent (event, container, type, value)
{
}

PajeAddVariableEvent::PajeAddVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value)
  : PajeVariableEvent (event, container, type, value)
{
}

PajeSubVariableEvent::PajeSubVariableEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, double value)
  : PajeVariableEvent (event, container, type, value)
{
}

PajeStartLinkEvent::PajeStartLinkEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value, PajeContainer *linkedContainer, std::string key)
  : PajeLinkEvent (event, container, type, value, linkedContainer, key)
{
}

PajeContainer *PajeStartLinkEvent::startContainer (void)
{
  return _linkedContainer;
}

PajeEndLinkEvent::PajeEndLinkEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value, PajeContainer *linkedContainer, std::string key)
  : PajeLinkEvent (event, container, type, value, linkedContainer, key)
{
}

PajeContainer *PajeEndLinkEvent::endContainer (void)
{
  return _linkedContainer;
}

PajeDestroyContainerEvent::PajeDestroyContainerEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type)
  : PajeEvent (event, container, type)
{
}

const std::string &PajeDestroyContainerEvent::kind (void)
{
  static const std::string constr = "Container";
  return constr;
}
