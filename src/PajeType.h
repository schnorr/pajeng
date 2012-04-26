#ifndef __PAJETYPE_H__
#define __PAJETYPE_H__
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeComponent.h"
#include "PajeEvent.h"

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
  PajeVariableType (std::string name, std::string alias, PajeType *parent);
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
private:
  std::string starttype;
  std::string endtype;

public:
  PajeLinkType (std::string name, std::string alias, std::string start, std::string end, PajeType *parent);
};

class PajeContainerType : public PajeType {

public:
  std::map<std::string,PajeType*> children;

  PajeContainerType (std::string name, std::string alias, PajeType *parent);
  PajeType *getRootType (void);

  PajeContainerType *addContainerType (std::string name, std::string alias);
  PajeVariableType *addVariableType (std::string name, std::string alias);
  PajeStateType *addStateType (std::string name, std::string alias);
  PajeEventType *addEventType (std::string name, std::string alias);
  PajeLinkType *addLinkType (std::string name, std::string alias, std::string starttype, std::string endtype);
};

std::ostream &operator<< (std::ostream &output, const PajeType &type);
bool operator!= (const PajeType& t1, const PajeType& t2);
bool operator== (const PajeType& t1, const PajeType& t2);

#endif
