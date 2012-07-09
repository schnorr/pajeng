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
#include "PajeSimulator.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

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
  invocation[PajeResetStateEventId] = &PajeSimulator::pajeResetState;
  invocation[PajeSetVariableEventId] = &PajeSimulator::pajeSetVariable;
  invocation[PajeAddVariableEventId] = &PajeSimulator::pajeAddVariable;
  invocation[PajeSubVariableEventId] = &PajeSimulator::pajeSubVariable;
  invocation[PajeStartLinkEventId] = &PajeSimulator::pajeStartLink;
  invocation[PajeEndLinkEventId] = &PajeSimulator::pajeEndLink;
  rootType = new PajeContainerType ("0", "0", NULL);
  root = new PajeContainer (0, "0", "0", NULL, rootType, NULL);
  typeMap[rootType->identifier()] = rootType;
  typeNamesMap[rootType->name] = rootType;
  contMap[root->identifier()] = root;
  contNamesMap[root->name()] = root;
  lastKnownTime = -1;

  selectionStart = -1;
  selectionEnd = -1;
}

void PajeSimulator::report (void)
{
  std::cout << __FUNCTION__ << " Containers: " << contMap.size() << std::endl;
  std::cout << __FUNCTION__ << " Types: " << typeMap.size() <<  std::endl;
  std::cout << __FUNCTION__ << " Type Hierarchy:" << std::endl;

  std::vector<PajeType*> stack;
  stack.push_back (rootType);
  while (stack.size()){
    PajeType *last = stack.back();
    stack.pop_back();
    std::string name = typeid(*last).name();
    printf ("%s |%*.*s%s (%s)\n", __FUNCTION__, last->depth, last->depth, "| ", last->name.c_str(), name.c_str());

    //push back more types
    if (this->isContainerType (last)){
      std::vector<PajeType*> children = this->containedTypesForContainerType(last);
      while (children.size()){
        PajeType *x = children.back();
        stack.push_back (x);
        children.pop_back();
      }
    }
  }
}

void PajeSimulator::setLastKnownTime (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  if (time.length()){
    double evttime = atof(time.c_str());
    lastKnownTime = evttime;
  }
}

PajeColor *PajeSimulator::getColor (std::string color, PajeEvent *event)
{
  PajeColor *ret = NULL;
  if (!color.empty()){
    std::vector<float> v;
    boost::char_separator<char> sep(", ");
    boost::tokenizer< boost::char_separator<char> > tokens(color, sep);
    BOOST_FOREACH(std::string t, tokens) {
      v.push_back (atof(t.c_str()));
    }
    if (v.size()==3){
      ret = new PajeColor (v[0], v[1], v[2], 1);
    }else if (v.size()==4){
        ret = new PajeColor (v[0], v[1], v[2], v[3]);
    }else{
      std::stringstream line;
      line << *event;
      throw "Could not understand color parameter in "+line.str();
    }
  }
  return ret;
}

PajeSimulator::~PajeSimulator ()
{
  delete root;
  delete rootType;
  typeMap.clear ();
  typeNamesMap.clear ();
  contMap.clear();
  contNamesMap.clear();
}

