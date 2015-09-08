/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeSimulator.h"
#include "PajeException.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

int ignoreIncompleteLinks = 0;

PajeSimulator::PajeSimulator ()
{
  stopSimulationAtTime = -1;
  init ();
}

PajeSimulator::PajeSimulator (double stopat)
{
  stopSimulationAtTime = stopat;
  init ();
}

PajeSimulator::PajeSimulator (double stopat, int ignore)
{
  stopSimulationAtTime = stopat;
  ignoreIncompleteLinks = ignore;
  init ();
}

void PajeSimulator::init (void)
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
  invocation[PajeTraceFileEventId] = &PajeSimulator::pajeTraceFile;
  rootType = new PajeContainerType ("0", "0", NULL);
  root = new PajeContainer (0, "0", "0", NULL, rootType, NULL);
  typeMap[rootType->identifier()] = rootType;
  typeNamesMap[rootType->name()] = rootType;
  contMap[root->identifier()] = root;
  contNamesMap[root->name()] = root;
  lastKnownTime = -1;

  selectionStart = -1;
  selectionEnd = -1;
}

void PajeSimulator::report (void)
{
  std::cout << __FUNCTION__ << " Containers: " << contMap.size() << std::endl;
  std::cout << __FUNCTION__ << " Entities: " << root->numberOfEntities () << std::endl;
  std::cout << __FUNCTION__ << " Types: " << typeMap.size() <<  std::endl;
  std::cout << __FUNCTION__ << " Type Hierarchy:" << std::endl;

  std::vector<PajeType*> stack;
  stack.push_back (rootType);
  while (!stack.empty()){
    PajeType *last = stack.back();
    stack.pop_back();
    std::string name = typeid(*last).name();
    printf ("%s |%*.*s%s (%s)\n", __FUNCTION__, last->depth(), last->depth(), "| ", last->name().c_str(), name.c_str());

    //push back more types
    if (this->isContainerType (last)){
      std::vector<PajeType*> children = this->containedTypesForContainerType(last);
      while (!children.empty()){
        PajeType *x = children.back();
        stack.push_back (x);
        children.pop_back();
      }
    }
  }
}

static const char *getDotShape (PajeTypeNature nature)
{
  switch(nature){
  case PAJE_ContainerType:
    return "folder";
  case PAJE_VariableType:
    return "plaintext";
  case PAJE_StateType:
    return "rectangle";
  case PAJE_EventType:
    return "oval";
  case PAJE_LinkType:
    return "rarrow";
  case PAJE_UndefinedType:
  default:
    return "point";

  }
  return NULL;
}

void PajeSimulator::reportDotFormat (void)
{
  printf("digraph PajeTypes {\n");
  std::vector<PajeType*> stack;
  stack.push_back (rootType);
  while (!stack.empty()){
    PajeType *last = stack.back();
    stack.pop_back();

    const char *shape = getDotShape (last->nature());
    printf("\"%s\" [shape=%s, label=\"%s (%s)\"];\n", last->name().c_str(), shape, last->name().c_str(), last->alias().c_str());

    //push back more types
    if (this->isContainerType (last)){
      std::vector<PajeType*> children = this->containedTypesForContainerType(last);
      while (!children.empty()){
        PajeType *x = children.back();
	printf ("\"%s\" -> \"%s\";\n", last->name().c_str(), x->name().c_str());
        stack.push_back (x);
        children.pop_back();
      }
    }
  }
  printf("}\n");
}

void PajeSimulator::reportContainer (void)
{
  std::vector<PajeContainer*> stack;
  stack.push_back (rootInstance());
  while (!stack.empty()){
    PajeContainer *last = stack.back();
    stack.pop_back();

    std::string name = last->name();
    printf ("%s |%*.*s%s (%s)\n", __FUNCTION__, last->type()->depth(), last->type()->depth(), "| ", last->name().c_str(), name.c_str());

    //push back more containers
    std::vector<PajeContainer*> children = this->enumeratorOfContainersInContainer(last);
    while (!children.empty()){
      stack.push_back(children.back());
      children.pop_back();
    }
  }
}

