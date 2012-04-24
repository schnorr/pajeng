#include "PajeSimulator.h"

PajeType::PajeType (std::string name, std::string alias, PajeType *parent)
{
  this->name = name;
  this->alias = alias;
  this->parent = parent;
}

bool PajeType::isContainer (void)
{
  return false;
}

PajeType *PajeType::search (std::string identifier)
{
  if (alias.empty()){
    if (this->name == identifier) return this;
  }else{
    if (this->alias == identifier) return this;
  }
  return NULL;
}

PajeContainerType::PajeContainerType (std::string name, std::string alias, PajeType *parent):
  PajeType (name, alias, parent)
{
}

PajeType *PajeContainerType::search (std::string identifier)
{
  PajeType *found = PajeType::search(identifier);
  if (found) return found;

  //search on children
  std::list<PajeType*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    PajeContainerType* containerchild = dynamic_cast<PajeContainerType*>(*it);
    if (containerchild){
      PajeType *found = containerchild->search (identifier);
      if (found) return found;
    }else{
      PajeType *found = (*it)->search (identifier);
      if (found) return found;
    }
  }
  return NULL;
}

PajeType *PajeContainerType::getRootType (void)
{
  PajeType *root = this;
  while (root != NULL){
    if (root->parent != NULL) {
      root = root->parent;
    }else{
      break;
    }
  }
  return root;
}

void PajeContainerType::addChild (PajeType *child)
{
  std::list<PajeType*>::iterator it = children.end();
  children.insert (it, child);
}

bool PajeContainerType::isContainer(void)
{
  return true;
}

bool PajeContainerType::addContainerType (std::string name, std::string alias)
{
  PajeType *rootType = getRootType ();

  //check if already exists
  std::string identifier;
  identifier = !alias.empty() ? alias : name;
  PajeType *existing = getRootType()->search (identifier);
  if (existing) return false;

  //define new container type
  PajeContainerType *newContainerType = new PajeContainerType (name, alias, this);

  //add it as a child of its container type
  addChild (newContainerType);
  return true;
}

bool PajeContainerType::addType (std::string name, std::string alias)
{
  //check if already exists
  std::string identifier;
  identifier = !alias.empty() ? alias : name;
  PajeType *existing = getRootType()->search (identifier);
  if (existing) return false;

  //define new variable type
  PajeType *newType = new PajeType (name, alias, this);

  //add it as a child of its container type
  addChild (newType);
  return true;
}

bool PajeContainerType::addVariableType (std::string name, std::string alias)
{
  return addType (name, alias);
}

bool PajeContainerType::addStateType (std::string name, std::string alias)
{
  return addType (name, alias);
}

bool PajeContainerType::addEventType (std::string name, std::string alias)
{
  return addType (name, alias);
}

bool PajeContainerType::addLinkType (std::string name, std::string alias)
{
  return addType (name, alias);
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
  rootType = new PajeContainerType ("0", "0", NULL);
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
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  if (!dynamic_cast<PajeContainerType*>(containerType)->addContainerType (name, alias)){
    std::string identifier = !alias.empty() ? alias : name;
    std::stringstream line;
    line << *event;
    throw "Container type '"+identifier+"' in "+line.str()+" already defined";
  }
}

void PajeSimulator::pajeDefineLinkType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //verify presence of obligatory fields
  if (name.empty()) throw "Missing 'Name' field in event";
  if (type.empty()) throw "Missing 'Type' field in event"; 

  //search for parent type
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  if (!dynamic_cast<PajeContainerType*>(containerType)->addLinkType (name, alias)){
    std::string identifier = !alias.empty() ? alias : name;
    std::stringstream line;
    line << *event;
    throw "Link type '"+identifier+"' in "+line.str()+" already defined";
  }
}

void PajeSimulator::pajeDefineEventType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //verify presence of obligatory fields
  if (name.empty()) throw "Missing 'Name' field in event";
  if (type.empty()) throw "Missing 'Type' field in event"; 

  //search for parent type
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  if (!dynamic_cast<PajeContainerType*>(containerType)->addEventType (name, alias)){
    std::string identifier = !alias.empty() ? alias : name;
    std::stringstream line;
    line << *event;
    throw "Event type '"+identifier+"' in "+line.str()+" already defined";
  }
}

void PajeSimulator::pajeDefineStateType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //verify presence of obligatory fields
  if (name.empty()) throw "Missing 'Name' field in event";
  if (type.empty()) throw "Missing 'Type' field in event"; 

  //search for parent type
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  if (!dynamic_cast<PajeContainerType*>(containerType)->addStateType (name, alias)){
    std::string identifier = !alias.empty() ? alias : name;
    std::stringstream line;
    line << *event;
    throw "State type '"+identifier+"' in "+line.str()+" already defined";
  }
}

void PajeSimulator::pajeDefineVariableType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //verify presence of obligatory fields
  if (name.empty()) throw "Missing 'Name' field in event";
  if (type.empty()) throw "Missing 'Type' field in event"; 

  //search for parent type
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  if (!dynamic_cast<PajeContainerType*>(containerType)->addVariableType (name, alias)){
    std::string identifier = !alias.empty() ? alias : name;
    std::stringstream line;
    line << *event;
    throw "Variable type '"+identifier+"' in "+line.str()+" already defined";
  }
}

void PajeSimulator::pajeDefineEntityValue (PajeEvent *event)
{
  std::cout << __FUNCTION__ << " TBI" << std::endl;
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
