/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeType.h"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

PajeType::PajeType (std::string name, std::string alias, PajeType *parent)
{
  this->name = name;
  this->alias = alias;
  this->parent = parent;
  if (parent){
    this->depth = parent->depth + 1;
  }else{
    this->depth = 0;
  }
}

std::string PajeType::identifier (void) const
{
  return alias.empty() ? name : alias;
}

PajeVariableType::PajeVariableType  (std::string name, std::string alias, PajeType *parent):PajeType(name,alias,parent)
{
  color = new PajeColor (1, 1, 1, 1); // white
}

PajeVariableType::PajeVariableType (std::string name, std::string alias, PajeType *parent, std::string c):PajeType(name,alias,parent)
{
  if (c.empty()){
    color = new PajeColor (1, 1, 1, 1); //white
  }else{
    std::vector<float> v;
    boost::char_separator<char> sep(", ");
    boost::tokenizer< boost::char_separator<char> > tokens(c, sep);
    BOOST_FOREACH(std::string t, tokens) {
      v.push_back (atof(t.c_str()));
    }
    color = new PajeColor (v[0], v[1], v[2], v[3]);
  }
}

PajeStateType::PajeStateType  (std::string name, std::string alias, PajeType *parent):PajeType(name,alias,parent)
{
}

PajeEventType::PajeEventType  (std::string name, std::string alias, PajeType *parent):PajeType(name,alias,parent)
{
}

PajeLinkType::PajeLinkType (std::string name, std::string alias, PajeType *start, PajeType *end, PajeType *parent):PajeType(name,alias,parent)
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

PajeVariableType *PajeContainerType::addVariableType (std::string name, std::string alias, std::string color)
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

std::ostream &operator<< (std::ostream &output, const PajeType &type)
{
  output << "(Type, name: "
         << type.name
         << ", alias: " << type.alias << ")";
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
