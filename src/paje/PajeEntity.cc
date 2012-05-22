#include "PajeEntity.h"

PajeEntity::PajeEntity (PajeContainer *container, PajeType *type, std::string name)
{
  this->container = container;
  this->type = type;
  this->name = name;
}

bool PajeEntity::isContainer (void)
{
  throw "Should be implemented in subclasses";
  return false;
}

std::string PajeEntity::value (void)
{
  throw "Should be implemented in subclasses";
  return std::string();
}

PajeContainer *PajeEntity::startContainer (void)
{
  return container;
}

PajeContainer *PajeEntity::endContainer (void)
{
  return container;
}

bool PajeEntity::isContainedBy (PajeContainer *container)
{
  return this->container == container;
}

double PajeEntity::time (void)
{
  throw "Should be implemented in subclasses";
  return 0;
}

double PajeEntity::startTime (void)
{
  return time();
}

double PajeEntity::endTime (void)
{
  return time();
}

double PajeEntity::firstTime (void)
{
  return startTime () < endTime() ? startTime() : endTime();
}

double PajeEntity::lastTime (void)
{
  return startTime () > endTime() ? startTime() : endTime();
}

double PajeEntity::duration (void)
{
  return endTime() - startTime();
}

double PajeEntity::exclusiveDuration (void)
{
  return duration();
}

double PajeEntity::doubleValue (void)
{
  return 0;
}

double PajeEntity::minValue (void)
{
  return 0;
}

double PajeEntity::maxvalue (void)
{
  return 0;
}

PajeUserEvent::PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, double time):PajeEntity(container, type, name)
{
  this->t = time;
}

void PajeUserEvent::setStartTime (double time)
{
  this->t = time;
}

double PajeUserEvent::startTime (void)
{
  return t;
}

double PajeUserEvent::time (void)
{
  return t;
}

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

PajeUserVariable::PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime):PajeUserState (container, type, "", startTime, endTime)
{
  this->val = value;
}

double PajeUserVariable::doubleValue (void)
{
  return val;
}

double PajeUserVariable::maxValue (void)
{
  return val;
}

double PajeUserVariable::minValue (void)
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

PajeUserLink::PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, double startTime):PajeUserState(container, type, value, startTime, startTime)
{
  this->key = key;
  this->startCont = startContainer;
  this->endCont = NULL;
}

std::string PajeUserLink::value (void)
{
  return name;
}

void PajeUserLink::setStartContainer (PajeContainer *startContainer)
{
  this->startCont = startContainer;
}

void PajeUserLink::setEndContainer (PajeContainer *endContainer)
{
  this->endCont = endContainer;
}

PajeContainer *PajeUserLink::startContainer (void)
{
  return startCont;
}

PajeContainer *PajeUserLink::endContainer (void)
{
  return endCont;
}
