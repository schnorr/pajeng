/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Lesser Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJECOMPONENT_H__
#define __PAJECOMPONENT_H__
#include <iostream>
#include "PajeObject.h"
#include "PajeData.h"
#include "PajeContainer.h"
#include "PajeColor.h"

class PajeComponent : public PajeObject
{
private:
  PajeComponent *outputComponent;
  PajeComponent *inputComponent;

public:
  PajeComponent (void);

  void setInputComponent (PajeComponent *component);
  void setOutputComponent (PajeComponent *component);

  void virtual inputEntity (PajeObject *data);
  void virtual outputEntity (PajeObject *data);
  bool virtual canEndChunkBefore (PajeObject *data);
              
  void virtual startChunk (int chunkNumber); //a new chunk is starting
  void virtual endOfChunkLast (bool last);   //the current chunk is ending

  //notifications
  void virtual timeLimitsChanged (void);
  void virtual timeSelectionChanged (void);
  void virtual hierarchyChanged (void);

  //commands
  void virtual setSelectionStartEndTime (double start, double end);

  //queries
  PajeContainer virtual *rootInstance (void);
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
  virtual PajeContainer *containerWithName (std::string name);
  PajeColor virtual *colorForValueOfEntityType (std::string value, PajeType *type);
  PajeColor virtual *colorForEntityType (PajeType *type);

  //spatial/time integration queries
  std::map<std::string,double> virtual timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container);
  std::map<std::string,double> virtual integrationOfContainer (PajeContainer *container);
  std::map<std::string,double> virtual spatialIntegrationOfContainer (PajeContainer *container);
};

#endif
