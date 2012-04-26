#include "PajeType.h"

PajeType::PajeType (std::string name, std::string alias, PajeType *parent)
{
  this->name = name;
  this->alias = alias;
  this->parent = parent;
}

std::string PajeType::identifier (void)
{
  return alias.empty() ? name : alias;
}

PajeVariableType::PajeVariableType  (std::string name, std::string alias, PajeType *parent):PajeType(name,alias,parent)
{
}

PajeStateType::PajeStateType  (std::string name, std::string alias, PajeType *parent):PajeType(name,alias,parent)
{
}

PajeEventType::PajeEventType  (std::string name, std::string alias, PajeType *parent):PajeType(name,alias,parent)
{
}

PajeLinkType::PajeLinkType (std::string name, std::string alias, std::string start, std::string end, PajeType *parent):PajeType(name,alias,parent)
{
  this->starttype = start;
  this->endtype = end;
}

PajeContainerType::PajeContainerType (std::string name, std::string alias, PajeType *parent):
  PajeType (name, alias, parent)
{
}

PajeType *PajeContainerType::getRootType (void)
{
  PajeType *root = this;
  while (root != NULL){
    if (root->parent != NULL) {
      root = root->parent;
    }else{
      break;
    }
  }
  return root;
}

PajeContainerType *PajeContainerType::addContainerType (std::string name, std::string alias)
{
  PajeContainerType *newType = new PajeContainerType (name, alias, this);
  children[newType->identifier()] = newType;
  return newType;
}

PajeVariableType *PajeContainerType::addVariableType (std::string name, std::string alias)
{
  PajeVariableType *newType = new PajeVariableType (name, alias, this);
  children[newType->identifier()] = newType;
  return newType;
}

PajeStateType *PajeContainerType::addStateType (std::string name, std::string alias)
{
  PajeStateType *newType = new PajeStateType (name, alias, this);
  children[newType->identifier()] = newType;
  return newType;
}

PajeEventType *PajeContainerType::addEventType (std::string name, std::string alias)
{
  PajeEventType *newType = new PajeEventType (name, alias, this);
  children[newType->identifier()] = newType;
  return newType;
}

PajeLinkType *PajeContainerType::addLinkType (std::string name, std::string alias, std::string starttype, std::string endtype)
{
  PajeLinkType *newType = new PajeLinkType (name, alias, starttype, endtype, this);
  children[newType->identifier()] = newType;
  return newType;
}

std::ostream &operator<< (std::ostream &output, const PajeType &type)
{
  output << "(Type, name: "
         << type.name
         << ", alias: " << type.alias << ")";
  return output;
}

bool operator!= (const PajeType& t1, const PajeType& t2)
{
  if (t1.alias.empty()){
    return t1.name != t2.name;
  }else{
    return t1.alias != t2.alias;
  }
}

bool operator== (const PajeType& t1, const PajeType& t2)
{
  if (t1.alias.empty()){
    return t1.name == t2.name;
  }else{
    return t1.alias == t2.alias;
  }
}
