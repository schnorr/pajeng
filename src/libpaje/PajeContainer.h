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
#ifndef __PAJE_CONTAINER_H
#define __PAJE_CONTAINER_H
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "PajeType.h"
#include "PajeTraceEvent.h"
#include "PajeEvent.h"
#include "PajeEntity.h"

class PajeContainer;
class PajeEvent;

class PajeContainer : public PajeNamedEntity {
private:
  double stopSimulationAtTime;
  void (PajeContainer::*invocation[PajeEventIdCount])(PajeEvent *);
  bool _destroyed;

public:
  std::string _alias;
  std::map<std::string,PajeContainer*> children;
  int depth;

private:
  std::map<PajeType*,std::set<std::string> > linksUsedKeys; //all used keys for this container
  std::map<PajeType*,std::map<std::string,PajeUserLink*> > pendingLinks; //all pending links
  std::map<PajeType*,std::vector<PajeUserState*> > stackStates; //the simulation stack for state types

  //keeps all simulated entities (variables, links, states and events)
  std::map<PajeType*,std::vector<PajeEntity*> > entities;

private:
  void init (std::string alias, PajeContainer *parent);

public:
  PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeType *type, PajeTraceEvent *event);
  PajeContainer (double time, std::string name, std::string alias, PajeContainer *parent, PajeType *type, PajeTraceEvent *event, double stopat);
  ~PajeContainer ();
  int numberOfEntities (void); //recursive
  std::string description (void) const;
  const std::string &identifier (void);
  bool isContainer (void) const;
  PajeContainer *getRoot (void);
  std::vector<PajeContainer*> getChildren (void);
  bool isAncestorOf (PajeContainer *c);
  bool keepSimulating (void);

  //entry method
  void demuxer (PajeEvent *event);

  //Simulator events (not treated by demuxer yet)
  PajeContainer *pajeCreateContainer (double time, PajeType *type, PajeTraceEvent *event, double stopat);
  void pajeDestroyContainer (double time, PajeEvent *event);
private:
  //Simulator events
  void pajeNewEvent (PajeEvent *event);
  void pajeSetState (PajeEvent *event);
  void pajePushState (PajeEvent *event);
  void pajePopState (PajeEvent *event);
  void pajeResetState (PajeEvent *event);
  void pajeSetVariable (PajeEvent *event);
  void pajeAddVariable (PajeEvent *event);
  void pajeSubVariable (PajeEvent *event);
  void pajeStartLink (PajeEvent *event);
  void pajeEndLink (PajeEvent *event);
  void pajeDestroyContainer (PajeEvent *event);

private:
  void destroy (double time);

public:
  void recursiveDestroy (double time); //not a PajeSimulator event, EOF found

  //queries
  std::vector<PajeEntity*> enumeratorOfEntitiesTyped (double start, double end, PajeType *type);
  PajeAggregatedDict timeIntegrationOfTypeInContainer (double start, double end, PajeType *type);
  PajeAggregatedDict timeIntegrationOfStateTypeInContainer (double start, double end, PajeType *type);
  PajeAggregatedDict timeIntegrationOfVariableTypeInContainer (double start, double end, PajeType *type);
  PajeAggregatedDict integrationOfContainer (double start, double end);
  PajeAggregatedDict spatialIntegrationOfContainer (double start, double end);

private:
  PajeAggregatedDict merge (PajeAggregatedDict a,
                            PajeAggregatedDict b);
  PajeAggregatedDict add (PajeAggregatedDict a,
                          PajeAggregatedDict b);
  bool checkTimeOrder (PajeEvent *event);
  bool checkTimeOrder (double time, PajeType *type, PajeTraceEvent *traceEvent);
  bool checkPendingLinks (void);


private:
  double selectionStart;
  double selectionEnd;
  PajeAggregatedDict spatialAggregated;
};

std::ostream &operator<< (std::ostream &output, const PajeContainer &container);

#endif
