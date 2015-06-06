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
#include "PajeObject.h"
#include "PajeColor.h"
#include "PajeProtocols.h"
#include "PajeValue.h"

class PajeValue;

typedef enum {
  PAJE_ContainerType,
  PAJE_VariableType,
  PAJE_StateType,
  PAJE_EventType,
  PAJE_LinkType,
  PAJE_UndefinedType} PajeTypeNature;

class PajeType : public PajeObject {
protected:
  std::string _name;
  std::string _alias;
  PajeType *_parent;
  int _depth;

public:
  PajeType (std::string name, std::string alias, PajeType *parent);
  ~PajeType ();
  const std::string &name (void) const;
  const std::string &alias (void) const;
  int depth (void) const;
  PajeType *parent (void) const;
  const std::string &identifier (void) const;
  std::string kind (void) const;
  virtual bool isCategorizedType (void) const;
  virtual PajeValue *addValue (std::string alias, std::string value, PajeColor *color);
  virtual PajeValue *valueForIdentifier (const std::string &identifier) const;
  virtual bool hasValueForIdentifier (const std::string &identifier) const;
  virtual PajeColor *colorForIdentifier (const std::string &identifier) const;
  virtual PajeColor *color (void) const;
  virtual PajeDrawingType drawingType (void) const = 0;
  virtual PajeTypeNature nature (void) const = 0;
  virtual PajeType *startType (void) const;
  virtual PajeType *endType (void) const;
  virtual PajeType *addContainerType (std::string name, std::string alias);
  virtual PajeType *addVariableType (std::string name, std::string alias, PajeColor *color);
  virtual PajeType *addStateType (std::string name, std::string alias);
  virtual PajeType *addEventType (std::string name, std::string alias);
  virtual PajeType *addLinkType (std::string name, std::string alias, PajeType *starttype, PajeType *endtype);
  virtual std::map<std::string,PajeType*> children (void) const;
};

class PajeCategorizedType : public PajeType {
public:
  PajeCategorizedType (std::string name, std::string alias, PajeType *parent);
  std::map<std::string,PajeValue*> values;
  std::map<std::string,PajeColor*> colors;

  bool isCategorizedType (void) const;
  PajeValue *addValue (std::string alias, std::string value, PajeColor *color);
  PajeValue *valueForIdentifier (const std::string &identifier) const;
  bool hasValueForIdentifier (const std::string &identifier) const;
  PajeColor *colorForIdentifier (const std::string &identifier) const;
};

class PajeVariableType : public PajeType {
protected:
  PajeColor *_color;
public:
  PajeVariableType (std::string name, std::string alias, PajeType *parent);
  PajeVariableType (std::string name, std::string alias, PajeType *parent, PajeColor *color);
  PajeDrawingType drawingType (void) const;
  PajeTypeNature nature (void) const;
  PajeColor *color (void) const;
};

class PajeStateType : public PajeCategorizedType {
public:
  PajeStateType (std::string name, std::string alias, PajeType *parent);
  PajeDrawingType drawingType (void) const;
  PajeTypeNature nature (void) const;
};

class PajeEventType : public PajeCategorizedType {
public:
  PajeEventType (std::string name, std::string alias, PajeType *parent);
  PajeDrawingType drawingType (void) const;
  PajeTypeNature nature (void) const;
};

class PajeLinkType : public PajeCategorizedType {
private:
  PajeType *starttype;
  PajeType *endtype;

public:
  PajeLinkType (std::string name, std::string alias, PajeType *start, PajeType *end, PajeType *parent);
  PajeDrawingType drawingType (void) const;
  PajeTypeNature nature (void) const;
  PajeType *startType (void) const;
  PajeType *endType (void) const;
};

class PajeContainerType : public PajeType {
private:
  std::map<std::string,PajeType*> _children;

public:
  PajeContainerType (std::string name, std::string alias, PajeType *parent);
  ~PajeContainerType (void);
  PajeType *getRootType (void);

  PajeType *addContainerType (std::string name, std::string alias);
  PajeType *addVariableType (std::string name, std::string alias, PajeColor *color);
  PajeType *addStateType (std::string name, std::string alias);
  PajeType *addEventType (std::string name, std::string alias);
  PajeType *addLinkType (std::string name, std::string alias, PajeType *starttype, PajeType *endtype);
  std::map<std::string,PajeType*> children (void) const;
  PajeDrawingType drawingType (void) const;
  PajeTypeNature nature (void) const;
};

std::ostream &operator<< (std::ostream &output, const PajeType &type);
bool operator!= (const PajeType& t1, const PajeType& t2);
bool operator== (const PajeType& t1, const PajeType& t2);

class PajeAggregatedType {
private:
  PajeValue *aggregatedValue;
  PajeType *aggregatedType;

public:
  PajeAggregatedType (PajeType *type, PajeValue *value);
  PajeAggregatedType (PajeType *type);
  PajeColor *color (void) const;
  PajeType *type (void) const;
  PajeValue *value (void) const;
  const std::string &name (void) const;
};

struct PajeAggregatedTypeCompare {
  bool operator () (const PajeAggregatedType *t1, const PajeAggregatedType *t2) const {
    return t1->name() < t2->name();
  };
};

typedef std::pair<PajeAggregatedType*, double> PajeAggregatedDictEntry;
typedef std::map<PajeAggregatedType*, double, PajeAggregatedTypeCompare> PajeAggregatedDict;

#endif
