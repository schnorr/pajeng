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
  std::map<PajeType*,std::vector<var_t> >::iterator it1;
  for (it1 = variables.begin(); it1 != variables.end(); it1++){
    var_t *last = &((*it1).second).back();
    last->etime = time;
  }

  //finish all states, events, ... //TODO FIXME
}

void PajeContainer::setVariable (double time, PajeType *type, double value, PajeEvent *event)
{
  var_t *last = NULL;
  if (variables[type].size() != 0){
    last = &variables[type].back();
  }
  if (last){
    if (last->stime > time){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
    }
    if (last->stime == time){
      //only update last value
      last->value = value;
      return;
    }else{
      last->etime = time;
    }
  }

  //create new
  var_t val;
  val.stime = time;
  val.etime = -1;
  val.value = value;
  variables[type].push_back(val);

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

  var_t *last = &variables[type].back();
  if (last->stime > time){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
  }
  if (last->stime == time){
    //only update last value
    last->value += value;
    return;
  }else{
    last->etime = time;
  }

  //create new
  var_t val;
  val.stime = time;
  val.etime = -1;
  val.value = last->value + value;
  variables[type].push_back(val);

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

  var_t *last = &variables[type].back();
  if (last->stime > time){
      std::stringstream eventdesc;
      eventdesc << *event;
      throw "Illegal, trace is not time-ordered in "+eventdesc.str();
  }
  if (last->stime == time){
    last->value -= value;
    return;
  }else{
    last->etime = time;
  }

  //create new
  var_t val;
  val.stime = time;
  val.etime = -1;
  val.value = last->value - value;
  variables[type].push_back(val);

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

bool operator< (const var_t& v1, const var_t& v2)
{
  return v1.stime < v2.stime;
}

std::ostream &operator<< (std::ostream &output, const var_t& v1)
{
  output << " VAR ["<< v1.stime << ", "<<v1.etime<<"] V=" <<v1.value;
  return output;
}

std::map<std::string,double> PajeContainer::timeIntegrationOfTypeInContainer (double start, double end, PajeType *type)
{
  std::map<std::string,double> empty;
  std::vector<var_t> vector = variables[type];
  if (vector.size() == 0) return empty;

  std::vector<var_t>::iterator low, up, it;
  var_t target;
  target.stime = start;
  low = lower_bound (vector.begin(), vector.end(), target);
  target.stime = end;
  up = lower_bound (vector.begin(), vector.end(), target);

  if (low != vector.begin()){
    low--;
  }

  double tsDuration = end - start;
  double integrated = 0;
  for (it = low; it != up; it++){
    var_t var = (*it);
    double s = var.stime;
    double e = var.etime;
    if (!var.value) continue;
    if (s < start) s = start;
    if (e > end) e = end;
    double duration = e - s;
    double var_integrated = duration/tsDuration * var.value;
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
