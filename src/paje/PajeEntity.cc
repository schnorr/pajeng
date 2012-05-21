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

PajeUserEvent::PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, PajeEvent *event):PajeEntity(container, type, name)
{
}

PajeUserVariable::PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime):PajeEntity (container, type, "")
{
  this->val = value;
  this->stime = startTime;
  this->etime = endTime;
}

double PajeUserVariable::startTime (void)
{
  return stime;
}

double PajeUserVariable::endTime (void)
{
  return etime;
}

double PajeUserVariable::time (void)
{
  return stime;
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

void PajeUserVariable::setStartTime (double startTime)
{
  this->stime = startTime;
}

void PajeUserVariable::setEndTime (double endTime)
{
  this->etime = endTime;
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

PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, std::string value, PajeEvent *startEvent):PajeUserEvent(container, type, value, startEvent)
{
}

PajeUserLink::PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, PajeEvent *startEvent):PajeUserState(container, type, value, startEvent)
{
  this->key = key;
  this->startContainer = startContainer;
}

// PajeUserLink::PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *endContainer, PajeEvent *endEvent)
// {
//   this->key = key;
//   this->endContainer = endContainer;
// }
