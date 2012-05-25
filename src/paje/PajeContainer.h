#ifndef __PAJE_CONTAINER_H
#define __PAJE_CONTAINER_H
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "PajeType.h"
#include "PajeEvent.h"
#include "PajeEntity.h"

class PajeContainer;

class PajeContainer : public PajeUserState {
public:
  std::string alias;
  bool destroyed;
public:
  std::map<std::string,PajeContainer*> children;

  //keeps the values of states, events
  std::map<PajeType*,std::vector<double> > states;
  std::map<PajeType*,std::vector<double> > events;

private:
  std::set<std::string> linksUsedKeys;
  std::map<std::string,PajeUserLink*> pendingLinks;
  std::map<PajeType*,std::vector<PajeEntity*> > entities;

public:
  PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeContainerType *type);
  bool isContainer (void);
  PajeContainer *getRoot (void);
  PajeContainer *addContainer (double time, std::string name, std::string alias, PajeContainerType *type, PajeEvent *event);
  std::string identifier (void);
  void destroy (double time, PajeEvent *event);
  void setVariable (double time, PajeType *type, double value, PajeEvent *event);
  void addVariable (double time, PajeType *type, double value, PajeEvent *event);
  void subVariable (double time, PajeType *type, double value, PajeEvent *event);
  void startLink (double time, PajeType *type, PajeContainer *startContainer, std::string value, std::string key, PajeEvent *event);
  void endLink (double time, PajeType *type, PajeContainer *endContainer, std::string value, std::string key, PajeEvent *event);

public:
  void recursiveDestroy (double time, PajeEvent *event); //not a PajeSimulator event, EOF found

  //queries
  std::vector<PajeEntity*> enumeratorOfEntitiesTyped (double start, double end, PajeType *type);
  std::map<std::string,double> timeIntegrationOfTypeInContainer (double start, double end, PajeType *type);
  std::map<std::string,double> integrationOfContainer (double start, double end);
  std::map<std::string,double> spatialIntegrationOfContainer (double start, double end);

private:
  std::map<std::string,double> merge (std::map<std::string,double> a,
                                      std::map<std::string,double> b);
  std::map<std::string,double> add (std::map<std::string,double> a,
                                    std::map<std::string,double> b);
};

std::ostream &operator<< (std::ostream &output, const PajeContainer &container);

#endif
