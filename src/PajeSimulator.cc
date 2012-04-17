#include "PajeSimulator.h"

PajeSimulator::PajeSimulator ()
{
  invocation[PajeDefineContainerTypeEventId] = &PajeSimulator::pajeDefineContainerType;
  invocation[PajeDefineLinkTypeEventId] = &PajeSimulator::pajeDefineLinkType;
  invocation[PajeDefineEventTypeEventId] = &PajeSimulator::pajeDefineEventType;
  invocation[PajeDefineStateTypeEventId] = &PajeSimulator::pajeDefineStateType;
  invocation[PajeDefineVariableTypeEventId] = &PajeSimulator::pajeDefineVariableType;
  invocation[PajeCreateContainerEventId] = &PajeSimulator::pajeCreateContainer;
  invocation[PajeDestroyContainerEventId] = &PajeSimulator::pajeDestroyContainer;
  invocation[PajeNewEventEventId] = &PajeSimulator::pajeNewEvent;
  invocation[PajeSetStateEventId] = &PajeSimulator::pajeSetState;
  invocation[PajePushStateEventId] = &PajeSimulator::pajePushState;
  invocation[PajePopStateEventId] = &PajeSimulator::pajePopState;
  invocation[PajeSetVariableEventId] = &PajeSimulator::pajeSetVariable;
  invocation[PajeAddVariableEventId] = &PajeSimulator::pajeAddVariable;
  invocation[PajeSubVariableEventId] = &PajeSimulator::pajeSubVariable;
  invocation[PajeStartLinkEventId] = &PajeSimulator::pajeStartLink;
  invocation[PajeEndLinkEventId] = &PajeSimulator::pajeEndLink;
}

void PajeSimulator::inputEntity (PajeObject *data)
{
  PajeEvent *event = (PajeEvent*)data;
  PajeEventId eventId = event->pajeEventId();
  if (eventId < PajeEventIdCount){
    CALL_MEMBER_PAJE_SIMULATOR(*this,invocation[eventId])(event);
  }else{
    throw "Unknow event id.";
  }
}

bool PajeSimulator::canEndChunkBefore (PajeObject *data)
{
  return true;
}

void PajeSimulator::startChunk (int chunkNumber)
{
//  std::cout << __FUNCTION__ << " " << chunkNumber << std::endl;
}

void PajeSimulator::endOfChunkLast (bool last)
{
  // std::cout << __FUNCTION__ << " " << last << std::endl;
}

void PajeSimulator::pajeDefineContainerType (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeDefineLinkType (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeDefineEventType (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeDefineStateType (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeDefineVariableType (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeCreateContainer (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeDestroyContainer (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeNewEvent (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeSetState (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajePushState (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajePopState (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeSetVariable (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeAddVariable (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeSubVariable (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeStartLink (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}

void PajeSimulator::pajeEndLink (PajeEvent *event)
{
  //std::cout << __FUNCTION__ << std::endl;
}
