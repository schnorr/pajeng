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
#ifndef __PAJETYPE_H__
#define __PAJETYPE_H__
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include "PajeTraceController.h"
#include "PajeColor.h"
#include "PajeProtocols.h"

class PajeType {
public:
  PajeColor *typeColor;
  std::string name;
  std::string alias;
  PajeType *parent;
  int depth;

public:
  PajeType (std::string name, std::string alias, PajeType *parent, PajeColor *color);
  virtual std::string identifier (void) const;
  virtual bool isCategorizedType (void) const;
  virtual void addNewValue (std::string alias, std::string value, PajeColor *color);
  virtual std::string valueForIdentifier (std::string identifier);
  virtual bool hasValueForIdentifier (std::string identifier);
  virtual PajeColor *colorForIdentifier (std::string identifier);
  virtual PajeColor *color (void);
  virtual PajeDrawingType drawingType (void) = 0;
};

class PajeCategorizedType : public PajeType {
public:
  PajeCategorizedType (std::string name, std::string alias, PajeType *parent);
  std::map<std::string,std::string> values;
  std::map<std::string,PajeColor*> colors;

  bool isCategorizedType (void) const;
  void addNewValue (std::string alias, std::string value, PajeColor *color);
  std::string valueForIdentifier (std::string identifier);
  bool hasValueForIdentifier (std::string identifier);
  PajeColor *colorForIdentifier (std::string identifier);
};

class PajeVariableType : public PajeType {
public:
  PajeVariableType (std::string name, std::string alias, PajeType *parent);
  PajeVariableType (std::string name, std::string alias, PajeType *parent, PajeColor *color);
  PajeDrawingType drawingType (void);
};

class PajeStateType : public PajeCategorizedType {
public:
  PajeStateType (std::string name, std::string alias, PajeType *parent);
  PajeDrawingType drawingType (void);
};

class PajeEventType : public PajeCategorizedType {
public:
  PajeEventType (std::string name, std::string alias, PajeType *parent);
  PajeDrawingType drawingType (void);
};

class PajeLinkType : public PajeCategorizedType {
public:
  PajeType *starttype;
  PajeType *endtype;

public:
  PajeLinkType (std::string name, std::string alias, PajeType *start, PajeType *end, PajeType *parent);
  PajeDrawingType drawingType (void);
};

class PajeContainerType : public PajeType {

public:
  std::map<std::string,PajeType*> children;

  PajeContainerType (std::string name, std::string alias, PajeType *parent);
  PajeType *getRootType (void);

  PajeContainerType *addContainerType (std::string name, std::string alias);
  PajeVariableType *addVariableType (std::string name, std::string alias, PajeColor *color);
  PajeStateType *addStateType (std::string name, std::string alias);
  PajeEventType *addEventType (std::string name, std::string alias);
  PajeLinkType *addLinkType (std::string name, std::string alias, PajeType *starttype, PajeType *endtype);
  PajeDrawingType drawingType (void);
};

std::ostream &operator<< (std::ostream &output, const PajeType &type);
bool operator!= (const PajeType& t1, const PajeType& t2);
bool operator== (const PajeType& t1, const PajeType& t2);

#endif