bool PajeSimulator::keepSimulating (void)
{
  if (stopSimulationAtTime == -1){
    return true;
  }else{
    return root->keepSimulating ();
  }
}

void PajeSimulator::setLastKnownTime (PajeTraceEvent *event)
{
  std::string time = event->valueForField (PAJE_Time);
  if (time.length()){
    double evttime = atof(time.c_str());
    lastKnownTime = evttime;
  }
}

PajeColor *PajeSimulator::getColor (std::string color, PajeTraceEvent *event)
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
      throw PajeDecodeException ("Could not understand color parameter in "+line.str());
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
  //get event, set last known time
  PajeTraceEvent *event = (PajeTraceEvent*)data;
  setLastKnownTime (event);
  //change the simulated behavior according to the event
  PajeEventId eventId = event->pajeEventId();
  if (eventId < PajeEventIdCount){
    if (invocation[eventId]){
      CALL_MEMBER_PAJE_SIMULATOR(*this,invocation[eventId])(event);
    }
  }else{
    throw PajeSimulationException ("Unknow event id.");
  }
}

void PajeSimulator::startReading (void)
{
}

void PajeSimulator::finishedReading (void)
{
  //file has ended, mark all containers as destroyed
  if (stopSimulationAtTime == -1){
    root->recursiveDestroy (lastKnownTime);
  }else{
    root->recursiveDestroy (stopSimulationAtTime);
  }
  hierarchyChanged ();
  timeLimitsChanged ();
  setSelectionStartEndTime (startTime(), endTime());
}

void PajeSimulator::pajeDefineContainerType (PajeTraceEvent *event)
{
  std::string name = event->valueForField (PAJE_Name);
  std::string type = event->valueForField (PAJE_Type);
  std::string alias = event->valueForField (PAJE_Alias);

  //first, check if the name is allowed (it should be
  //anything but "0" (the zero character)
  if (name == "0"){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("The type name '0' is reserved and should not be used in "+line.str());
  }

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow container type '"+type+"' in "+line.str());
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Container type '"+identifier+"' in "+line.str()+" already defined.");
  }
  newType = containerType->addContainerType (name, alias);
  typeMap[newType->identifier()] = newType;
  typeNamesMap[newType->name()] = newType;
}

void PajeSimulator::pajeDefineLinkType (PajeTraceEvent *event)
{
  std::string name = event->valueForField (PAJE_Name);
  std::string type = event->valueForField (PAJE_Type);
  std::string starttype = event->valueForField (PAJE_StartContainerType);
  std::string endtype = event->valueForField (PAJE_EndContainerType);
  std::string alias = event->valueForField (PAJE_Alias);

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow container type '"+type+"' in "+line.str());
  }

  //search for start container type
  PajeType *startcontainertype = typeMap[starttype];
  if (!startcontainertype){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow start container type '"+starttype+"' for link definition in "+line.str());
  }

  //search for end container type
  PajeType *endcontainertype = typeMap[endtype];
  if (!endcontainertype){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow end container type '"+endtype+"' for link definition in "+line.str());
  }

  //check if the new type already exists
  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Link type '"+identifier+"' in "+line.str()+" already defined");
  }
  newType = containerType->addLinkType (name, alias, startcontainertype, endcontainertype);
  typeMap[newType->identifier()] = newType;
  typeNamesMap[newType->name()] = newType;
}

void PajeSimulator::pajeDefineEventType (PajeTraceEvent *event)
{
  std::string name = event->valueForField (PAJE_Name);
  std::string type = event->valueForField (PAJE_Type);
  std::string alias = event->valueForField (PAJE_Alias);

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow container type '"+type+"' in "+line.str());
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Event type '"+identifier+"' in "+line.str()+" already defined");
  }
  newType = containerType->addEventType (name, alias);
  typeMap[newType->identifier()] = newType;
  typeNamesMap[newType->name()] = newType;
}

