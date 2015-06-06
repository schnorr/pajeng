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
#include "PajeType.h"
#include "PajeException.h"

PajeType::PajeType (std::string name, std::string alias, PajeType *parent)
{
  _name = name;
  _alias = alias;
  _parent = parent;
  if (parent){
    _depth = parent->depth() + 1;
  }else{
    _depth = 0;
  }
}

PajeType::~PajeType ()
{
}

const std::string &PajeType::name (void) const
{
  return _name;
}

const std::string &PajeType::alias (void) const
{
  return _alias;
}

int PajeType::depth (void) const
{
  return _depth;
}

PajeType *PajeType::parent (void) const
{
  return _parent;
}

const std::string &PajeType::identifier (void) const
{
  return _alias.empty() ? _name : _alias;
}

std::string PajeType::kind (void) const
{
  switch (nature()){
    case PAJE_ContainerType: return std::string ("Container"); break;
    case PAJE_VariableType: return std::string ("Variable"); break;
    case PAJE_StateType: return std::string ("State"); break;
    case PAJE_EventType: return std::string ("Event"); break;
    case PAJE_LinkType: return std::string ("Link"); break;
    default: return std::string ("Unknown"); break;
  }
}

bool PajeType::isCategorizedType (void) const
{
  return false;
}

PajeValue *PajeType::addValue (std::string alias, std::string value, PajeColor *color)
{
  throw "should be implemented in subclass";
}

PajeValue *PajeType::valueForIdentifier (const std::string &identifier) const
{
  throw "should be implemented in subclass";
}

bool PajeType::hasValueForIdentifier (const std::string &identifier) const
{
  throw "should be implemented in subclass";
  return false;
}

PajeColor *PajeType::colorForIdentifier (const std::string &identifier) const
{
  throw "should be implemented in subclass";
}

PajeColor *PajeType::color (void) const
{
  throw "should be implemented in subclass";
}

PajeType *PajeType::startType (void) const
{
  return NULL;
}

PajeType *PajeType::endType (void) const
{
  return NULL;
}

PajeCategorizedType::PajeCategorizedType (std::string name, std::string alias, PajeType *parent)
  : PajeType(name,alias,parent)
{
}

bool PajeCategorizedType::isCategorizedType (void) const
{
  return true;
}

PajeValue *PajeCategorizedType::addValue (std::string alias, std::string value, PajeColor *color)
{
  PajeValue *newValue = new PajeValue (value, alias, this, color);
  values[newValue->identifier()] = newValue;
  colors[newValue->identifier()] = color;
  return newValue;
}

PajeValue *PajeCategorizedType::valueForIdentifier (const std::string &identifier) const
{
  std::map<std::string, PajeValue*>::const_iterator found = values.find(identifier);
  if (found != values.end()) {
    return found->second;
  } else {
    return NULL;
  }
}

bool PajeCategorizedType::hasValueForIdentifier (const std::string &identifier) const
{
  if (values.count(identifier)){
    return true;
  }else{
    return false;
  }
}

PajeColor *PajeCategorizedType::colorForIdentifier (const std::string &identifier) const
{
  std::map<std::string, PajeColor*>::const_iterator found = colors.find(identifier);
  if (found != colors.end()) {
    return found->second;
  } else {
    return NULL;
  }
}

PajeVariableType::PajeVariableType  (std::string name, std::string alias, PajeType *parent)
  : PajeType(name,alias,parent)
{
  _color = NULL;
}

PajeVariableType::PajeVariableType (std::string name, std::string alias, PajeType *parent, PajeColor *color)
 : PajeType(name,alias,parent)
{
  _color = color;
}

PajeDrawingType PajeVariableType::drawingType (void) const
{
  return PajeVariableDrawingType;
}

PajeTypeNature PajeVariableType::nature (void) const
{
  return PAJE_VariableType;
}

PajeColor *PajeVariableType::color (void) const
{
  return _color;
}

PajeStateType::PajeStateType  (std::string name, std::string alias, PajeType *parent):PajeCategorizedType(name,alias,parent)
{
}

PajeDrawingType PajeStateType::drawingType (void) const
{
  return PajeStateDrawingType;
}

PajeTypeNature PajeStateType::nature (void) const
{
  return PAJE_StateType;
}

PajeEventType::PajeEventType  (std::string name, std::string alias, PajeType *parent):PajeCategorizedType(name,alias,parent)
{
}

PajeDrawingType PajeEventType::drawingType (void) const
{
  return PajeEventDrawingType;
}

PajeTypeNature PajeEventType::nature (void) const
{
  return PAJE_EventType;
}

PajeLinkType::PajeLinkType (std::string name, std::string alias, PajeType *start, PajeType *end, PajeType *parent):PajeCategorizedType(name,alias,parent)
{
  this->starttype = start;
  this->endtype = end;
}

