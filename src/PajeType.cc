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

PajeType *PajeType::search (std::string identifier)
{
  if (alias.empty()){
    if (this->name == identifier) return this;
  }else{
    if (this->alias == identifier) return this;
  }
  return NULL;
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

PajeType *PajeContainerType::search (std::string identifier)
{
  PajeType *found = PajeType::search(identifier);
  if (found) return found;

  //search on children
  std::list<PajeType*>::iterator it;
  for (it = children.begin(); it != children.end(); it++){
    PajeContainerType* containerchild = dynamic_cast<PajeContainerType*>(*it);
    if (containerchild){
      PajeType *found = containerchild->search (identifier);
      if (found) return found;
    }else{
      PajeType *found = (*it)->search (identifier);
      if (found) return found;
    }
  }
  return NULL;
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

bool PajeContainerType::addContainerType (std::string name, std::string alias)
{
  PajeType *rootType = getRootType ();

  //check if already exists
  std::string identifier;
  identifier = !alias.empty() ? alias : name;
  PajeType *existing = getRootType()->search (identifier);
  if (existing) return false;

  //define new container type
  PajeContainerType *newContainerType = new PajeContainerType (name, alias, this);

  //add it as a child of its container type
  addChild (newContainerType);
  return true;
}

bool PajeContainerType::addType (std::string name, std::string alias)
{
  //check if already exists
  std::string identifier;
  identifier = !alias.empty() ? alias : name;
  PajeType *existing = getRootType()->search (identifier);
  if (existing) return false;

  //define new variable type
  PajeType *newType = new PajeType (name, alias, this);

  //add it as a child of its container type
  addChild (newType);
  return true;
}

bool PajeContainerType::addVariableType (std::string name, std::string alias)
{
  return addType (name, alias);
}

bool PajeContainerType::addStateType (std::string name, std::string alias)
{
  return addType (name, alias);
}

bool PajeContainerType::addEventType (std::string name, std::string alias)
{
  return addType (name, alias);
}

bool PajeContainerType::addLinkType (std::string name, std::string alias, std::string starttype, std::string endtype)
{
  //check if already exists
  std::string identifier;
  identifier = !alias.empty() ? alias : name;
  PajeType *existing = getRootType()->search (identifier);
  if (existing) return false;

  //define new variable type
  PajeLinkType *newType = new PajeLinkType (name, alias, starttype, endtype, this);

  //add it as a child of its container type
  addChild (newType);
  return true;
}

