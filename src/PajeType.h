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

class PajeContainerType;
class PajeLinkType;

class PajeType {
public:
  std::string name;
  std::string alias;
  PajeType *parent;

public:
  PajeType (std::string name, std::string alias, PajeType *parent);
  bool virtual isContainer (void);
  PajeType virtual *search (std::string identifier);

  friend class PajeContainerType;
  friend class PajeLinkType;
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
  std::list<PajeType*> children;

  PajeContainerType (std::string name, std::string alias, PajeType *parent);
  PajeType *search (std::string identifier);
  PajeType *getRootType (void);

  bool isContainer (void);
  bool addContainerType (std::string name, std::string alias);
  bool addVariableType (std::string name, std::string alias);
  bool addStateType (std::string name, std::string alias);
  bool addEventType (std::string name, std::string alias);
  bool addLinkType (std::string name, std::string alias, std::string starttype, std::string endtype);
private:
  bool addType (std::string name, std::string alias);
  void addChild (PajeType *type);
};

std::ostream &operator<< (std::ostream &output, const PajeType &type);
bool operator!= (const PajeType& t1, const PajeType& t2);
bool operator== (const PajeType& t1, const PajeType& t2);

#endif
