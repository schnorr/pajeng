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
  int depth;

private:
  std::set<std::string> linksUsedKeys; //all used keys for this container
  std::map<std::string,PajeUserLink*> pendingLinks; //all pending links
  std::map<PajeType*,std::vector<PajeUserState*> > stackStates; //the simulation stack for state types

  //keeps all simulated entities (variables, links, states and events)
  std::map<PajeType*,std::vector<PajeEntity*> > entities;

public:
  PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeContainerType *type, PajeEvent *event);
  std::string description (void);
  bool isContainer (void);
  PajeContainer *getRoot (void);
  std::vector<PajeContainer*> getChildren (void);
  bool isAncestorOf (PajeContainer *c);

  PajeContainer *addContainer (double time, std::string name, std::string alias, PajeContainerType *type, PajeEvent *event);
  std::string identifier (void);
  void destroy (double time, PajeEvent *event);
  void setVariable (double time, PajeType *type, double value, PajeEvent *event);
  void addVariable (double time, PajeType *type, double value, PajeEvent *event);
  void subVariable (double time, PajeType *type, double value, PajeEvent *event);
  void startLink (double time, PajeType *type, PajeContainer *startContainer, std::string value, std::string key, PajeEvent *event);
  void endLink (double time, PajeType *type, PajeContainer *endContainer, std::string value, std::string key, PajeEvent *event);
  void newEvent (double time, PajeType *type, std::string value, PajeEvent *event);
  void setState (double time, PajeType *type, std::string value, PajeEvent *event);
  void pushState (double time, PajeType *type, std::string value, PajeEvent *event);
  void popState (double time, PajeType *type, std::string value, PajeEvent *event);
  void resetState (double time, PajeType *type, std::string value, PajeEvent *event);

public:
  void recursiveDestroy (double time, PajeEvent *event); //not a PajeSimulator event, EOF found

  //queries
  std::vector<PajeEntity*> enumeratorOfEntitiesTyped (double start, double end, PajeType *type);
  std::map<std::string,double> timeIntegrationOfTypeInContainer (double start, double end, PajeType *type);
  std::map<std::string,double> timeIntegrationOfStateTypeInContainer (double start, double end, PajeStateType *type);
  std::map<std::string,double> timeIntegrationOfVariableTypeInContainer (double start, double end, PajeVariableType *type);
  std::map<std::string,double> integrationOfContainer (double start, double end);
  std::map<std::string,double> spatialIntegrationOfContainer (double start, double end);

private:
  std::map<std::string,double> merge (std::map<std::string,double> a,
                                      std::map<std::string,double> b);
  std::map<std::string,double> add (std::map<std::string,double> a,
                                    std::map<std::string,double> b);
  bool checkTimeOrder (double time, PajeType *type, PajeEvent *event);
};

std::ostream &operator<< (std::ostream &output, const PajeContainer &container);

#endif
