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
#include "PajeContainer.h"
#include "PajeException.h"

extern int ignoreIncompleteLinks;

#define CALL_MEMBER_PAJE_CONTAINER(object,ptr) ((object).*(ptr))

PajeContainer::PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeType *type, PajeTraceEvent *event)
  : PajeNamedEntity (parent, type, time, name, event)
{
  stopSimulationAtTime = -1;
  init (alias, parent);
}

PajeContainer::PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeType *type, PajeTraceEvent *event, double stopat)
  : PajeNamedEntity (parent, type, time, name, event)
{
  stopSimulationAtTime = stopat;
  init (alias, parent);
}

PajeContainer::~PajeContainer ()
{
  std::map<std::string,PajeContainer*>::iterator i;
  for (i = children.begin(); i != children.end(); i++){
    delete ((*i).second);
  }
  children.clear();

  std::map<PajeType*,std::vector<PajeEntity*> >::iterator j;
  for (j = entities.begin(); j != entities.end(); j++){
    std::vector<PajeEntity*> list = ((*j).second);
    std::vector<PajeEntity*>::iterator entity;
    for (entity = list.begin(); entity != list.end(); entity++){
      delete *entity;
    }
    list.clear();
  }
  entities.clear();
}

void PajeContainer::init (std::string alias, PajeContainer *parent)
{
  _alias = alias;
  _destroyed = false;
  if (parent){
    depth = parent->depth + 1;
  }else{
    depth = 0;
  }

  invocation[PajeDefineContainerTypeEventId] = NULL;
  invocation[PajeDefineLinkTypeEventId] = NULL;
  invocation[PajeDefineEventTypeEventId] = NULL;
  invocation[PajeDefineStateTypeEventId] = NULL;
  invocation[PajeDefineVariableTypeEventId] = NULL;
  invocation[PajeDefineEntityValueEventId] = NULL;
  invocation[PajeCreateContainerEventId] = NULL;
  invocation[PajeDestroyContainerEventId] = &PajeContainer::pajeDestroyContainer;
  invocation[PajeNewEventEventId] = &PajeContainer::pajeNewEvent;
  invocation[PajeSetStateEventId] = &PajeContainer::pajeSetState;
  invocation[PajePushStateEventId] = &PajeContainer::pajePushState;
  invocation[PajePopStateEventId] = &PajeContainer::pajePopState;
  invocation[PajeResetStateEventId] = &PajeContainer::pajeResetState;
  invocation[PajeSetVariableEventId] = &PajeContainer::pajeSetVariable;
  invocation[PajeAddVariableEventId] = &PajeContainer::pajeAddVariable;
  invocation[PajeSubVariableEventId] = &PajeContainer::pajeSubVariable;
  invocation[PajeStartLinkEventId] = &PajeContainer::pajeStartLink;
  invocation[PajeEndLinkEventId] = &PajeContainer::pajeEndLink;
}

int PajeContainer::numberOfEntities (void)
{
  int ret = 0;

  std::map<std::string,PajeContainer*>::iterator i;
  for (i = children.begin(); i != children.end(); i++){
    ret += ((*i).second)->numberOfEntities();
  }

  std::map<PajeType*,std::vector<PajeEntity*> >::iterator j;
  for (j = entities.begin(); j != entities.end(); j++){
    ret += ((*j).second).size();
  }
  return ret;
}

std::vector<PajeContainer*> PajeContainer::getChildren (void)
{
  std::vector<PajeContainer*> ret;
  std::map<std::string,PajeContainer*>::iterator i;
  for (i = children.begin(); i != children.end(); i++){
    ret.push_back ((*i).second);
  }
  return ret;
}

bool PajeContainer::isAncestorOf (PajeContainer *c)
{
  c = c->container();
  while (c){
    if (c == this) return true;
    c = c->container();
  }
  return false;
}

