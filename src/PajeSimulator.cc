#include "PajeSimulator.h"

PajeContainer::PajeContainer (std::string name, std::string alias, PajeContainer *parent, PajeType *type)
{
  this->name = name;
  this->alias = alias;
  this->parent = parent;
  this->type = type;
}

std::string PajeContainer::identifier ()
{
  return alias.empty() ? name : alias;
}

PajeContainer *PajeContainer::addContainer (std::string name, std::string alias, PajeType *type)
{
  PajeContainer *newContainer = new PajeContainer (name, alias, this, type);  
  children[newContainer->identifier()] = newContainer;
  return newContainer;
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
  typeMap[rootType->identifier()] = rootType;
  contMap[root->identifier()] = root;
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

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw "Container type '"+identifier+"' in "+line.str()+" already defined";
  }
  newType = dynamic_cast<PajeContainerType*>(containerType)->addContainerType (name, alias);
  typeMap[newType->identifier()] = newType;
}

void PajeSimulator::pajeDefineLinkType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string starttype = event->valueForFieldId (std::string("StartContainerType"));
  std::string endtype = event->valueForFieldId (std::string("EndContainerType"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  //search for start container type
  PajeType *startcontainertype = typeMap[starttype];
  if (!startcontainertype){
    std::stringstream line;
    line << *event;
    throw "Unknow start container type '"+starttype+"' for link definition in "+line.str();
  }

  //search for end container type
  PajeType *endcontainertype = typeMap[endtype];
  if (!endcontainertype){
    std::stringstream line;
    line << *event;
    throw "Unknow end container type '"+endtype+"' for link definition in "+line.str();
  }

  //check if the new type already exists
  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw "Link type '"+identifier+"' in "+line.str()+" already defined";
  }
  newType = dynamic_cast<PajeContainerType*>(containerType)->addLinkType (name, alias, startcontainertype, endcontainertype);
  typeMap[newType->identifier()] = newType;
}

void PajeSimulator::pajeDefineEventType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw "Event type '"+identifier+"' in "+line.str()+" already defined";
  }
  newType = dynamic_cast<PajeContainerType*>(containerType)->addEventType (name, alias);
  typeMap[newType->identifier()] = newType;
}

void PajeSimulator::pajeDefineStateType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw "State type '"+identifier+"' in "+line.str()+" already defined";
  }
  newType = dynamic_cast<PajeContainerType*>(containerType)->addStateType (name, alias);
  typeMap[newType->identifier()] = newType;
}

void PajeSimulator::pajeDefineVariableType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknow container type '"+type+"' in "+line.str();
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw "Variable type '"+identifier+"' in "+line.str()+" already defined";
  }
  newType = dynamic_cast<PajeContainerType*>(containerType)->addVariableType (name, alias);
  typeMap[newType->identifier()] = newType;
}

void PajeSimulator::pajeDefineEntityValue (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string color = event->valueForFieldId (std::string("Color"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //search for type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknow type '"+typestr+"' in "+line.str();
  }
}

void PajeSimulator::pajeCreateContainer (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string containerid = event->valueForFieldId (std::string("Container"));
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //search the container type for the new container
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknown container type '"+type+"' in "+line.str();
  }

  //search the container of the new container
  PajeContainer *container = contMap[containerid];
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
  PajeContainer *cont = contMap[identifier];
  if (cont){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "(Container, name: '"+name+"' alias: '"+alias+"') already exists in "+eventdesc.str();
  }

  //everything seems ok, create the container
  PajeContainer *newContainer = container->addContainer (name, alias, containerType);
  contMap[newContainer->identifier()] = newContainer;
}

void PajeSimulator::pajeDestroyContainer (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string name = event->valueForFieldId (std::string("Name"));

  //search the container type for the new container
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Unknown container type '"+type+"' in "+line.str();
  }

  //search the container to be destroyed
  PajeContainer *container = contMap[name];
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

void PajeSimulator::pajeNewEvent (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a event type
  if (!dynamic_cast<PajeEventType*>(type)){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a event type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->states[type].size() != 0){
    container->states[type].clear ();
  }
  container->states[type].push_back (0);
}

