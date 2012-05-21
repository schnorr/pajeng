#include "PajeContainer.h"

PajeContainer::PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeContainerType *type):PajeEntity (parent, type, name)
{
  this->alias = alias;
  this->destroyed = false;
  this->stime = time;
  this->etime = time;
}

std::string PajeContainer::identifier ()
{
  return alias.empty() ? name : alias;
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
  this->etime = time;

  //finish all variables
  std::map<PajeType*,std::vector<PajeUserVariable> >::iterator it1;
  for (it1 = variables.begin(); it1 != variables.end(); it1++){
    PajeUserVariable *last = &((*it1).second).back();
    last->setEndTime (time);
  }

  //finish all states, events, ... //TODO FIXME
}

void PajeContainer::setVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  PajeUserVariable *last = NULL;
  if (variables[type].size() != 0){
    last = &variables[type].back();
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
  variables[type].push_back(*val);

  //update container endtime
  this->etime = time;
}

void PajeContainer::addVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  if (variables[type].size() == 0){
    std::stringstream line;
    line << *event;
    throw "Illegal addition to a variable that has no value (yet) in "+line.str();
  }

  PajeUserVariable *last = &variables[type].back();
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
  variables[type].push_back(*val);

  //update container endtime
  this->etime = time;
}

void PajeContainer::subVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  if (variables[type].size() == 0){
    std::stringstream line;
    line << *event;
    throw "Illegal subtraction from a variable that has no value (yet) in "+line.str();
  }

  PajeUserVariable *last = &variables[type].back();
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
  variables[type].push_back(*val);

  //update container endtime
  this->etime = time;
}

void PajeContainer::startLink (double time, PajeType *type, PajeContainer *startContainer, std::string value, std::string key, PajeEvent *event)
{
  if (!pendingLinks.count(key)){
    if (linksUsedKeys.count(key)){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal PajeEndLink in "+eventdesc.str()+", the key was already used for another link";
    }

    link_t link;
    link.stime = time;
    link.etime = -1;
    link.startContainer = startContainer;
    link.endContainer = NULL;
    link.value = value;
    pendingLinks[key] = link;
    return;
  }

  //key was already used
  link_t link = pendingLinks[key];
  if (link.startContainer){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "Illegal PajeStartLink in "+eventdesc.str()+", the key was already used for another PajeStartLink";
  }

  //endlink was already registered for this key
  link.stime = time;
  link.startContainer = startContainer;
  if (link.value != value){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "Illegal PajeStartLink in "+eventdesc.str()+", value is different from the value of the corresponding PajeEndLink (which had "+link.value+")";
  }

  //checking time-ordered for this type
  link_t *last_link = NULL;
  if (links[type].size() != 0){
    last_link = &links[type].back();
  }
  if (last_link){
    if (last_link->stime > link.stime){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
    }
  }

  //push the newly completed link on the back of the vector
  links[type].push_back(link);

  //remove the link for the temporary pool, add the key to usedKeys
  pendingLinks.erase(key);
  linksUsedKeys.insert(key);
}

void PajeContainer::endLink (double time, PajeType *type, PajeContainer *endContainer, std::string value, std::string key, PajeEvent *event)
{
  if (!pendingLinks.count(key)){
    //check if key was already used
    if (linksUsedKeys.count(key)){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal PajeEndLink in "+eventdesc.str()+", the key was already used for another link";
    }
    link_t link;
    link.stime = -1;
    link.etime = time;
    link.startContainer = NULL;
    link.endContainer = endContainer;
    link.value = value;
    pendingLinks[key] = link;
    return;
  }

  //key was already used
  link_t link = pendingLinks[key];
  if (link.endContainer){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "Illegal PajeEndLink in "+eventdesc.str()+", the key was already used for another PajeEndLink";
  }

  //startLink was already registered for this key
  link.etime = time;
  link.endContainer = endContainer;
  if (link.value != value){
    std::stringstream eventdesc;
    eventdesc << *event;
    throw "Illegal PajeEndLink in "+eventdesc.str()+", value is different from the value of the corresponding PajeStartLink (which had "+link.value+")";
  }

  //checking time-ordered for this type
  link_t *last_link = NULL;
  if (links[type].size() != 0){
    last_link = &links[type].back();
  }
  if (last_link){
    if (last_link->stime > link.stime){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
    }
  }

  //push the newly completed link on the back of the vector
  links[type].push_back(link);

  //remove the link for the temporary pool, add the key to usedKeys
  pendingLinks.erase(key);
  linksUsedKeys.insert(key);
}

std::ostream &operator<< (std::ostream &output, const PajeContainer &container)
{
  output << "(Container, name: "
         << container.name
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

double PajeContainer::startTime (void)
{
  return stime;
}

double PajeContainer::endTime (void)
{
  return etime;
}

bool operator< (PajeUserVariable &t1, const double s)
{
  return t1.startTime() < s;
}


std::map<std::string,double> PajeContainer::timeIntegrationOfTypeInContainer (double start, double end, PajeType *type)
{

  std::map<std::string,double> empty;
  std::vector<PajeUserVariable> vector = variables[type];
  if (vector.size() == 0) return empty;

  std::vector<PajeUserVariable>::iterator low, up, it;
  low = lower_bound (vector.begin(), vector.end(), start);
  up = lower_bound (vector.begin(), vector.end(), end);

  if (low != vector.begin()){
    low--;
  }

  double tsDuration = end - start;
  double integrated = 0;
  for (it = low; it != up; it++){
    PajeUserVariable *var = &(*it);
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
  PajeContainerType *contType = dynamic_cast<PajeContainerType*>(type);
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