bool PajeContainer::keepSimulating (void)
{
  if (stopSimulationAtTime != -1){
    if (endTime() < stopSimulationAtTime){
      return true;
    }
  }

  std::map<std::string,PajeContainer*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    PajeContainer *child = ((*it).second);
    if (!child->keepSimulating()) return false;
  }
  return true;
}

std::string PajeContainer::description (void) const
{
  std::stringstream description;
  std::string containername;
  if (container()){
    containername = container()->name();
  }else{
    containername = std::string("0");
  }
  description << (type()? type()->kind() : "NULL") << ", "
              << containername << ", "
              << (type()? type()->name() : "NULL") << ", "
              << startTime() << ", "
              << endTime() << ", "
              << endTime() - startTime() << ", "
              << name();
  return description.str();
}

bool PajeContainer::isContainer (void) const
{
  return true;
}

const std::string &PajeContainer::identifier ()
{
  return _alias.empty() ? name() : _alias;
}

bool PajeContainer::checkPendingLinks (void)
{
  std::vector<PajeUserLink*> invalidLinks;

  std::map<PajeType*,std::map<std::string,PajeUserLink*> >::iterator it;
  for (it = pendingLinks.begin(); it != pendingLinks.end(); it++){
    std::map<std::string,PajeUserLink*> x = ((*it).second);
    if (x.size() == 0) continue;

    std::map<std::string,PajeUserLink*>::iterator it2;
    for (it2 = x.begin(); it2 != x.end(); it2++){
      invalidLinks.push_back ((*it2).second);
    }
    if (!x.empty()){
      //report
      std::cout << "List of incomplete links in container '" << name() << "':" << std::endl;
      std::vector<PajeUserLink*>::iterator it;
      for (it = invalidLinks.begin(); it != invalidLinks.end(); it++){
        std::cout << (*it)->description() << std::endl;
      }
      return false;
    }
  }
  return true;
}

void PajeContainer::demuxer (PajeEvent *event)
{
  PajeEventId eventId = event->traceEvent()->pajeEventId();
  if (eventId == PajeDestroyContainerEventId){
    //check time ordering for container
    if (event->time() < endTime()){
      PajeTraceEvent *traceEvent = event->traceEvent();
      std::stringstream eventdesc;
      eventdesc << *traceEvent;
      throw PajeSimulationException ("Illegal, container events are not time-ordered in "+eventdesc.str());
      return;
    }
  }
  
  //check if I'm stopped
  if (_destroyed){
    return;
  }

  double lastKnownTime = event->time();
  //stop the simulation before the end
  if (stopSimulationAtTime != -1){
    if (lastKnownTime > stopSimulationAtTime){
      pajeDestroyContainer (stopSimulationAtTime, event);
      return;
    }
  }

  //change the simulated behavior according to the event
  if (eventId < PajeEventIdCount){
    if (invocation[eventId]){
      CALL_MEMBER_PAJE_CONTAINER(*this,invocation[eventId])(event);
    }else{
      throw PajeSimulationException ("Asked to simulate something I don't know how to simulate");
    }
  }else{
    throw PajeSimulationException ("Unknow event id.");
  }

  //update container endtime
  setEndTime (event->time());
}

void PajeContainer::pajeNewEvent (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  PajeValue *value = event->value();
  PajeTraceEvent *traceEvent = event->traceEvent();

  checkTimeOrder (event);
  PajeUserEvent *n = new PajeUserEvent(this, type, time, value, traceEvent);
  entities[type].push_back (n);
}

void PajeContainer::pajeSetState (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  PajeValue *value = event->value();
  PajeTraceEvent *traceEvent = event->traceEvent();

  checkTimeOrder (event);
  pajeResetState (event);

  PajeUserState *state = new PajeUserState (this, type, time, value, traceEvent);
  entities[type].push_back (state);

  std::vector<PajeUserState*> *stack = &stackStates[type];
  stack->push_back (state);
}

