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
#include <iomanip>
#include "PajeEntity.h"
#include "PajeException.h"

int dumpFloatingPointPrecision = 6;

PajeEntity::PajeEntity (PajeContainer *container, PajeType *type, PajeTraceEvent *event)
{
  _container = container;
  _type = type;

  addPajeTraceEvent (event);
}

void PajeEntity::addPajeTraceEvent (PajeTraceEvent *event)
{
  if (!event) return;

  PajeEventDefinition *def = event->definition();
  std::vector<std::string> extra = def->extraFields();
  std::vector<std::string>::iterator it;
  for (it = extra.begin(); it != extra.end(); it++){
    std::string fieldName = *it;
    std::string value = event->valueForExtraField (fieldName);

    //check if fieldName already exists
    if (extraFields.count(fieldName)){
      //if it does, check if value is NOT the same
      if (extraFields[fieldName] != value){
        std::stringstream line;
        line << *event;
        throw PajeDecodeException ("When treating event "+line.str()+", the value for "+fieldName+" is "+extraFields[fieldName]+", but it is different from "+value);
      }
    }else{
      extraFields[fieldName] = value;
    }
  }
}

PajeContainer *PajeEntity::container (void) const
{
  return _container;
}

PajeType *PajeEntity::type (void) const
{
  return _type;
}

const std::string &PajeEntity::name (void) const
{
  static const std::string nullstr = std::string();
  return nullstr;
}

bool PajeEntity::isContainedBy (PajeContainer *container) const
{
  return this->container() == container;
}

bool PajeEntity::isContainer (void) const
{
  return false;
}

PajeValue *PajeEntity::value (void) const
{
  return NULL;
}

void PajeEntity::setDoubleValue (double value)
{
}

void PajeEntity::addDoubleValue (double value)
{
}

void PajeEntity::subtractDoubleValue (double value)
{
}

double PajeEntity::doubleValue (void) const
{
}

PajeContainer *PajeEntity::startContainer (void) const
{
  return container();
}

PajeContainer *PajeEntity::endContainer (void) const
{
  return container();
}

int PajeEntity::imbricationLevel (void) const
{
  return 0;
}

std::string PajeEntity::extraDescription (bool printComma) const
{
  if (extraFields.size() == 0) return std::string();

  std::stringstream description;
  if (printComma){
    description << ", ";
  }
  std::map<std::string,std::string>::const_iterator it;
  for (it = extraFields.begin(); it != extraFields.end(); it++){
    description << (*it).second;

    //look forward to see if we ouput a comma
    it++;
    if (it != extraFields.end()){
      description << ", ";
    }
    it--;
  }
  return description.str();
}

/**************************************************************
 * PajeSingleTimedEntity
 */
PajeSingleTimedEntity::PajeSingleTimedEntity (PajeContainer *container, PajeType *type, double time, PajeTraceEvent *event)
  : PajeEntity (container, type, event)
{
  _stime = time;
}

double PajeSingleTimedEntity::time (void) const
{
  return _stime;
}

double PajeSingleTimedEntity::startTime (void) const
{
  return _stime;
}

double PajeSingleTimedEntity::firstTime (void) const
{
  return startTime();
}

void PajeSingleTimedEntity::setStartTime (double time)
{
  _stime = time;
}

double PajeSingleTimedEntity::endTime (void) const
{
  return startTime();
}

double PajeSingleTimedEntity::lastTime (void) const
{
  return startTime();
}

void PajeSingleTimedEntity::setEndTime (double endTime)
{
  setStartTime (endTime);
}

double PajeSingleTimedEntity::duration (void) const
{
  return 0;
}

/**************************************************************
 * PajeDoubleTimedEntity
 */
PajeDoubleTimedEntity::PajeDoubleTimedEntity (PajeContainer *container, PajeType *type, double time, PajeTraceEvent *event)
  : PajeSingleTimedEntity (container, type, time, event)
{
  _etime = -1;
}

void PajeDoubleTimedEntity::setEndTime (double endTime)
{
  _etime = endTime;
}

double PajeDoubleTimedEntity::endTime (void) const
{
  return _etime;
}

double PajeDoubleTimedEntity::lastTime (void) const
{
  return _etime;
}

double PajeDoubleTimedEntity::duration (void) const
{
  if (endTime() >= 0 && startTime() >= 0){
    return endTime() - startTime();
  }else{
    return 0;
  }
}

/**************************************************************
 * PajeValueEntity
 */
PajeValueEntity::PajeValueEntity (PajeContainer *container, PajeType *type, double time, PajeValue *value, PajeTraceEvent *event)
  : PajeDoubleTimedEntity (container, type, time, event)
{
  _value = value;
}

