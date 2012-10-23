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

  double lastKnownTime;

private:
  //for time-slice selection (used by time aggregation)
  double selectionStart;
  double selectionEnd;

  void setLastKnownTime (PajeEvent *event);
  PajeColor *getColor (std::string color, PajeEvent *event);

public:
  PajeSimulator();
  ~PajeSimulator();
  void report (void);
  
  void inputEntity (PajeObject *data);
  void startReading (void);
  void finishedReading (void);

private:
  void (PajeSimulator::*invocation[PajeEventIdCount])(PajeEvent*);

  void pajeDefineContainerType (PajeEvent *event);
  void pajeDefineLinkType (PajeEvent *event);
  void pajeDefineEventType (PajeEvent *event);
  void pajeDefineStateType (PajeEvent *event);
  void pajeDefineVariableType (PajeEvent *event);
  void pajeDefineEntityValue (PajeEvent *event);

  void pajeCreateContainer (PajeEvent *event);
  void pajeDestroyContainer (PajeEvent *event);

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

public:
  //commands
  void setSelectionStartEndTime (double start, double end);

  //queries
  PajeContainer *rootInstance (void);
  PajeType *rootEntityType (void);
  std::vector<PajeType*> containedTypesForContainerType (PajeType *type);
  std::vector<PajeContainer*> enumeratorOfContainersInContainer (PajeContainer *container);
  std::vector<PajeContainer*> enumeratorOfContainersTypedInContainer (PajeType *type, PajeContainer *container);
  std::vector<PajeEntity*> enumeratorOfEntitiesTypedInContainer (PajeType *type, PajeContainer *container, double start, double end);
  bool isContainerType (PajeType *type);
  bool isVariableType (PajeType *type);
  double startTime (void);
  double endTime (void);
  double selectionStartTime (void);
  double selectionEndTime (void);
  PajeType *entityTypeWithName (std::string name);
  PajeContainer *containerWithName (std::string name);
  PajeColor *colorForValueOfEntityType (PajeType *type, PajeValue *value);
  PajeColor *colorForEntityType (PajeType *type);
  std::vector<PajeValue*> valuesForEntityType (PajeType *type);
  
  //spatial/time integration queries
  PajeAggregatedDict timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container);
  PajeAggregatedDict integrationOfContainer (PajeContainer *container);
  PajeAggregatedDict spatialIntegrationOfContainer (PajeContainer *container);
};
#endif
