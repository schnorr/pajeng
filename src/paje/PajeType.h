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

class PajeType {
public:
  std::string name;
  std::string alias;
  PajeType *parent;

public:
  PajeType (std::string name, std::string alias, PajeType *parent);
  virtual std::string identifier (void) const;
};

class PajeVariableType : public PajeType {
public:
  PajeColor *color;
  PajeVariableType (std::string name, std::string alias, PajeType *parent);
  PajeVariableType (std::string name, std::string alias, PajeType *parent, std::string color);
};

class PajeStateType : public PajeType {
public:
  PajeStateType (std::string name, std::string alias, PajeType *parent);
};

class PajeEventType : public PajeType {
public:
  PajeEventType (std::string name, std::string alias, PajeType *parent);
};

class PajeLinkType : public PajeType {
public:
  PajeType *starttype;
  PajeType *endtype;

public:
  PajeLinkType (std::string name, std::string alias, PajeType *start, PajeType *end, PajeType *parent);
};

class PajeContainerType : public PajeType {

public:
  std::map<std::string,PajeType*> children;

  PajeContainerType (std::string name, std::string alias, PajeType *parent);
  PajeType *getRootType (void);

  PajeContainerType *addContainerType (std::string name, std::string alias);
  PajeVariableType *addVariableType (std::string name, std::string alias, std::string color);
  PajeStateType *addStateType (std::string name, std::string alias);
  PajeEventType *addEventType (std::string name, std::string alias);
  PajeLinkType *addLinkType (std::string name, std::string alias, PajeType *starttype, PajeType *endtype);
};

std::ostream &operator<< (std::ostream &output, const PajeType &type);
bool operator!= (const PajeType& t1, const PajeType& t2);
bool operator== (const PajeType& t1, const PajeType& t2);

#endif