PajeValue *PajeValueEntity::value (void) const
{
  return _value;
}

/**************************************************************
 * PajeNamedEntity
 */
PajeNamedEntity::PajeNamedEntity (PajeContainer *container, PajeType *type, double time, std::string name, PajeTraceEvent *event)
  : PajeDoubleTimedEntity (container, type, time, event)
{
  _name = name;
}

const std::string &PajeNamedEntity::name (void) const
{
  return _name;
}

/**************************************************************
 * PajeUserEvent
 */
PajeUserEvent::PajeUserEvent (PajeContainer *container, PajeType *type, double time, PajeValue *value, PajeTraceEvent *event)
  : PajeSingleTimedEntity (container, type, time, event)
{
  this->_value = value;
}

std::string PajeUserEvent::description (void) const
{
  std::stringstream description;
  description << (type()? type()->kind() : "NULL") << ", "
              << (container()? container()->name() : "NULL") << ", "
              << (type()? type()->name() : "NULL") << ", "
              << std::fixed << std::setprecision(dumpFloatingPointPrecision)
	      << startTime() << ", "
              << (value()? value()->name() : "NULL");
  return description.str();
}

PajeValue *PajeUserEvent::value (void) const
{
  return _value;
}

/**************************************************************
 * PajeUserState
 */
PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, double startTime, PajeValue *value, PajeTraceEvent *event)
  : PajeValueEntity (container, type, startTime, value, event)
{
  this->imbrication = 0;
}

PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, double startTime, PajeValue *value, int imbric, PajeTraceEvent *event)
  : PajeValueEntity (container, type, startTime, value, event)
{
  this->imbrication = imbric;
}

std::string PajeUserState::description (void) const
{
  std::stringstream description;
  description << (type()? type()->kind() : "NULL") << ", "
              << (container()? container()->name() : "NULL") << ", "
              << (type()? type()->name() : "NULL") << ", "
              << std::fixed << std::setprecision(dumpFloatingPointPrecision)
	      << startTime() << ", "
              << endTime() << ", "
              << duration() << ", "
              << imbrication << ", "
              << (value()? value()->name() : "NULL");
  return description.str();
}

int PajeUserState::imbricationLevel (void) const
{
  return imbrication;
}

/**************************************************************
 * PajeUserVariable
 */
PajeUserVariable::PajeUserVariable (PajeContainer *container, PajeType *type, double time, double value, PajeTraceEvent *event)
  : PajeDoubleTimedEntity (container, type, time, event)
{
  _value = value;
}

std::string PajeUserVariable::description (void) const
{
  std::stringstream description;
  description << (type()? type()->kind() : "NULL") << ", "
              << (container()? container()->name() : "NULL") << ", "
              << (type()? type()->name() : "NULL") << ", "
              << std::fixed << std::setprecision(dumpFloatingPointPrecision)
	      << startTime() << ", "
              << endTime() << ", "
              << duration() << ", "
              << doubleValue();
  return description.str();
}

double PajeUserVariable::doubleValue (void) const
{
  return _value;
}

void PajeUserVariable::setDoubleValue (double value)
{
  _value = value;
}

void PajeUserVariable::addDoubleValue (double value)
{
  _value += value;
}

void PajeUserVariable::subtractDoubleValue (double value)
{
  _value -= value;
}

/**************************************************************
 * PajeUserLink
 */
PajeUserLink::PajeUserLink (PajeContainer *container, PajeType *type, double time, PajeValue *value, std::string key, PajeContainer *startContainer, PajeTraceEvent *event)
  : PajeValueEntity (container, type, time, value, event)
{
  this->key = key;
  this->startCont = startContainer;
  this->endCont = NULL;
}

std::string PajeUserLink::description (void) const
{
  std::stringstream description;
  description << (type()? type()->kind() : "NULL") << ", "
              << (container()? container()->name() : "NULL") << ", "
              << (type()? type()->name() : "NULL") << ", "
              << std::fixed << std::setprecision(dumpFloatingPointPrecision)
	      << startTime() << ", "
              << endTime() << ", "
              << duration() << ", "
              << (value()? value()->name() : "NULL") << ", "
              << (startContainer()? startContainer()->name() : "NULL") << ", "
              << (endContainer()? endContainer()->name() : "NULL");
  return description.str();
}

PajeContainer *PajeUserLink::startContainer (void) const
{
  return startCont;
}

void PajeUserLink::setStartContainer (PajeContainer *startContainer)
{
  this->startCont = startContainer;
}

void PajeUserLink::setEndContainer (PajeContainer *endContainer)
{
  this->endCont = endContainer;
}

PajeContainer *PajeUserLink::endContainer (void) const
{
  return endCont;
}
