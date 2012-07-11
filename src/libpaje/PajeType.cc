/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeType.h"



PajeType::PajeType (std::string name, std::string alias, PajeType *parent, PajeColor *color)
{
  this->typeName = name;
  this->typeAlias = alias;
  this->typeParent = parent;
  this->typeColor = color;
  if (parent){
    this->typeDepth = parent->depth() + 1;
  }else{
    this->typeDepth = 0;
  }
}

std::string PajeType::name (void) const
{
  return typeName;
}

std::string PajeType::alias (void) const
{
  return typeAlias;
}

int PajeType::depth (void) const
{
  return typeDepth;
}

PajeType *PajeType::parent (void) const
{
  return typeParent;
}

std::string PajeType::identifier (void) const
{
  return typeAlias.empty() ? typeName : typeAlias;
}

bool PajeType::isCategorizedType (void) const
{
  return false;
}

void PajeType::addNewValue (std::string alias, std::string value, PajeColor *color)
{
  throw "should be implemented in subclass";
}

std::string PajeType::valueForIdentifier (std::string identifier)
{
  throw "should be implemented in subclass";
}

bool PajeType::hasValueForIdentifier (std::string identifier)
{
  throw "should be implemented in subclass";
  return false;
}

PajeColor *PajeType::colorForIdentifier (std::string identifier)
{
  throw "should be implemented in subclass";
}

PajeColor *PajeType::color (void)
{
  return typeColor;
}

PajeCategorizedType::PajeCategorizedType (std::string name, std::string alias, PajeType *parent)
  : PajeType(name,alias,parent, NULL)
{
}

bool PajeCategorizedType::isCategorizedType (void) const
{
  return true;
}

void PajeCategorizedType::addNewValue (std::string alias, std::string value, PajeColor *color)
{
  if (alias.empty()){
    values[value] = value;
  }else{
    values[alias] = value;
  }
  colors[value] = color;
  colors[alias] = color;
}

std::string PajeCategorizedType::valueForIdentifier (std::string identifier)
{
  if (values.count(identifier)){
    return values[identifier];
  }else{
    return NULL;
  }
}

bool PajeCategorizedType::hasValueForIdentifier (std::string identifier)
{
  if (values.count(identifier)){
    return true;
  }else{
    return false;
  }
}

PajeColor *PajeCategorizedType::colorForIdentifier (std::string identifier)
{
  if (colors.count(identifier)){
    return colors[identifier];
  }else{
    return NULL;
  }
}

PajeVariableType::PajeVariableType  (std::string name, std::string alias, PajeType *parent)
  : PajeType(name,alias,parent, NULL)
{
}

PajeVariableType::PajeVariableType (std::string name, std::string alias, PajeType *parent, PajeColor *color)
 : PajeType(name,alias,parent,color)
{

}

PajeDrawingType PajeVariableType::drawingType (void)
{
  return PajeVariableDrawingType;
}

PajeStateType::PajeStateType  (std::string name, std::string alias, PajeType *parent):PajeCategorizedType(name,alias,parent)
{
}

PajeDrawingType PajeStateType::drawingType (void)
{
  return PajeStateDrawingType;
}

PajeEventType::PajeEventType  (std::string name, std::string alias, PajeType *parent):PajeCategorizedType(name,alias,parent)
{
}

PajeDrawingType PajeEventType::drawingType (void)
{
  return PajeEventDrawingType;
}

PajeLinkType::PajeLinkType (std::string name, std::string alias, PajeType *start, PajeType *end, PajeType *parent):PajeCategorizedType(name,alias,parent)
{
  this->starttype = start;
  this->endtype = end;
}

PajeDrawingType PajeLinkType::drawingType (void)
{
  return PajeLinkDrawingType;
}

PajeContainerType::PajeContainerType (std::string name, std::string alias, PajeType *parent)
  : PajeType (name, alias, parent, NULL)
{
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

PajeContainerType *PajeContainerType::addContainerType (std::string name, std::string alias)
{
  PajeContainerType *newType = new PajeContainerType (name, alias, this);
  children[newType->identifier()] = newType;
  return newType;
}

PajeVariableType *PajeContainerType::addVariableType (std::string name, std::string alias, PajeColor *color)
{
  PajeVariableType *newType = new PajeVariableType (name, alias, this, color);
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

PajeLinkType *PajeContainerType::addLinkType (std::string name, std::string alias, PajeType *starttype, PajeType *endtype)
{
  PajeLinkType *newType = new PajeLinkType (name, alias, starttype, endtype, this);
  children[newType->identifier()] = newType;
  return newType;
}

PajeDrawingType PajeContainerType::drawingType (void)
{
  return PajeContainerDrawingType;
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

PajeAggregatedType::PajeAggregatedType (PajeType *type, std::string value)
{
  this->aggregatedType = type;
  this->aggregatedValue = value;
}

PajeAggregatedType::PajeAggregatedType (PajeType *type)
{
  this->aggregatedType = type;
}

PajeColor *PajeAggregatedType::color (void) const
{
  if (aggregatedValue.empty()){
    return aggregatedType->color();
  }else{
    return aggregatedType->colorForIdentifier (aggregatedValue);
  }
}

PajeType *PajeAggregatedType::type (void) const
{
  return aggregatedType;
}

std::string PajeAggregatedType::name (void) const
{
  if (aggregatedValue.empty()){
    return aggregatedType->name();
  }else{
    return aggregatedValue;
  }
}

bool operator== (const PajeAggregatedType& t1, const PajeAggregatedType& t2)
{
  return t1.name() == t2.name();
}
