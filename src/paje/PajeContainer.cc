#include "PajeContainer.h"

PajeContainer::PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeContainerType *type):PajeUserState (parent, type, name, time, time)
{
  this->alias = alias;
  this->destroyed = false;
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

  //finish all variables
  std::map<PajeType*,std::vector<PajeEntity*> >::iterator it1;
  for (it1 = entities.begin(); it1 != entities.end(); it1++){
    PajeEntity *last = ((*it1).second).back();
    last->setEndTime (time);
  }

  //finish all states, events, ... //TODO FIXME
}

void PajeContainer::setVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  PajeEntity *last = NULL;
  if (entities[type].size() != 0){
    last = entities[type].back();
  }
  if (last){
    if (last->startTime() > time){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
    }
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

  PajeEntity *last = entities[type].back();
  if (last->startTime() > time){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
  }
  if (last->startTime() == time){
    //only update last value
    last->addDoubleValue (value);
    return;
  }else{
    last->setEndTime (time);
  }

  //create new
  PajeUserVariable *val = new PajeUserVariable (this, type, last->doubleValue() + value, time, time);
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

  PajeEntity *last = entities[type].back();
  if (last->startTime() > time){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
  }
  if (last->startTime() == time){
    last->subtractDoubleValue (value);
    return;
  }else{
    last->setEndTime (time);
  }

  //create new
  PajeUserVariable *val = new PajeUserVariable (this, type, last->doubleValue() - value, time, time);
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
    PajeEntity *last_link = NULL;
    if (entities[type].size() != 0){
      last_link = entities[type].back();
    }
    if (last_link){
      if (last_link->startTime() > link->startTime()){
        std::stringstream eventdesc;
        eventdesc << *event;
        throw "Illegal, trace is not time-ordered in "+eventdesc.str();
      }
    }

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
    PajeEntity *last_link = NULL;
    if (entities[type].size() != 0){
      last_link = entities[type].back();
    }
    if (last_link){
      if (last_link->startTime() > link->startTime()){
        std::stringstream eventdesc;
        eventdesc << *event;
        throw "Illegal, trace is not time-ordered in "+eventdesc.str();
      }
    }

    //push the newly completed link on the back of the vector
    entities[type].push_back(link);

    //remove the link for the temporary pool, add the key to usedKeys
    pendingLinks.erase(key);
    linksUsedKeys.insert(key);
  }
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

std::map<std::string,double> PajeContainer::timeIntegrationOfTypeInContainer (double start, double end, PajeType *type)
{
  std::map<std::string,double> empty;
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
  empty[type->name] = integrated;
  return empty;
}

std::map<std::string,double> PajeContainer::merge (std::map<std::string,double> a,
                                                   std::map<std::string,double> b)
{
  std::map<std::string,double> ret = a;
  std::map<std::string,double>::iterator it;
  for (it = b.begin(); it != b.end(); it++){
    if (ret[(*it).first]){
      std::cout << "error on " << __FILE__ << ":" << __LINE__ << std::endl;
    }
    ret[(*it).first] = (*it).second;
  }
  return ret;
}

std::map<std::string,double> PajeContainer::add (std::map<std::string,double> a,
                                                 std::map<std::string,double> b)
{
  std::map<std::string,double> ret = a;
  std::map<std::string,double>::iterator it;
  for (it = b.begin(); it != b.end(); it++){
    std::string var = (*it).first;
    ret[var] += (*it).second;
  }
  return ret;
}

std::map<std::string,double> PajeContainer::integrationOfContainer (double start, double end)
{
  std::map<std::string,PajeType*>::iterator it;
  std::map<std::string,double> ret, partial;
  PajeContainerType *contType = dynamic_cast<PajeContainerType*>(type());
  for (it = contType->children.begin(); it != contType->children.end(); it++){
    partial = timeIntegrationOfTypeInContainer (start, end, (*it).second);
    ret = merge (ret, partial);
  }
  return ret;
}

std::map<std::string,double> PajeContainer::spatialIntegrationOfContainer (double start, double end)
{
  std::map<std::string,PajeContainer*>::iterator it;
  std::map<std::string,double> ret = integrationOfContainer (start, end);
  for (it = children.begin(); it != children.end() ; it++){
    std::map<std::string,double> partial = ((*it).second)->spatialIntegrationOfContainer (start, end);
    ret = add (ret, partial);
  }
  return ret;
}
