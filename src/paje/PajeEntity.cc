#include "PajeEntity.h"

PajeEntity::PajeEntity (PajeContainer *container, PajeType *type, std::string name)
{
  this->entityContainer = container;
  this->entityType = type;
  this->entityName = name;
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

/**************************************************************
 * PajeUserEvent
 */
PajeUserEvent::PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, double time):PajeEntity(container, type, name)
{
  this->t = time;
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
PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, double endTime):PajeUserEvent(container, type, value, startTime)
{
  this->etime = endTime;
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

/**************************************************************
 * PajeUserVariable
 */
PajeUserVariable::PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime):PajeUserState (container, type, "", startTime, endTime)
{
  this->val = value;
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
PajeUserLink::PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, double startTime):PajeUserState(container, type, value, startTime, startTime)
{
  this->key = key;
  this->startCont = startContainer;
  this->endCont = NULL;
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
