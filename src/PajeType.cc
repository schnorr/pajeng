#include "PajeType.h"

PajeType::PajeType (std::string name, std::string alias, PajeType *parent)
{
  this->name = name;
  this->alias = alias;
  this->parent = parent;
}

bool PajeType::isContainer (void)
{
  return false;
}

const std::string PajeType::identifier (void)
{
  return alias.empty() ? name : alias;
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

void PajeContainerType::addChild (PajeType *child)
{
  std::list<PajeType*>::iterator it = children.end();
  children.insert (it, child);
}

bool PajeContainerType::isContainer(void)
{
  return true;
}

PajeContainerType *PajeContainerType::addContainerType (std::string name, std::string alias)
{
  PajeContainerType *newContainerType = new PajeContainerType (name, alias, this);
  addChild (newContainerType);
  return newContainerType;
}

PajeType *PajeContainerType::addType (std::string name, std::string alias)
{
  PajeType *newType = new PajeType (name, alias, this);
  addChild (newType);
  return newType;
}

PajeType *PajeContainerType::addVariableType (std::string name, std::string alias)
{
  return addType (name, alias);
}

PajeType *PajeContainerType::addStateType (std::string name, std::string alias)
{
  return addType (name, alias);
}

PajeType *PajeContainerType::addEventType (std::string name, std::string alias)
{
  return addType (name, alias);
}

PajeLinkType *PajeContainerType::addLinkType (std::string name, std::string alias, std::string starttype, std::string endtype)
{
  PajeLinkType *newType = new PajeLinkType (name, alias, starttype, endtype, this);
  addChild (newType);
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