void PajeContainer::pajePushState (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  PajeValue *value = event->value();
  PajeTraceEvent *traceEvent = event->traceEvent();

  checkTimeOrder (event);

  std::vector<PajeUserState*> *stack = &stackStates[type];

  //define new imbrication level
  int imbrication = stack->empty() ? 0 : (stack->back())->imbricationLevel() + 1;

  PajeUserState *state = new PajeUserState (this, type, time, value, imbrication, traceEvent);
  entities[type].push_back (state);
  stack->push_back (state);
}

void PajeContainer::pajePopState (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  PajeTraceEvent *traceEvent = event->traceEvent();

  checkTimeOrder (event);

  //check if there is something in the stack
  std::vector<PajeUserState*> *stack = &stackStates[type];
  if (stack->empty()){
    std::stringstream line;
    line << *traceEvent;
    throw PajeStateException ("Illegal pop event of a state that has no value in "+line.str());
  }

  //update the top of the stack, set its endTime
  PajeUserState *last_stacked = stack->back();
  last_stacked->setEndTime (time);

  //pop the stack
  stack->pop_back();
}

void PajeContainer::pajeResetState (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  PajeTraceEvent *traceEvent = event->traceEvent();

  checkTimeOrder (event);

  //clean the stack, using time as endTime
  std::vector<PajeUserState*> *stack = &stackStates[type];
  std::vector<PajeUserState*>::iterator it;
  for (it = stack->begin(); it != stack->end(); it++){
    PajeUserState *state = (*it);
    state->setEndTime (time);
  }
  stack->clear();
}

void PajeContainer::pajeSetVariable (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  double value = event->doubleValue();
  PajeTraceEvent *traceEvent = event->traceEvent();


  PajeEntity *last = NULL;
  if (entities[type].size() != 0){
    last = entities[type].back();
  }
  if (last){
    checkTimeOrder (event);
    if (last->startTime() == time){
      //only update last value
      last->setDoubleValue (value);
      return;
    }else{
      last->setEndTime (time);
    }
  }

  //create new
  PajeUserVariable *val = new PajeUserVariable (this, type, time, value, traceEvent);
  entities[type].push_back(val);
}

void PajeContainer::pajeAddVariable (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  double value = event->doubleValue();
  PajeTraceEvent *traceEvent = event->traceEvent();

  if (entities[type].size() == 0){
    std::stringstream line;
    line << *traceEvent;
    throw PajeVariableException ("Illegal addition to a variable that has no value (yet) in "+line.str());
  }

  checkTimeOrder (event);

  double lastValue = 0;
  if (entities.count(type)){
    if (!entities[type].empty()){
      PajeEntity *last = entities[type].back();
      if (last->startTime() == time){
        //only update last value
        last->addDoubleValue (value);
        return;
      }else{
        last->setEndTime (time);
      }
      lastValue = last->doubleValue();
    }
  }
  //create new
  PajeUserVariable *val = new PajeUserVariable (this, type, time, lastValue + value, traceEvent);
  entities[type].push_back(val);
}

void PajeContainer::pajeSubVariable (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  double value = event->doubleValue();
  PajeTraceEvent *traceEvent = event->traceEvent();

  if (entities[type].size() == 0){
    std::stringstream line;
    line << *traceEvent;
    throw PajeVariableException ("Illegal subtraction from a variable that has no value (yet) in "+line.str());
  }

  checkTimeOrder (event);

  double lastValue = 0;
  if (entities.count(type)){
    if (!entities[type].empty()){
      PajeEntity *last = entities[type].back();
      if (last->startTime() == time){
        //only update last value
        last->subtractDoubleValue (value);
        return;
      }else{
        last->setEndTime (time);
      }
      lastValue = last->doubleValue();
    }
  }

  //create new
  PajeUserVariable *val = new PajeUserVariable (this, type, time, lastValue - value, traceEvent);
  entities[type].push_back(val);
}

