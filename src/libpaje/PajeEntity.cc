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
#include "PajeEntity.h"

PajeEntity::PajeEntity (PajeContainer *container, PajeType *type, std::string name, PajeEvent *event)
{
  this->entityContainer = container;
  this->entityType = type;
  this->entityName = name;

  addPajeEvent (event);
}

void PajeEntity::addPajeEvent (PajeEvent *event)
{
  if (!event) return;

  PajeEventDefinition *def = event->pajeEventDefinition;
  std::vector<std::string> extra = def->extraFields();
  std::vector<std::string>::iterator it;
  for (it = extra.begin(); it != extra.end(); it++){
    std::string fieldName = *it;
    std::string value = event->valueForFieldId (fieldName);

    //check if fieldName already exists
    if (extraFields.count(fieldName)){
      //if it does, check if value is NOT the same
      if (extraFields[fieldName] != value){
        std::stringstream line;
        line << *event;
        throw "When treating event "+line.str()+", the value for "+fieldName+" is "+extraFields[fieldName]+", but it is different from "+value;
      }
    }else{
      extraFields[fieldName] = value;
    }
  }
}

PajeContainer *PajeEntity::container (void) const
{
  return entityContainer;
}

PajeType *PajeEntity::type (void) const
{
  return entityType;
}

std::string PajeEntity::name (void) const
{
  return entityName;
}

void PajeEntity::setName (std::string newname)
{
  this->entityName = newname;
}

bool PajeEntity::isContainedBy (PajeContainer *container)
{
  return this->container() == container;
}

bool PajeEntity::isContainer (void)
{
  return false;
}

std::string PajeEntity::value (void)
{
  return name();
}

std::string PajeEntity::extraDescription (void)
{
  if (extraFields.size() == 0) return std::string("");

  std::stringstream description;
  std::map<std::string,std::string>::iterator it;
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

void PajeEntity::setValue (std::string newvalue)
{
  throw "should be implemented in subclass";
}

void PajeEntity::setDoubleValue (double value)
{
  throw "should be implemented in subclass";
}

void PajeEntity::addDoubleValue (double value)
{
  setDoubleValue (value);
}

void PajeEntity::subtractDoubleValue (double value)
{
  setDoubleValue (value);
}

double PajeEntity::doubleValue (void)
{
  return 0;
}

PajeContainer *PajeEntity::startContainer (void)
{
  return container();
}

PajeContainer *PajeEntity::endContainer (void)
{
  return container();
}

int PajeEntity::imbricationLevel (void)
{
  return 0;
}

/**************************************************************
 * PajeUserEvent
 */
PajeUserEvent::PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, double time, PajeEvent *event):PajeEntity(container, type, name, event)
{
  this->t = time;
}

std::string PajeUserEvent::description (void)
{
  std::stringstream description;
  description << "Event, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << name();
  std::string extra = extraDescription();
  if (!extra.empty()){
    description << ", "
                << extra;
  }
  return description.str();
}

double PajeUserEvent::time (void)
{
  return t;
}

double PajeUserEvent::startTime (void)
{
  return t;
}

void PajeUserEvent::setStartTime (double time)
{
  this->t = time;
}

double PajeUserEvent::endTime (void)
{
  return startTime();
}

void PajeUserEvent::setEndTime (double endTime)
{
  setStartTime (endTime);
}

double PajeUserEvent::firstTime (void)
{
  return startTime();
}

double PajeUserEvent::lastTime (void)
{
  return startTime();
}

double PajeUserEvent::duration (void)
{
  return 0;
}

/**************************************************************
 * PajeUserState
 */
PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, PajeEvent *event):PajeUserEvent(container, type, value, startTime, event)
{
  this->etime = -1;
  this->imbrication = 0;
}

PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, int imbric, PajeEvent *event):PajeUserEvent(container, type, value, startTime, event)
{
  this->etime = -1;
  this->imbrication = imbric;
}

std::string PajeUserState::description (void)
{
  std::stringstream description;
  description << "State, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << endTime() << ", "
              << name();
  std::string extra = extraDescription();
  if (!extra.empty()){
    description << ", "
                << extra;
  }
  return description.str();
}

void PajeUserState::setValue (std::string newvalue)
{
  setName (newvalue);
}

void PajeUserState::setEndTime (double endTime)
{
  this->etime = endTime;
}

double PajeUserState::endTime (void)
{
  return etime;
}

double PajeUserState::lastTime (void)
{
  return etime;
}

double PajeUserState::duration (void)
{
  return endTime() - startTime();
}

int PajeUserState::imbricationLevel (void)
{
  return imbrication;
}

/**************************************************************
 * PajeUserVariable
 */
PajeUserVariable::PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime, PajeEvent *event):PajeUserState (container, type, "", startTime, endTime, event)
{
  this->val = value;
}

std::string PajeUserVariable::description (void)
{

  std::stringstream description;
  description << "Variable, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << endTime() << ", "
              << doubleValue();
  std::string extra = extraDescription();
  if (!extra.empty()){
    description << ", "
                << extra;
  }
  return description.str();
}

double PajeUserVariable::doubleValue (void)
{
  return val;
}

void PajeUserVariable::setDoubleValue (double value)
{
  this->val = value;
}

void PajeUserVariable::addDoubleValue (double value)
{
  this->val += value;
}

void PajeUserVariable::subtractDoubleValue (double value)
{
  this->val -= value;
}

/**************************************************************
 * PajeUserLink
 */
PajeUserLink::PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, double startTime, PajeEvent *event):PajeUserState(container, type, value, startTime, startTime, event)
{
  this->key = key;
  this->startCont = startContainer;
  this->endCont = NULL;
}

std::string PajeUserLink::description (void)
{
  std::stringstream description;
  description << "Link, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << endTime() << ", "
              << value() << ", "
              << startContainer()->name() << ", "
              << endContainer()->name();
  std::string extra = extraDescription();
  if (!extra.empty()){
    description << ", "
                << extra;
  }
  return description.str();
}

PajeContainer *PajeUserLink::startContainer (void)
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

PajeContainer *PajeUserLink::endContainer (void)
{
  return endCont;
}
