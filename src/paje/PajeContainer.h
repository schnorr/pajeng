#ifndef __PAJE_CONTAINER_H
#define __PAJE_CONTAINER_H
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "PajeType.h"
#include "PajeEvent.h"

typedef struct {
  double stime;
  double etime;
  double value;
}var_t;

class PajeContainer {
public:
  std::string name;
  std::string alias;
  bool destroyed;
  double stime;
  double etime;
public:
  PajeContainer *parent;
  PajeContainerType *type;

  std::map<std::string,PajeContainer*> children;

  //keeps the values of variables, states, events, links
  std::map<PajeType*,std::vector<double> > states;
  std::map<PajeType*,std::vector<double> > events;
  std::map<PajeType*,std::map<std::string,double> > links;

private:
  std::map<PajeType*,std::vector<var_t> > variables;

public:
  PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeContainerType *type);
  PajeContainer *getRoot (void);
  PajeContainer *addContainer (double time, std::string name, std::string alias, PajeContainerType *type, PajeEvent *event);
  std::string identifier (void);
  void destroy (double time, PajeEvent *event);
  void setVariable (double time, PajeType *type, double value, PajeEvent *event);
  void addVariable (double time, PajeType *type, double value, PajeEvent *event);
  void subVariable (double time, PajeType *type, double value, PajeEvent *event);

public:
  void recursiveDestroy (double time, PajeEvent *event); //not a PajeSimulator event, EOF found

  //queries
  double startTime (void);
  double endTime (void);
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
