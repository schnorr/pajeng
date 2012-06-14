/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeContainer.h"

PajeContainer::PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeContainerType *type):PajeUserState (parent, type, name, time, time)
{
  this->alias = alias;
  this->destroyed = false;
  if (parent){
    this->depth = parent->depth + 1;
  }else{
    this->depth = 0;
  }
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

std::string PajeContainer::description (void)
{
  std::stringstream description;
  std::string containername;
  if (container()){
    containername = container()->name();
  }else{
    containername = std::string("0");
  }
  description << "Container, "
              << containername << ", "
              << type()->name << ", "
              << "[" << startTime() << " " << endTime() << "], "
              << name();
  return description.str();
}

bool PajeContainer::isContainer (void)
{
  return true;
}

std::string PajeContainer::identifier ()
{
  return alias.empty() ? name() : alias;
}

PajeContainer *PajeContainer::addContainer (double time, std::string name, std::string alias, PajeContainerType *type, PajeEvent *event)
{
  PajeContainer *newContainer = new PajeContainer (time, name, alias, this, type);
  children[newContainer->identifier()] = newContainer;
  return newContainer;
}

void PajeContainer::destroy (double time, PajeEvent *event)
{
  if (destroyed){
    std::stringstream line;
    line << *event;
    std::stringstream desc;
    desc << *this;
    throw "Container '"+desc.str()+"' already destroyed in "+line.str();
  }

  //mark as destroyed, update endtime
  destroyed = true;
  setEndTime (time);

  //finish all entities
  std::map<PajeType*,std::vector<PajeEntity*> >::iterator it1;
  for (it1 = entities.begin(); it1 != entities.end(); it1++){
    if (((*it1).second).size()){
      PajeEntity *last = ((*it1).second).back();
      if (last->endTime() == -1){
        last->setEndTime (time);
      }
    }
  }

  //check pendingLinks
  if (pendingLinks.size()){
    throw "Incomplete links at the end of container "+name();
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
}

void PajeContainer::setVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  PajeEntity *last = NULL;
  if (entities[type].size() != 0){
    last = entities[type].back();
  }
  if (last){
    checkTimeOrder (time, type, event);
    if (last->startTime() == time){
      //only update last value
      last->setDoubleValue (value);
      return;
    }else{
      last->setEndTime (time);
    }
  }

  //create new
  PajeUserVariable *val = new PajeUserVariable (this, type, value, time, time);
  entities[type].push_back(val);

  //update container endtime
  setEndTime (time);
}

void PajeContainer::addVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  if (entities[type].size() == 0){
    std::stringstream line;
    line << *event;
    throw "Illegal addition to a variable that has no value (yet) in "+line.str();
  }

  checkTimeOrder (time, type, event);

  double lastValue = 0;
  if (entities.count(type)){
    if (entities[type].size()){
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
  PajeUserVariable *val = new PajeUserVariable (this, type, lastValue + value, time, time);
  entities[type].push_back(val);

  //update container endtime
  setEndTime (time);
}

void PajeContainer::subVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  if (entities[type].size() == 0){
    std::stringstream line;
    line << *event;
    throw "Illegal subtraction from a variable that has no value (yet) in "+line.str();
  }

  checkTimeOrder (time, type, event);

  double lastValue = 0;
  if (entities.count(type)){
    if (entities[type].size()){
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
  PajeUserVariable *val = new PajeUserVariable (this, type, lastValue - value, time, time);
  entities[type].push_back(val);

  //update container endtime
  setEndTime (time);
}

void PajeContainer::startLink (double time, PajeType *type, PajeContainer *startContainer, std::string value, std::string key, PajeEvent *event)
{
  if (linksUsedKeys.count(key)){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "Illegal event in "+eventdesc.str()+", the key was already used for another link";
  }

  if (pendingLinks.count(key) == 0){
    PajeUserLink *link = new PajeUserLink(this, type, value, key, startContainer, time);
    pendingLinks.insert (std::make_pair(key, link));

  }else{
    //there is a PajeEndLink
    PajeUserLink *link = (pendingLinks.find(key))->second;
    link->setStartTime (time);
    link->setStartContainer (startContainer);

    //check validity of the PajeEndLink when compared to its PajeStartLink
    if (link->value() != value){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal PajeStartLink in "+eventdesc.str()+", value is different from the value of the corresponding PajeEndLink (which had "+link->value()+")";
    }

    //checking time-ordered for this type
    checkTimeOrder (link->startTime(), type, event);

    //push the newly completed link on the back of the vector
    entities[type].push_back(link);

    //remove the link for the temporary pool, add the key to usedKeys
    pendingLinks.erase(key);
    linksUsedKeys.insert(key);
  }
}

void PajeContainer::endLink (double time, PajeType *type, PajeContainer *endContainer, std::string value, std::string key, PajeEvent *event)
{
  if (linksUsedKeys.count(key)){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "Illegal event in "+eventdesc.str()+", the key was already used for another link";
  }

  if (pendingLinks.count(key) == 0){
    //there is no corresponding PajeStartLink
    PajeUserLink *link = new PajeUserLink(this, type, value, key, NULL, -1);
    link->setEndContainer (endContainer);
    link->setEndTime (time);
    pendingLinks.insert (std::make_pair(key, link));

  }else{
    //there is a PajeStartLink
    PajeUserLink *link = (pendingLinks.find(key))->second;
    link->setEndContainer (endContainer);
    link->setEndTime (time);

    //check validity of the PajeEndLink when compared to its PajeStartLink
    if (link->value() != value){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal PajeEndLink in "+eventdesc.str()+", value is different from the value of the corresponding PajeStartLink (which had "+link->value()+")";
    }

    //checking time-ordered for this type
    checkTimeOrder (link->endTime(), type, event);

    //push the newly completed link on the back of the vector
    entities[type].push_back(link);

    //remove the link for the temporary pool, add the key to usedKeys
    pendingLinks.erase(key);
    linksUsedKeys.insert(key);
  }
}

void PajeContainer::newEvent (double time, PajeType *type, std::string value, PajeEvent *event)
{
  checkTimeOrder (time, type, event);
  PajeUserEvent *n = new PajeUserEvent(this, type, value, time);
  entities[type].push_back (n);
}

void PajeContainer::setState (double time, PajeType *type, std::string value, PajeEvent *event)
{
  checkTimeOrder (time, type, event);

  resetState (time, type, value, event);

  PajeUserState *state = new PajeUserState (this, type, value, time);
  entities[type].push_back (state);

  std::vector<PajeUserState*> *stack = &stackStates[type];
  stack->push_back (state);
}

void PajeContainer::pushState (double time, PajeType *type, std::string value, PajeEvent *event)
{
  checkTimeOrder (time, type, event);

  std::vector<PajeUserState*> *stack = &stackStates[type];

  //define new imbrication level
  int imbrication = !stack->size() ? 0 : (stack->back())->imbricationLevel() + 1;

  PajeUserState *state = new PajeUserState (this, type, value, time, imbrication);
  entities[type].push_back (state);
  stack->push_back (state);
}

void PajeContainer::popState (double time, PajeType *type, std::string value, PajeEvent *event)
{
  checkTimeOrder (time, type, event);

  //check if there is something in the stack
  std::vector<PajeUserState*> *stack = &stackStates[type];
  if (!stack->size()){
    std::stringstream line;
    line << *event;
    throw "Illegal pop event of a state that has no value in "+line.str();
  }

  //update the top of the stack, set its endTime
  PajeUserState *last_stacked = stack->back();
  last_stacked->setEndTime (time);

  //pop the stack
  stack->pop_back();
}

void PajeContainer::resetState (double time, PajeType *type, std::string value, PajeEvent *event)
{
  checkTimeOrder (time, type, event);

  //clean the stack, using time as endTime
  std::vector<PajeUserState*> *stack = &stackStates[type];
  std::vector<PajeUserState*>::iterator it;
  for (it = stack->begin(); it != stack->end(); it++){
    PajeUserState *state = (*it);
    state->setEndTime (time);
  }
  stack->clear();
}

std::ostream &operator<< (std::ostream &output, const PajeContainer &container)
{
  output << "(Container, name: "
         << container.name()
         << ", alias: " << container.alias << ")";
  return output;
}

void PajeContainer::recursiveDestroy (double time, PajeEvent *event)
{
  if (!destroyed){
    this->destroy (time, event);
  }
  std::map<std::string,PajeContainer*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    ((*it).second)->recursiveDestroy (time, event);
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

  for (it = low; it != up; it++){
    empty.push_back (*it);
  }
  return empty;
}

std::map<PajeType*,double> PajeContainer::timeIntegrationOfTypeInContainer (double start, double end, PajeType *type)
{
  std::map<PajeType*,double> empty;
  if (entities[type].size() == 0) return empty;

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
    empty[type] = integrated;
  }
  return empty;
}

std::map<PajeType*,double> PajeContainer::merge (std::map<PajeType*,double> a,
                                                   std::map<PajeType*,double> b)
{
  std::map<PajeType*,double> ret = a;
  std::map<PajeType*,double>::iterator it;
  for (it = b.begin(); it != b.end(); it++){
    if (ret[(*it).first]){
      std::cout << "error on " << __FILE__ << ":" << __LINE__ << std::endl;
    }
    ret[(*it).first] = (*it).second;
  }
  return ret;
}

std::map<PajeType*,double> PajeContainer::add (std::map<PajeType*,double> a,
                                                 std::map<PajeType*,double> b)
{
  std::map<PajeType*,double> ret = a;
  std::map<PajeType*,double>::iterator it;
  for (it = b.begin(); it != b.end(); it++){
    PajeType* var = (*it).first;
    ret[var] += (*it).second;
  }
  return ret;
}

std::map<PajeType*,double> PajeContainer::integrationOfContainer (double start, double end)
{
  std::map<std::string,PajeType*>::iterator it;
  std::map<PajeType*,double> ret, partial;
  PajeContainerType *contType = dynamic_cast<PajeContainerType*>(this->type());
  for (it = contType->children.begin(); it != contType->children.end(); it++){
    partial = timeIntegrationOfTypeInContainer (start, end, (*it).second);
    ret = merge (ret, partial);
  }
  return ret;
}

std::map<PajeType*,double> PajeContainer::spatialIntegrationOfContainer (double start, double end)
{
  std::map<std::string,PajeContainer*>::iterator it;
  std::map<PajeType*,double> ret = integrationOfContainer (start, end);
  for (it = children.begin(); it != children.end() ; it++){
    std::map<PajeType*,double> partial = ((*it).second)->spatialIntegrationOfContainer (start, end);
    ret = add (ret, partial);
  }
  return ret;
}

bool PajeContainer::checkTimeOrder (double time, PajeType *type, PajeEvent *event)
{
  std::vector<PajeEntity*> *v = &entities[type];
  if (v->size()){
    PajeEntity *last = entities[type].back();
    if ( (last && last->startTime() > time) ||
         (last && last->endTime() != -1 && last->endTime() > time)){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
    }
  }
}
