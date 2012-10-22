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
#ifndef __PAJECOMPONENT_H__
#define __PAJECOMPONENT_H__
#include <iostream>
#include <vector>
#include "PajeObject.h"
#include "PajeData.h"
#include "PajeContainer.h"
#include "PajeColor.h"
#include "PajeProtocols.h"

// PajeComponent : where the Paje protocol is declared
class PajeComponent : public PajeObject
{
private:
  std::vector<PajeComponent*> outputComponent;
  PajeComponent *inputComponent;

public:
  PajeComponent (void);

  void setInputComponent (PajeComponent *component);
  void setOutputComponent (PajeComponent *component);
  void disconnectFromInput (void);
  void disconnectOutputComponent (PajeComponent *component);

  void virtual inputEntity (PajeObject *data);
  void virtual outputEntity (PajeObject *data);
  void virtual startReading (void);
  void virtual finishedReading (void);

  //notifications
  void virtual timeLimitsChanged (void);
  void virtual timeSelectionChanged (void);
  void virtual hierarchyChanged (void);

  //commands
  void virtual setSelectionStartEndTime (double start, double end);

  //queries
  PajeContainer virtual *rootInstance (void);
  PajeType virtual *rootEntityType (void);
  std::vector<PajeType*> virtual containedTypesForContainerType (PajeType *type);
  std::vector<PajeContainer*> virtual enumeratorOfContainersInContainer (PajeContainer *container);
  std::vector<PajeContainer*> virtual enumeratorOfContainersTypedInContainer (PajeType *type, PajeContainer *container);
  std::vector<PajeEntity*> virtual enumeratorOfEntitiesTypedInContainer (PajeType *type, PajeContainer *container, double start, double end);
  bool virtual isContainerType (PajeType *type);
  double virtual startTime (void);
  double virtual endTime (void);
  double virtual selectionStartTime (void);
  double virtual selectionEndTime (void);
  PajeType virtual *entityTypeWithName (std::string name);
  PajeContainer virtual *containerWithName (std::string name);
  PajeColor virtual *colorForValueOfEntityType (std::string value, PajeType *type);
  PajeColor virtual *colorForEntityType (PajeType *type);
  std::vector<std::string> valuesForEntityType (PajeType *type);

  //spatial/time integration queries
  PajeAggregatedDict virtual timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container);
  PajeAggregatedDict virtual integrationOfContainer (PajeContainer *container);
  PajeAggregatedDict virtual spatialIntegrationOfContainer (PajeContainer *container);
};

#endif