void PajeContainer::pajeStartLink (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  std::string key = event->key();
  PajeValue *value = event->value();
  PajeTraceEvent *traceEvent = event->traceEvent();
  PajeContainer *startContainer = event->startContainer();

  if (linksUsedKeys[type].count(key)){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    throw PajeLinkException ("Illegal event in "+eventdesc.str()+", the key was already used for another link");
  }

  if (pendingLinks[type].count(key) == 0){
    PajeUserLink *link = new PajeUserLink(this, type, time, value, key, startContainer, traceEvent);
    pendingLinks[type].insert (std::make_pair(key, link));

  }else{
    //there is a PajeEndLink
    PajeUserLink *link = (pendingLinks[type].find(key))->second;
    link->setStartTime (time);
    link->setStartContainer (startContainer);
    link->addPajeTraceEvent (traceEvent);

    //check validity of the PajeEndLink when compared to its PajeStartLink
    if (link->value() != value){
      std::stringstream eventdesc;
      eventdesc << *traceEvent;
      throw PajeLinkException ("Illegal PajeStartLink in "+eventdesc.str()+", value is different from the value of the corresponding PajeEndLink (which had "+link->value()->identifier()+")");
    }

    //checking time-ordered for this type
    checkTimeOrder (link->startTime(), type, traceEvent);

    //push the newly completed link on the back of the vector
    entities[type].push_back(link);

    //remove the link for the temporary pool, add the key to usedKeys
    pendingLinks[type].erase(key);
    linksUsedKeys[type].insert(key);
  }
}

void PajeContainer::pajeEndLink (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  std::string key = event->key();
  PajeValue *value = event->value();
  PajeTraceEvent *traceEvent = event->traceEvent();
  PajeContainer *endContainer = event->endContainer();

  if (linksUsedKeys[type].count(key)){
    std::stringstream eventdesc;
    eventdesc << *traceEvent;
    throw PajeLinkException ("Illegal event in "+eventdesc.str()+", the key was already used for another link");
  }

  if (pendingLinks[type].count(key) == 0){
    //there is no corresponding PajeStartLink
    PajeUserLink *link = new PajeUserLink(this, type, -1, value, key, NULL, traceEvent);
    link->setEndContainer (endContainer);
    link->setEndTime (time);
    pendingLinks[type].insert (std::make_pair(key, link));

  }else{
    //there is a PajeStartLink
    PajeUserLink *link = (pendingLinks[type].find(key))->second;
    link->setEndContainer (endContainer);
    link->setEndTime (time);
    link->addPajeTraceEvent (traceEvent);

    //check validity of the PajeEndLink when compared to its PajeStartLink
    if (link->value() != value){
      std::stringstream eventdesc;
      eventdesc << *traceEvent;
      throw PajeLinkException ("Illegal PajeEndLink in "+eventdesc.str()+", value is different from the value of the corresponding PajeStartLink (which had "+link->value()->identifier()+")");
    }

    //checking time-ordered for this type
    checkTimeOrder (link->endTime(), type, traceEvent);

    //push the newly completed link on the back of the vector
    entities[type].push_back(link);

    //remove the link for the temporary pool, add the key to usedKeys
    pendingLinks[type].erase(key);
    linksUsedKeys[type].insert(key);
  }
}

std::ostream &operator<< (std::ostream &output, const PajeContainer &container)
{
  output << "(Container, name: "
         << container.name()
         << ", alias: " << container._alias << ")";
  return output;
}

void PajeContainer::recursiveDestroy (double time)
{
  if (!_destroyed){
    destroy (time);
  }
  std::map<std::string,PajeContainer*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    ((*it).second)->recursiveDestroy (time);
  }
}

std::vector<PajeEntity*> PajeContainer::enumeratorOfEntitiesTyped (double start, double end, PajeType *type)
{
  std::vector<PajeEntity*> empty;
  if (entities[type].size() == 0) return empty;

  std::vector<PajeEntity*>::iterator low, up, it;
  low = lower_bound (entities[type].begin(), entities[type].end(), start, PajeEntity::PajeEntityCompare());
  up = lower_bound (entities[type].begin(), entities[type].end(), end, PajeEntity::PajeEntityCompare());

  if (low != entities[type].begin()){
    low--;
  }

  if (up != entities[type].end()){
    up++;
  }

  for (it = low; it != up; it++){
    empty.push_back (*it);
  }
  return empty;
}

