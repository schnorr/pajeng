#include "PajeSimulator.h"

PajeContainerType::PajeContainerType (std::string name, std::string alias)
{
  this->name = name;
  this->alias = alias;
}

PajeContainerType *PajeContainerType::search (std::string identifier)
{
  //check if it's me
  if (alias.empty()){
    if (this->name == identifier){
      return this;
    }
  }else{
    if (this->alias == identifier){
      return this;
    }
  }

  //search on children
  std::list<PajeContainerType*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    PajeContainerType *found = (*it)->search (identifier);
    if (found) return found;
  }
  return NULL;
}

void PajeContainerType::addChild (PajeContainerType *child)
{
  std::list<PajeContainerType*>::iterator it = children.end();
  children.insert (it, child);
}


PajeSimulator::PajeSimulator ()
{
  invocation[PajeDefineContainerTypeEventId] = &PajeSimulator::pajeDefineContainerType;
  invocation[PajeDefineLinkTypeEventId] = &PajeSimulator::pajeDefineLinkType;
  invocation[PajeDefineEventTypeEventId] = &PajeSimulator::pajeDefineEventType;
  invocation[PajeDefineStateTypeEventId] = &PajeSimulator::pajeDefineStateType;
  invocation[PajeDefineVariableTypeEventId] = &PajeSimulator::pajeDefineVariableType;
  invocation[PajeDefineEntityValueEventId] = &PajeSimulator::pajeDefineEntityValue;
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
  rootType = new PajeContainerType ("0", "0");
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
}

void PajeSimulator::endOfChunkLast (bool last)
{
}

void PajeSimulator::pajeDefineContainerType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //verify presence of obligatory fields
  if (name.empty()) throw "Missing 'Name' field in event";
  if (type.empty()) throw "Missing 'Type' field in event"; 

  //search for parent type
  PajeContainerType *containerType = rootType->search (type);
  std::stringstream line;
  line << *event;
  if (!containerType) throw "Unknow container type '"+type+"' in "+line.str();

  //define new container type
  PajeContainerType *newContainerType = new PajeContainerType (name, alias);

  //add it as a child of its container type
  containerType->addChild (newContainerType);
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

void PajeSimulator::pajeDefineEntityValue (PajeEvent *event)
{
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
