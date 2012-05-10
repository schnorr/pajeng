#include "PajeContainer.h"

PajeContainer::PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeType *type)
{
  this->name = name;
  this->alias = alias;
  this->parent = parent;
  this->type = type;
  this->destroyed = false;
  this->stime = time;
  this->etime = time;
}

std::string PajeContainer::identifier ()
{
  return alias.empty() ? name : alias;
}

PajeContainer *PajeContainer::addContainer (double time, std::string name, std::string alias, PajeType *type, PajeEvent *event)
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
    last->value += value;
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