void PajeSimulator::inputEntity (PajeObject *data)
{
  PajeEvent *event = (PajeEvent*)data;
  setLastKnownTime (event);
  PajeEventId eventId = event->pajeEventId();
  if (eventId < PajeEventIdCount){
    if (invocation[eventId]){
      CALL_MEMBER_PAJE_SIMULATOR(*this,invocation[eventId])(event);
    }
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
  if (last){
    //file has ended, mark all containers as destroyed
    root->recursiveDestroy (lastKnownTime, NULL);
    hierarchyChanged ();
    timeLimitsChanged ();
    setSelectionStartEndTime (startTime(), endTime());
  }
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
  typeNamesMap[newType->name] = newType;
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
  typeNamesMap[newType->name] = newType;
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
  typeNamesMap[newType->name] = newType;
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
  typeNamesMap[newType->name] = newType;
}

void PajeSimulator::pajeDefineVariableType (PajeEvent *event)
{
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string type = event->valueForFieldId (std::string("Type"));
  std::string alias = event->valueForFieldId (std::string("Alias"));
  std::string color = event->valueForFieldId (std::string("Color"));

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

  //validate the color, if provided
  PajeColor *pajeColor = getColor (color, event);

  newType = dynamic_cast<PajeContainerType*>(containerType)->addVariableType (name, alias, pajeColor);
  typeMap[newType->identifier()] = newType;
  typeNamesMap[newType->name] = newType;
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

  //check if the type accepts values
  if (this->isContainerType (type)){
    std::stringstream line;
    line << *event;
    throw "Trying to define the value '"+name+"' for the type '"+typestr+"' (which is a container type) is invalid in "+line.str();
  }

  if (this->isVariableType (type)){
    std::stringstream line;
    line << *event;
    throw "Trying to define the value '"+name+"' for the type '"+typestr+"' (which is a variable type) is invalid in "+line.str();
  }

  //check if the value already exists using the alias or the name
  if (!alias.empty() && type->hasValueForIdentifier (alias)){
    std::stringstream line;
    line << *event;
    throw "Trying to redefine the value identified by '"+alias+"' for the type '"+typestr+"' in "+line.str();
  }else if (type->hasValueForIdentifier (name)){
    std::stringstream line;
    line << *event;
    throw "Trying to redefine the value identified by '"+name+"' for the type '"+typestr+"' in "+line.str();
  }

  //validate the color, if provided
  PajeColor *pajeColor = getColor (color, event);

  type->addNewValue (alias, name, pajeColor);
}

void PajeSimulator::pajeCreateContainer (PajeEvent *event)
{
  std::string time = event->valueForFieldId (std::string("Time"));
  std::string typestr = event->valueForFieldId (std::string("Type"));
  std::string containerid = event->valueForFieldId (std::string("Container"));
  std::string name = event->valueForFieldId (std::string("Name"));
  std::string alias = event->valueForFieldId (std::string("Alias"));

  //search the container type for the new container
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw "Unknown container type '"+typestr+"' in "+line.str();
  }

  PajeContainerType *containerType = dynamic_cast<PajeContainerType*>(type);
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw "Not a container type '"+typestr+"' in "+line.str();
  }

  //search the container of the new container
  PajeContainer *container = contMap[containerid];
  if (!container){
    std::stringstream line;
    line << *event;
    throw "Unknown container '"+containerid+"' in "+line.str();
  }

  //verify if the container type is correctly informed
  if (containerType->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *containerType;
    std::stringstream ctype2;
    ctype2 << *container->type();
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
  PajeContainer *newContainer = container->addContainer (lastKnownTime, name, alias, containerType, event);
  contMap[newContainer->identifier()] = newContainer;
  contNamesMap[newContainer->name()] = newContainer;
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
  if (container->type() != containerType){
    std::stringstream line;
    line << *event;
    std::stringstream cont1;
    cont1 << *container;
    std::stringstream cont2;
    cont2 << *containerType;
    std::stringstream cont3;
    cont3 << *container->type();
    throw "Wrong container type '"+cont2.str()+"' of container '"+cont1.str()+"' with type '"+cont3.str()+"' in "+line.str();
  }

  //mark container as destroyed
  container->destroy (lastKnownTime, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //check if the value was previously declared
  if (type->hasValueForIdentifier (value)){
    value = type->valueForIdentifier (value);
  }

  container->newEvent (lastKnownTime, type, value, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //check if the value was previously declared
  if (type->hasValueForIdentifier (value)){
    value = type->valueForIdentifier (value);
  }

  container->setState (lastKnownTime, type, value, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //check if the value was previously declared
  if (type->hasValueForIdentifier (value)){
    value = type->valueForIdentifier (value);
  }

  container->pushState (lastKnownTime, type, value, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  container->popState (lastKnownTime, type, value, event);
}


void PajeSimulator::pajeResetState (PajeEvent *event)
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  container->resetState (lastKnownTime, type, value, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  float v = strtof (value.c_str(), NULL);
  container->setVariable (lastKnownTime, type, v, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  float v = strtof (value.c_str(), NULL);
  container->addVariable (lastKnownTime, type, v, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  float v = strtof (value.c_str(), NULL);
  container->subVariable (lastKnownTime, type, v, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //verify if the type of start container is the type expected for the start of this link
  if (linktype->starttype != startcontainer->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *startcontainer->type();
    std::stringstream ctype2;
    ctype2 << *type;
    throw "Type '"+ctype1.str()+"' of container '"+startcontainerstr+"' is not the container type expected for the start of link type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //check if the value was previously declared
  if (type->hasValueForIdentifier (value)){
    value = type->valueForIdentifier (value);
  }

  float v = strtof (value.c_str(), NULL);
  container->startLink (lastKnownTime, type, startcontainer, value, key, event);
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
  if (type->parent != container->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw "Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //verify if the type of end container is the type expected for the end of this link
  if (linktype->endtype != endcontainer->type()){
    std::stringstream eventdesc;
    eventdesc << *event;
    std::stringstream ctype1;
    ctype1 << *endcontainer->type();
    std::stringstream ctype2;
    ctype2 << *type;
    throw "Type '"+ctype1.str()+"' of container '"+endcontainerstr+"' is not the container type expected for the end of link type '"+ctype2.str()+"' in "+eventdesc.str();
  }

  //check if the value was previously declared
  if (type->hasValueForIdentifier (value)){
    value = type->valueForIdentifier (value);
  }

  float v = strtof (value.c_str(), NULL);
  container->endLink (lastKnownTime, type, endcontainer, value, key, event);
}