PajeAggregatedDict PajeContainer::timeIntegrationOfTypeInContainer (double start, double end, PajeType *type)
{
  PajeAggregatedDict ret;
  if (entities[type].size() == 0) return ret;
  if (type->nature() == PAJE_LinkType) return ret;

  if (type->nature() == PAJE_StateType){
    ret = timeIntegrationOfStateTypeInContainer (start, end, type);
  }else if (type->nature() == PAJE_VariableType){
    ret = timeIntegrationOfVariableTypeInContainer (start, end, type);
  }
  return ret;
}

PajeAggregatedDict PajeContainer::timeIntegrationOfStateTypeInContainer (double start, double end, PajeType *type)
{
  PajeAggregatedDict ret;
  std::vector<PajeEntity*> slice = enumeratorOfEntitiesTyped (start, end, type);
  double tsDuration = end - start;
  std::vector<PajeEntity*>::iterator it;
  for (it = slice.begin(); it != slice.end(); it++){
    PajeEntity *var = *it;
    double s = var->startTime();
    double e = var->endTime();
    if (!var->doubleValue()) continue;
    if (s < start) s = start;
    if (e > end) e = end;
    double duration = e - s;
    double var_integrated = duration/tsDuration;

    PajeAggregatedType *agtype = new PajeAggregatedType (type, var->value());
    PajeAggregatedDict::iterator found = ret.find (agtype);
    if (found != ret.end()){
      ret[agtype] += var_integrated;
    }else{
      ret[agtype] = var_integrated;
    }
  }
  return ret;
}

PajeAggregatedDict PajeContainer::timeIntegrationOfVariableTypeInContainer (double start, double end, PajeType *type)
{
  PajeAggregatedDict ret;

  std::vector<PajeEntity*> slice = enumeratorOfEntitiesTyped (start, end, type);
  double integrated = 0;
  double tsDuration = end - start;
  std::vector<PajeEntity*>::iterator it;
  for (it = slice.begin(); it != slice.end(); it++){
    PajeEntity *var = *it;
    double s = var->startTime();
    double e = var->endTime();
    if (!var->doubleValue()) continue;
    if (s < start) s = start;
    if (e > end) e = end;
    double duration = e - s;
    double var_integrated = duration/tsDuration * var->doubleValue();
    integrated += var_integrated;
  }

  if (integrated){
    ret[new PajeAggregatedType(type)] = integrated;
  }

  return ret;
}

PajeAggregatedDict PajeContainer::merge (PajeAggregatedDict a,
                                                           PajeAggregatedDict b)
{
  PajeAggregatedDict ret = a;
  PajeAggregatedDict::iterator it;
  for (it = b.begin(); it != b.end(); it++){
    if (ret.count((*it).first)){
      std::cout << "error on " << __FILE__ << ":" << __LINE__ << std::endl;
    }
    ret[(*it).first] = (*it).second;
  }
  return ret;
}

PajeAggregatedDict PajeContainer::add (PajeAggregatedDict a,
                                                         PajeAggregatedDict b)
{
  PajeAggregatedDict ret = a;
  PajeAggregatedDict::iterator it;
  for (it = b.begin(); it != b.end(); it++){
    ret[(*it).first] += (*it).second;
  }
  return ret;
}

PajeAggregatedDict PajeContainer::integrationOfContainer (double start, double end)
{
  PajeAggregatedDict ret;
  if (start == -1 || end == -1) return ret;
  std::map<std::string,PajeType*>::iterator it;
  PajeAggregatedDict partial;
  std::map<std::string,PajeType*> c = type()->children();
  for (it = c.begin(); it != c.end(); it++){
    PajeType *ctype = (*it).second;
    partial = timeIntegrationOfTypeInContainer (start, end, (*it).second);
    ret = merge (ret, partial);
  }
  return ret;
}