PajeDrawingType PajeLinkType::drawingType (void) const
{
  return PajeLinkDrawingType;
}

PajeTypeNature PajeLinkType::nature (void) const
{
  return PAJE_LinkType;
}

PajeType *PajeLinkType::startType (void) const
{
  return starttype;
}

PajeType *PajeLinkType::endType (void) const
{
  return endtype;
}

PajeType *PajeType::addContainerType (std::string name, std::string alias)
{
  throw PajeTypeException ("should not be called in this type");
  return NULL;
}

PajeType *PajeType::addVariableType (std::string name, std::string alias, PajeColor *color)
{
  throw PajeTypeException ("should not be called in this type");
  return NULL;
}

PajeType *PajeType::addStateType (std::string name, std::string alias)
{
  throw PajeTypeException ("should not be called in this type");
  return NULL;
}

PajeType *PajeType::addEventType (std::string name, std::string alias)
{
  throw PajeTypeException ("should not be called in this type");
  return NULL;
}

PajeType *PajeType::addLinkType (std::string name, std::string alias, PajeType *starttype, PajeType *endtype)
{
  throw PajeTypeException ("should not be called in this type");
  return NULL;
}

std::map<std::string,PajeType*> PajeType::children (void) const
{
  throw PajeTypeException ("should not be called in this type");
  return std::map<std::string,PajeType*>();
}

PajeContainerType::PajeContainerType (std::string name, std::string alias, PajeType *parent)
  : PajeType (name, alias, parent)
{
  _children = std::map<std::string,PajeType*>();
}

PajeContainerType::~PajeContainerType (void)
{
  std::map<std::string,PajeType*>::iterator child;
  for (child = _children.begin(); child != _children.end(); child++){
    delete (*child).second;
  }
  _children.clear();
}

PajeType *PajeContainerType::getRootType (void)
{
  PajeType *root = this;
  while (root != NULL){
    if (root->parent() != NULL) {
      root = root->parent();
    }else{
      break;
    }
  }
  return root;
}

PajeType *PajeContainerType::addContainerType (std::string name, std::string alias)
{
  PajeType *newType = new PajeContainerType (name, alias, this);
  _children[newType->identifier()] = newType;
  return newType;
}

PajeType *PajeContainerType::addVariableType (std::string name, std::string alias, PajeColor *color)
{
  PajeType *newType = new PajeVariableType (name, alias, this, color);
  _children[newType->identifier()] = newType;
  return newType;
}

PajeType *PajeContainerType::addStateType (std::string name, std::string alias)
{
  PajeType *newType = new PajeStateType (name, alias, this);
  _children[newType->identifier()] = newType;
  return newType;
}

PajeType *PajeContainerType::addEventType (std::string name, std::string alias)
{
  PajeType *newType = new PajeEventType (name, alias, this);
  _children[newType->identifier()] = newType;
  return newType;
}

PajeType *PajeContainerType::addLinkType (std::string name, std::string alias, PajeType *starttype, PajeType *endtype)
{
  PajeType *newType = new PajeLinkType (name, alias, starttype, endtype, this);
  _children[newType->identifier()] = newType;
  return newType;
}

std::map<std::string,PajeType*> PajeContainerType::children (void) const
{
  return _children;
}

PajeDrawingType PajeContainerType::drawingType (void) const
{
  return PajeContainerDrawingType;
}

PajeTypeNature PajeContainerType::nature (void) const
{
  return PAJE_ContainerType;
}

std::ostream &operator<< (std::ostream &output, const PajeType &type)
{
  output << "(Type, name: "
         << type.name()
         << ", alias: " << type.alias() << ")";
  return output;
}

bool operator!= (const PajeType& t1, const PajeType& t2)
{
  return t1.identifier() != t2.identifier();
}

bool operator== (const PajeType& t1, const PajeType& t2)
{
  return t1.identifier() == t2.identifier();
}

PajeAggregatedType::PajeAggregatedType (PajeType *type, PajeValue *value)
{
  this->aggregatedType = type;
  this->aggregatedValue = value;
}

PajeAggregatedType::PajeAggregatedType (PajeType *type)
{
  this->aggregatedType = type;
  this->aggregatedValue = NULL;
}

PajeColor *PajeAggregatedType::color (void) const
{
  if (aggregatedType->isCategorizedType()){
    return aggregatedType->colorForIdentifier (aggregatedValue->identifier());
  }else{
    return aggregatedType->color();
  }
}

PajeType *PajeAggregatedType::type (void) const
{
  return aggregatedType;
}

PajeValue *PajeAggregatedType::value (void) const
{
  return aggregatedValue;
}

const std::string &PajeAggregatedType::name (void) const
{
  if (aggregatedType->isCategorizedType()){
    return aggregatedValue->name();
  }else{
    return aggregatedType->name();
  }
}
