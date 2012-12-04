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

PajeEvent::PajeEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type)
{
  _event = event;
  _container = container;
  _type = type;
}

PajeTraceEvent *PajeEvent::event (void)
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

PajeCategorizedEvent::PajeCategorizedEvent (PajeTraceEvent *event, PajeContainer *container, PajeType *type, PajeValue *value)
  : PajeEvent (event, container, type)
{
  _value = value;
}

PajeValue *PajeCategorizedEvent::value (void)
{
  return _value;
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