PajeAggregatedDict PajeContainer::spatialIntegrationOfContainer (double start, double end)
{
  if (start == selectionStart && end == selectionEnd){
    return spatialAggregated;
  }
  std::map<std::string,PajeContainer*>::iterator it;
  PajeAggregatedDict ret = integrationOfContainer (start, end);
  for (it = children.begin(); it != children.end() ; it++){
    PajeAggregatedDict partial = ((*it).second)->spatialIntegrationOfContainer (start, end);
    ret = add (ret, partial);
  }
  selectionStart = start;
  selectionEnd = end;
  spatialAggregated = ret;
  return ret;
}

bool PajeContainer::checkTimeOrder (PajeEvent *event)
{
  double time = event->time();
  PajeType *type = event->type();
  PajeTraceEvent *traceEvent = event->traceEvent();
  return checkTimeOrder (time, type, traceEvent);
}

bool PajeContainer::checkTimeOrder (double time, PajeType *type, PajeTraceEvent *traceEvent)
{
  std::vector<PajeEntity*> *v = &entities[type];
  if (!v->empty()){
    PajeEntity *last = entities[type].back();
    if ( (last && last->startTime() > time) ||
         (last && last->endTime() != -1 && last->endTime() > time)){
      std::stringstream eventdesc;
      eventdesc << *traceEvent;
      throw PajeSimulationException ("Illegal, trace is not time-ordered in "+eventdesc.str());
      return false;
    }
  }
  return true;
}

PajeContainer *PajeContainer::pajeCreateContainer (double time, PajeType *type, PajeTraceEvent *event, double stopat)
{
  std::string name = event->valueForField (PAJE_Name);
  std::string alias = event->valueForField (PAJE_Alias);

  if (type->nature() != PAJE_ContainerType){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw PajeSimulationException ("Trying to create a container of a type that is not a container type in "+eventdesc.str());
  }

  PajeContainer *newContainer = new PajeContainer (time, name, alias, this, type, event, stopat);
  children[newContainer->identifier()] = newContainer;
  return newContainer;
}

void PajeContainer::pajeDestroyContainer (double time, PajeEvent *event)
{
  PajeTraceEvent *traceEvent = event->traceEvent();
  if (_destroyed){
    std::stringstream line;
    line << *traceEvent;
    std::stringstream desc;
    desc << *this;
    throw PajeContainerException ("Container '"+desc.str()+"' already destroyed in "+line.str());
  }
  destroy (time);
}


void PajeContainer::pajeDestroyContainer (PajeEvent *event)
{
  pajeDestroyContainer (event->time(), event);
}

void PajeContainer::destroy (double time)
{

  //mark as destroyed, update endtime
  _destroyed = true;
  setEndTime (time);

  //finish all entities
  std::map<PajeType*,std::vector<PajeEntity*> >::iterator it1;
  for (it1 = entities.begin(); it1 != entities.end(); it1++){
    if (!(*it1).second.empty()){
      PajeEntity *last = ((*it1).second).back();
      if (last->endTime() == -1){
        last->setEndTime (time);
      }
    }
  }

  //check pendingLinks
  if (!ignoreIncompleteLinks){
    if (!checkPendingLinks()){
      throw PajeLinkException ("Incomplete links at the end of container with name '"+name()+"'");
    }
  }

  //check stackStates, finish all stacked states, clear stacks
  std::map<PajeType*,std::vector<PajeUserState*> >::iterator it2;
  for (it2 = stackStates.begin(); it2 != stackStates.end(); it2++){
    std::vector<PajeUserState*> *stack = &((*it2).second);
    std::vector<PajeUserState*>::iterator it3;
    for (it3 = stack->begin(); it3 != stack->end(); it3++){
      (*it3)->setEndTime (time);
    }
    stack->clear();
  }

  //recursive destroy because parent container is being destroyed
  recursiveDestroy(time);
}