void PajeSimulator::pajeDefineStateType (PajeTraceEvent *event)
{
  std::string name = event->valueForField (PAJE_Name);
  std::string type = event->valueForField (PAJE_Type);
  std::string alias = event->valueForField (PAJE_Alias);

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow container type '"+type+"' in "+line.str());
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("State type '"+identifier+"' in "+line.str()+" already defined");
  }
  newType = containerType->addStateType (name, alias);
  typeMap[newType->identifier()] = newType;
  typeNamesMap[newType->name()] = newType;
}

void PajeSimulator::pajeDefineVariableType (PajeTraceEvent *event)
{
  std::string name = event->valueForField (PAJE_Name);
  std::string type = event->valueForField (PAJE_Type);
  std::string alias = event->valueForField (PAJE_Alias);
  std::string color = event->valueForField (PAJE_Color);

  //search for parent type
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow container type '"+type+"' in "+line.str());
  }

  std::string identifier = !alias.empty() ? alias : name;
  PajeType *newType = typeMap[identifier];
  if (newType){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Variable type '"+identifier+"' in "+line.str()+" already defined");
  }

  //validate the color, if provided
  PajeColor *pajeColor = getColor (color, event);

  newType = containerType->addVariableType (name, alias, pajeColor);
  typeMap[newType->identifier()] = newType;
  typeNamesMap[newType->name()] = newType;
}

void PajeSimulator::pajeDefineEntityValue (PajeTraceEvent *event)
{
  std::string name = event->valueForField (PAJE_Name);
  std::string typestr = event->valueForField (PAJE_Type);
  std::string color = event->valueForField (PAJE_Color);
  std::string alias = event->valueForField (PAJE_Alias);

  //search for type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Unknow type '"+typestr+"' in "+line.str());
  }

  //check if the type accepts values
  if (this->isContainerType (type)){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Trying to define the value '"+name+"' for the type '"+typestr+"' (which is a container type) is invalid in "+line.str());
  }

  if (this->isVariableType (type)){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Trying to define the value '"+name+"' for the type '"+typestr+"' (which is a variable type) is invalid in "+line.str());
  }

  //check if the value already exists using the alias or the name
  if (!alias.empty() && type->hasValueForIdentifier (alias)){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Trying to redefine the value identified by '"+alias+"' for the type '"+typestr+"' in "+line.str());
  }else if (type->hasValueForIdentifier (name)){
    std::stringstream line;
    line << *event;
    throw PajeTypeException ("Trying to redefine the value identified by '"+name+"' for the type '"+typestr+"' in "+line.str());
  }

  //validate the color, if provided
  PajeColor *pajeColor = getColor (color, event);

  type->addValue (alias, name, pajeColor);
}

void PajeSimulator::pajeCreateContainer (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerid = traceEvent->valueForField (PAJE_Container);
  std::string name = traceEvent->valueForField (PAJE_Name);
  std::string alias = traceEvent->valueForField (PAJE_Alias);

  //first, check if the name is allowed (it should be
  //anything but "0" (the zero character)
  std::string identifier_check = !alias.empty() ? alias : name;
  if (identifier_check == "0" && contMap.size() > 1){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("The container name '0' is reserved and should not be used in "+line.str());
  }

  //search the container type for the new container
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown container type '"+typestr+"' in "+line.str());
  }

  if (type->nature() != PAJE_ContainerType){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Not a container type '"+typestr+"' in "+line.str());
  }

  //search the container of the new container
  PajeContainer *container = contMap[containerid];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerid+"' in "+line.str());
  }

  //verify if the container type is correctly informed
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Container type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //verify if there is a container with the same name
  std::string identifier = !alias.empty() ? alias : name;  
  PajeContainer *cont = contMap[identifier];
  if (cont){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    throw PajeContainerException ("(Container, name: '"+name+"' alias: '"+alias+"') already exists in "+eventdesc.str());
  }

  //everything seems ok, create the container
  PajeContainer *newContainer = container->pajeCreateContainer (lastKnownTime, type, traceEvent, stopSimulationAtTime);
  if (newContainer){
    contMap[newContainer->identifier()] = newContainer;
    contNamesMap[newContainer->name()] = newContainer;
  }else{
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    throw PajeContainerException ("(Container, name: '"+name+"' alias: '"+alias+"') could not be created in "+eventdesc.str());
  }
}