void PajeSimulator::pajeSetState (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a state type
  if (!dynamic_cast<PajeStateType*>(type)){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a state type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->states[type].size() != 0){
    container->states[type].clear ();
  }
  container->states[type].push_back (0);
}

void PajeSimulator::pajePushState (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a state type
  if (!dynamic_cast<PajeStateType*>(type)){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a state type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->states[type].size() != 0){
    container->states[type].clear ();
  }
  container->states[type].push_back (0);
}

void PajeSimulator::pajePopState (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a state type
  if (!dynamic_cast<PajeStateType*>(type)){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a state type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->states[type].size() == 0){
    std::stringstream line;
    line << *event;
    throw "Illegal pop event of a state that has no value in "+line.str();
  }else{
    container->states[type].clear ();
  }
  container->states[type].push_back (0);
}

void PajeSimulator::pajeSetVariable (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a variable type
  if (!dynamic_cast<PajeVariableType*>(type)){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a variable type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->variables[type].size() != 0){
    container->variables[type].clear ();
  }
  container->variables[type].push_back (0);
}

void PajeSimulator::pajeAddVariable (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a variable type
  if (!dynamic_cast<PajeVariableType*>(type)){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a variable type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->variables[type].size() == 0){
    std::stringstream line;
    line << *event;
    throw "Illegal addition to a variable that has no value (yet) in "+line.str();
  }else{
    container->variables[type].clear ();
  }
  container->variables[type].push_back (0);
}

void PajeSimulator::pajeSubVariable (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string value = event->valueForFieldId (std::string("Value"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a variable type
  if (!dynamic_cast<PajeVariableType*>(type)){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a variable type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->variables[type].size() == 0){
    std::stringstream line;
    line << *event;
    throw "Illegal subtraction from a variable that has no value (yet) in "+line.str();
  }else{
    container->variables[type].clear ();
  }
  container->variables[type].push_back (0);
}

void PajeSimulator::pajeStartLink (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string startcontainerstr = event->valueForFieldId (std::string("StartContainer"));
  std::string value = event->valueForFieldId (std::string("Value"));
  std::string key = event->valueForFieldId (std::string("Key"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the start container
  PajeContainer *startcontainer = contMap[startcontainerstr];
  if (!startcontainer){
    std::stringstream line;
    line << *event;
    throw "Unknown start container '"+startcontainerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a link type
  PajeLinkType *linktype = dynamic_cast<PajeLinkType*>(type);
  if (!linktype){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a link type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //verify if the type of start container is the type expected for the start of this link
  if (linktype->starttype != startcontainer->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *startcontainer->type;
    std::stringstream ctype2;
    ctype2 << *type;
    throw "Type '"+ctype1.str()+"' of container '"+startcontainerstr+"' is not the container type expected for the start of link type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->links[type].count(key) == 0){
    container->links[type][key] = 1;
  }else{
    //key already present, remove it
    container->links[type].erase(key);
  }
}

void PajeSimulator::pajeEndLink (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerstr = event->valueForFieldId (std::string("Container"));
  std::string endcontainerstr = event->valueForFieldId (std::string("EndContainer"));
  std::string value = event->valueForFieldId (std::string("Value"));
  std::string key = event->valueForFieldId (std::string("Key"));

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerstr+"' in "+line.str();
  }

  //search the end container
  PajeContainer *endcontainer = contMap[endcontainerstr];
  if (!endcontainer){
    std::stringstream line;
    line << *event;
    throw "Unknown end container '"+endcontainerstr+"' in "+line.str();
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown type '"+typestr+"' in "+line.str();
  }

  //verify if the type is a link type
  PajeLinkType *linktype = dynamic_cast<PajeLinkType*>(type);
  if (!linktype){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *type;
    throw "Type '"+desc.str()+"' is not a link type in "+line.str();
  }

  //verify if the type is child of container type
  if (type->parent != container->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type;
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //verify if the type of end container is the type expected for the end of this link
  if (linktype->endtype != endcontainer->type){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *endcontainer->type;
    std::stringstream ctype2;
    ctype2 << *type;
    throw "Type '"+ctype1.str()+"' of container '"+endcontainerstr+"' is not the container type expected for the end of link type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  if (container->links[type].count(key) == 0){
    container->links[type][key] = 1;
  }else{
    //key already present, remove it
    container->links[type].erase(key);
  }
}
