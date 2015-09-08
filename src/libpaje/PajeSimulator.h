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
#ifndef __PAJESIMULATOR_H__
#define __PAJESIMULATOR_H__
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceEvent.h"
#include "PajeEvent.h"
#include "PajeType.h"
#include "PajeContainer.h"
#include "PajeComponent.h"

#define CALL_MEMBER_PAJE_SIMULATOR(object,ptr) ((object).*(ptr))

class PajeSimulator : public PajeComponent {
private:
  PajeContainerType *rootType;
  PajeContainer *root;
  std::map<std::string,PajeType*> typeMap;
  std::map<std::string,PajeType*> typeNamesMap; //for names
  std::map<std::string,PajeContainer*> contMap;
  std::map<std::string,PajeContainer*> contNamesMap; //for names


  double stopSimulationAtTime;

  void init (void);

private:
  //for time-slice selection (used by time aggregation)
  double selectionStart;
  double selectionEnd;

protected:
  double lastKnownTime;
  virtual void setLastKnownTime (PajeTraceEvent *event);
  PajeColor *getColor (std::string color, PajeTraceEvent *event);

public:
  PajeSimulator();
  PajeSimulator(double stopat);
  PajeSimulator(double stopat, int ignoreIncompleteLinks);
  ~PajeSimulator();
  void report (void);
  void reportDotFormat (void);
  void reportContainer (void);
  bool keepSimulating (void);
  
  void inputEntity (PajeObject *data);
  void startReading (void);
  void finishedReading (void);

private:
  void (PajeSimulator::*invocation[PajeEventIdCount])(PajeTraceEvent*);

  void pajeDefineContainerType (PajeTraceEvent *event);
  void pajeDefineLinkType (PajeTraceEvent *event);
  void pajeDefineEventType (PajeTraceEvent *event);
  void pajeDefineStateType (PajeTraceEvent *event);
  void pajeDefineVariableType (PajeTraceEvent *event);
  void pajeDefineEntityValue (PajeTraceEvent *event);

  void pajeCreateContainer (PajeTraceEvent *event);
  void pajeDestroyContainer (PajeTraceEvent *event);

  void pajeNewEvent (PajeTraceEvent *event);

  void pajeSetState (PajeTraceEvent *event);
  void pajePushState (PajeTraceEvent *event);
  void pajePopState (PajeTraceEvent *event);
  void pajeResetState (PajeTraceEvent *event);

  void pajeSetVariable (PajeTraceEvent *event);
  void pajeAddVariable (PajeTraceEvent *event);
  void pajeSubVariable (PajeTraceEvent *event);

  void pajeStartLink (PajeTraceEvent *event);
  void pajeEndLink (PajeTraceEvent *event);

  void pajeTraceFile (PajeTraceEvent *event); //suport for multiple files

public:
  //commands
  void setSelectionStartEndTime (double start, double end);

  //
  // Queries
  // -----------------------------------------------------------------
  // Messages sent by viewers or filters to the filter preceeding it.
  // These messages ask for some information about the loaded trace.
  //

  // The entity at the root of the hierarchy
  PajeContainer *rootInstance (void);

  // The type of the entity at the root of the hierarchy
  PajeType *rootEntityType (void);

  // Array of types that are directly under given type in hierarchy
  std::vector<PajeType*> containedTypesForContainerType (PajeType *type);

  // All containers contained by container
  std::vector<PajeContainer*> enumeratorOfContainersInContainer (PajeContainer *container);

  // All containers of a given type contained by container. Container must be
  // of a type ancestral of entityType in the hierarchy.
  std::vector<PajeContainer*> enumeratorOfContainersTypedInContainer (PajeType *type, PajeContainer *container);

  // All entities of a given type that are in a container in a certain
  // time interval.
  std::vector<PajeEntity*> enumeratorOfEntitiesTypedInContainer (PajeType *type, PajeContainer *container, double start, double end);

  // Tells if type is a container type
  bool isContainerType (PajeType *type);

  // Tells if type is a variable type
  bool isVariableType (PajeType *type);

  // The time period of the trace
  double startTime (void);
  double endTime (void);

  // The time selection
  double selectionStartTime (void);
  double selectionEndTime (void);

  // The type of the entity with the given name
  PajeType *entityTypeWithName (std::string name);

  // The container with the given name
  PajeContainer *containerWithName (std::string name);

  // Color of given value of entity type
  PajeColor *colorForValueOfEntityType (PajeType *type, PajeValue *value);

  // Color for all entities of given type (used for "variable" entity type).
  PajeColor *colorForEntityType (PajeType *type);

  // All values an entity of given type can have.
  std::vector<PajeValue*> valuesForEntityType (PajeType *type);

  // Spatial/time integration queries
  PajeAggregatedDict timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container);
  PajeAggregatedDict integrationOfContainer (PajeContainer *container);
  PajeAggregatedDict spatialIntegrationOfContainer (PajeContainer *container);
};
#endif
