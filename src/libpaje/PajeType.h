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
#ifndef __PAJETYPE_H__
#define __PAJETYPE_H__
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include "PajeColor.h"
#include "PajeProtocols.h"

class PajeType {
protected:
  PajeColor *typeColor;
  std::string typeName;
  std::string typeAlias;
  PajeType *typeParent;
  int typeDepth;

public:
  PajeType (std::string name, std::string alias, PajeType *parent, PajeColor *color);
  std::string name (void) const;
  std::string alias (void) const;
  int depth (void) const;
  PajeType *parent (void) const;
  std::string identifier (void) const;
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

class PajeAggregatedType {
private:
  std::string aggregatedValue;
  PajeType *aggregatedType;

public:
  PajeAggregatedType (PajeType *type, std::string value);
  PajeAggregatedType (PajeType *type);
  PajeColor *color (void) const;
  PajeType *type (void) const;
  std::string name (void) const;
};

struct PajeAggregatedTypeCompare {
  bool operator () (const PajeAggregatedType *t1, const PajeAggregatedType *t2) const {
    return t1->name() < t2->name();
  };
};

typedef std::map<PajeAggregatedType*, double, PajeAggregatedTypeCompare> PajeAggregatedDict;

#endif