void PajeSimulator::pajeDestroyContainer (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string type = traceEvent->valueForField (PAJE_Type);
  std::string name = traceEvent->valueForField (PAJE_Name);

  //search the container type for the new container
  PajeType *containerType = typeMap[type];
  if (!containerType){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown container type '"+type+"' in "+line.str());
  }

  //search the container to be destroyed
  PajeContainer *container = contMap[name];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+name+"' in "+line.str());
  }

  //checks
  if (container->type() != containerType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream cont1;
    cont1 << *container;
    std::stringstream cont2;
    cont2 << *containerType;
    std::stringstream cont3;
    cont3 << *container->type();
    throw PajeTypeException ("Wrong container type '"+cont2.str()+"' of container '"+cont1.str()+"' with type '"+cont3.str()+"' in "+line.str());
  }

  //mark container as destroyed
  PajeDestroyContainerEvent event (traceEvent, container, containerType);
  container->demuxer (&event);
}

void PajeSimulator::pajeNewEvent (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string value = traceEvent->valueForField (PAJE_Value);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a event type
  if (type->nature() != PAJE_EventType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a event type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //check if the value was previously declared
  PajeValue *val = NULL;
  if (type->hasValueForIdentifier (value)){
    val = type->valueForIdentifier (value);
  }else{
    val = type->addValue (value, value, NULL);
  }

  PajeNewEventEvent event (traceEvent, container, type, val);
  container->demuxer (&event);
}

void PajeSimulator::pajeSetState (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string value = traceEvent->valueForField (PAJE_Value);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a state type
  if (type->nature() != PAJE_StateType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a state type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //check if the value was previously declared
  PajeValue *val = NULL;
  if (type->hasValueForIdentifier (value)){
    val = type->valueForIdentifier (value);
  }else{
    val = type->addValue (value, value, NULL);
  }

  PajeSetStateEvent event (traceEvent, container, type, val);
  container->demuxer (&event);
}

void PajeSimulator::pajePushState (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string value = traceEvent->valueForField (PAJE_Value);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a state type
  if (type->nature() != PAJE_StateType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a state type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //check if the value was previously declared
  PajeValue *val = NULL;
  if (type->hasValueForIdentifier (value)){
    val = type->valueForIdentifier (value);
  }else{
    val = type->addValue (value, value, NULL);
  }

  PajePushStateEvent event (traceEvent, container, type, val);
  container->demuxer (&event);
}

void PajeSimulator::pajePopState (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a state type
  if (type->nature() != PAJE_StateType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a state type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  PajePopStateEvent event (traceEvent, container, type);
  container->demuxer (&event);
}


void PajeSimulator::pajeResetState (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a state type
  if (type->nature() != PAJE_StateType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a state type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  PajeResetStateEvent event (traceEvent, container, type);
  container->demuxer (&event);
}

void PajeSimulator::pajeSetVariable (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string value = traceEvent->valueForField (PAJE_Value);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a variable type
  if (type->nature() != PAJE_VariableType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a variable type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  float v = strtof (value.c_str(), NULL);

  PajeSetVariableEvent event (traceEvent, container, type, v);
  container->demuxer (&event);
}

void PajeSimulator::pajeAddVariable (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string value = traceEvent->valueForField (PAJE_Value);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a variable type
  if (type->nature() != PAJE_VariableType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a variable type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  float v = strtof (value.c_str(), NULL);
  PajeAddVariableEvent event (traceEvent, container, type, v);
  container->demuxer (&event);
}

void PajeSimulator::pajeSubVariable (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string value = traceEvent->valueForField (PAJE_Value);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a variable type
  if (type->nature() != PAJE_VariableType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a variable type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  float v = strtof (value.c_str(), NULL);

  PajeSubVariableEvent event (traceEvent, container, type, v);
  container->demuxer (&event);
}

void PajeSimulator::pajeStartLink (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string startcontainerstr = traceEvent->valueForField (PAJE_StartContainer);
  std::string value = traceEvent->valueForField (PAJE_Value);
  std::string key = traceEvent->valueForField (PAJE_Key);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the start container
  PajeContainer *startcontainer = contMap[startcontainerstr];
  if (!startcontainer){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown start container '"+startcontainerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a link type
  if (type->nature() != PAJE_LinkType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a link type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //verify if the type of start container is the type expected for the start of this link
  if (type->startType() != startcontainer->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *startcontainer->type();
    std::stringstream ctype2;
    ctype2 << *type;
    throw PajeTypeException ("Type '"+ctype1.str()+"' of container '"+startcontainerstr+"' is not the container type expected for the start of link type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //check if the value was previously declared
  PajeValue *val;
  if (type->hasValueForIdentifier (value)){
    val = type->valueForIdentifier (value);
  }else{
    val = type->addValue (value, value, NULL);
  }

  PajeStartLinkEvent event (traceEvent, container, type, val, startcontainer, key);
  container->demuxer (&event);
}

void PajeSimulator::pajeEndLink (PajeTraceEvent *traceEvent)
{
  std::string time = traceEvent->valueForField (PAJE_Time);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string endcontainerstr = traceEvent->valueForField (PAJE_EndContainer);
  std::string value = traceEvent->valueForField (PAJE_Value);
  std::string key = traceEvent->valueForField (PAJE_Key);

  //search the container
  PajeContainer *container = contMap[containerstr];
  if (!container){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown container '"+containerstr+"' in "+line.str());
  }

  //search the end container
  PajeContainer *endcontainer = contMap[endcontainerstr];
  if (!endcontainer){
    std::stringstream line;
    line << *traceEvent;
    throw PajeContainerException ("Unknown end container '"+endcontainerstr+"' in "+line.str());
  }

  //search the type
  PajeType *type = typeMap[typestr];
  if (!type){
    std::stringstream line;
    line << *traceEvent;
    throw PajeTypeException ("Unknown type '"+typestr+"' in "+line.str());
  }

  //verify if the type is a link type
  if (type->nature() != PAJE_LinkType){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *type;
    throw PajeTypeException ("Type '"+desc.str()+"' is not a link type in "+line.str());
  }

  //verify if the type is child of container type
  if (type->parent() != container->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *type;
    std::stringstream ctype2;
    ctype2 << *container->type();
    throw PajeTypeException ("Type '"+ctype1.str()+"' is not child type of container type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //verify if the type of end container is the type expected for the end of this link
  if (type->endType() != endcontainer->type()){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    std::stringstream ctype1;
    ctype1 << *endcontainer->type();
    std::stringstream ctype2;
    ctype2 << *type;
    throw PajeTypeException ("Type '"+ctype1.str()+"' of container '"+endcontainerstr+"' is not the container type expected for the end of link type '"+ctype2.str()+"' in "+eventdesc.str());
  }

  //check if the value was previously declared
  PajeValue *val = NULL;
  if (type->hasValueForIdentifier (value)){
    val = type->valueForIdentifier (value);
  }else{
    val = type->addValue (value, value, NULL);
  }

  PajeEndLinkEvent event (traceEvent, container, type, val, endcontainer, key);
  container->demuxer (&event);
}

void PajeSimulator::pajeTraceFile (PajeTraceEvent *traceEvent)
{
  std::string containerstr = traceEvent->valueForField (PAJE_Container);
  std::string typestr = traceEvent->valueForField (PAJE_Type);
  std::string filename = traceEvent->valueForField (PAJE_Filename);
}
