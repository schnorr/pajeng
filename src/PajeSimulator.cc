#include "PajeSimulator.h"

PajeContainer::PajeContainer (std::string name, std::string alias, PajeContainer *parent, PajeType *type)
{
  this->name = name;
  this->alias = alias;
  this->parent = parent;
  this->type = type;
}

PajeContainer *PajeContainer::search (std::string identifier)
{
  if (alias.empty()){
    if (this->name == identifier) return this;
  }else{
    if (this->alias == identifier) return this;
  }

  std::list<PajeContainer*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    PajeContainer *found = (*it)->search(identifier);
    if (found) return found;
  }
  return NULL;
}

void PajeContainer::addContainer (std::string name, std::string alias, PajeType *type)
{
  PajeContainer *newContainer = new PajeContainer (name, alias, this, type);  
  std::list<PajeContainer*>::iterator it = children.end();
  children.insert (it, newContainer);
}

std::ostream &operator<< (std::ostream &output, const PajeContainer &container)
{
  output << "(Container, name: "
         << container.name
         << ", alias: " << container.alias << ")";
  return output;
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
  root = new PajeContainer ("0", "0", NULL, rootType);
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
  std::string starttype = event->valueForFieldId (std::string("StartContainerType"));
  std::string endtype = event->valueForFieldId (std::string("EndContainerType"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //verify presence of obligatory fields
  if (name.empty()) throw "Missing 'Name' field in event";
  if (type.empty()) throw "Missing 'Type' field in event"; 
  if (starttype.empty()) throw "Missing 'StartContainerType' field in event";
  if (endtype.empty()) throw "Missing 'EndContainerType' field in event";

  //search for parent type
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  //search for start container type
  if (!rootType->search (starttype)){
    std::stringstream line;
    line << *event;
    throw "Unknow start container type '"+starttype+"' for link definition in "+line.str();
  }

  //search for end container type
  if (!rootType->search (endtype)){
    std::stringstream line;
    line << *event;
    throw "Unknow end container type '"+endtype+"' for link definition in "+line.str();
  }

  if (!dynamic_cast<PajeContainerType*>(containerType)->addLinkType (name, alias, starttype, endtype)){
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
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string containerid = event->valueForFieldId (std::string("Container"));
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //verify presence of obligatory fields
  if (time.empty()) throw "Missing 'Time' field in event";
  if (type.empty()) throw "Missing 'Type' field in event";
  if (containerid.empty()) throw "Missing 'Container' field in event";
  if (name.empty()) throw "Missing 'Name' field in event";
  if (alias.empty()) throw "Missing 'Alias' field in event";

  //search the container type for the new container
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknown container type '"+type+"' in "+line.str();
  }

  //search the container of the new container
  PajeContainer *container = root->search (containerid);
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerid+"' in "+line.str();
  }

  //verify if the container type is correctly informed
  if (containerType->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *containerType;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Container type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //verify if there is a container with the same name
  std::string identifier = !alias.empty() ? alias : name;  
  PajeContainer *cont = root->search (identifier);
  if (cont){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "(Container, name: '"+name+"' alias: '"+alias+"') already exists in "+eventdesc.str();
  }

  //everything seems ok, create the container
  container->addContainer (name, alias, containerType);
}

void PajeSimulator::pajeDestroyContainer (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string name = event->valueForFieldId (std::string("Name"));

  //verify presence of obligatory fields
  if (time.empty()) throw "Missing 'Time' field in event";
  if (type.empty()) throw "Missing 'Type' field in event";
  if (name.empty()) throw "Missing 'Name' field in event";

  //search the container type for the new container
  PajeType *containerType = rootType->search (type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknown container type '"+type+"' in "+line.str();
  }

  //search the container to be destroyed
  PajeContainer *container = root->search (name);
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+name+"' in "+line.str();
  }

  //checks
  if (container->type != containerType){
    std::stringstream line;
    line << *event;
    std::stringstream cont1;
    cont1 << *container;
    std::stringstream cont2;
    cont2 << *containerType;
    throw "Wrong container type '"+cont2.str()+"' of container '"+cont1.str()+"' in "+line.str();
  }

}

void PajeSimulator::validateVariableStateEvent (PajeEvent *event, bool checkValue)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //verify presence of obligatory fields
  if (time.empty()) throw "Missing 'Time' field in event";
  if (typestr.empty()) throw "Missing 'Type' field in event";
  if (containerstr.empty()) throw "Missing 'Container' field in event";
  if (checkValue && value.empty()) throw "Missing 'Value' field in event";

  //search the container
  PajeContainer *container = root->search (containerstr);
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = rootType->search (typestr);
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //validate the type
  if (type->parent != container->type){
    std::stringstream line;
    line << *event;
    std::stringstream cont1;
    cont1 << *type;
    std::stringstream cont2;
    cont2 << *container->type;
    throw cont1.str()+"' is not a child type of '"+cont2.str()+"' in "+line.str();
  }
}

void PajeSimulator::pajeNewEvent (PajeEvent *event)
{
  validateVariableStateEvent (event, true);
}

void PajeSimulator::pajeSetState (PajeEvent *event)
{
  validateVariableStateEvent (event, true);
}

void PajeSimulator::pajePushState (PajeEvent *event)
{
  validateVariableStateEvent (event, true);
}

void PajeSimulator::pajePopState (PajeEvent *event)
{
  validateVariableStateEvent (event, false);
}

void PajeSimulator::pajeSetVariable (PajeEvent *event)
{
  validateVariableStateEvent (event, true);
}

void PajeSimulator::pajeAddVariable (PajeEvent *event)
{
  validateVariableStateEvent (event, true);
}

void PajeSimulator::pajeSubVariable (PajeEvent *event)
{
  validateVariableStateEvent (event, true);
}

void PajeSimulator::pajeStartLink (PajeEvent *event)
{
}

void PajeSimulator::pajeEndLink (PajeEvent *event)
{
}
