#include "PajeContainer.h"

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
