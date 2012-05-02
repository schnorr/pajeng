#ifndef __PAJE_CONTAINER_H
#define __PAJE_CONTAINER_H
#include <map>
#include <vector>
#include <string>
#include "PajeType.h"

class PajeContainer {
public:
  std::string name;
  std::string alias;
public:
  PajeContainer *parent;
  PajeType *type;

  std::map<std::string,PajeContainer*> children;

  //keeps the values of variables, states, events, links
  std::map<PajeType*,std::vector<double> > variables;
  std::map<PajeType*,std::vector<double> > states;
  std::map<PajeType*,std::vector<double> > events;
  std::map<PajeType*,std::map<std::string,double> > links;

public:
  PajeContainer (std::string name, std::string alias, PajeContainer *parent, PajeType *type);
  PajeContainer *getRoot (void);
  PajeContainer *addContainer (std::string name, std::string alias, PajeType *type);
  std::string identifier (void);
};

std::ostream &operator<< (std::ostream &output, const PajeContainer &container);

#endif
