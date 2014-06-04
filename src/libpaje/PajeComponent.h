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
  bool virtual hasMoreData (void);
  void virtual readNextChunk (void);
  void virtual finishedReading (void);

  //notifications
  // Message sent when startTime or endTime of the whole visible trace changed
  void virtual timeLimitsChanged (void);
  // Message sent when the selected time slice has changed (or deselected).
  void virtual timeSelectionChanged (void);
  // Message sent when the hierarchy of types and/or containers has changed.
  void virtual hierarchyChanged (void);
  // Message sent when containers have been (de)selected.
  void virtual containerSelectionChanged (void);
  // Message sent when the selected entities have changed
  void virtual entitySelectionChanged (void);
  // Generic message. Used when something not specified in the other messages
  // has changed. entityType can be nil if not only one entityType is affected.
  void virtual dataChangedForEntityType (PajeType *type);
  // Message sent when the numeric limits of some variable entity type changed.
  void virtual limitsChangedForEntityType (PajeType *type);
  // Message sent when the color of something of entityType has changed.
  void virtual colorChangedForEntityType (PajeType *type);
  // Message sent when the order of the containers of some type has changed.
  void virtual orderChangedForContainerType (PajeType *type);

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
  PajeColor virtual *colorForValueOfEntityType (PajeType *type, PajeValue *value);
  PajeColor virtual *colorForEntityType (PajeType *type);
  std::vector<PajeValue*> virtual valuesForEntityType (PajeType *type);

  //spatial/time integration queries
  PajeAggregatedDict virtual timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container);
  PajeAggregatedDict virtual integrationOfContainer (PajeContainer *container);
  PajeAggregatedDict virtual spatialIntegrationOfContainer (PajeContainer *container);
};

#endif
