#ifndef __PAJECOMPONENT_H__
#define __PAJECOMPONENT_H__
#include <iostream>
#include "PajeObject.h"
#include "PajeData.h"
#include "PajeContainer.h"

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

  //queries
  PajeContainer virtual *rootInstance (void);
  std::vector<PajeType*> virtual containedTypesForContainerType (PajeType *type);
  std::vector<PajeContainer*> virtual enumeratorOfContainersInContainer (PajeContainer *container);
  std::vector<PajeContainer*> virtual enumeratorOfContainersTypedInContainer (PajeType *type, PajeContainer *container);
  bool virtual isContainerType (PajeType *type);
  double virtual startTime (void);
  double virtual endTime (void);
  double virtual selectionStartTime (void);
  double virtual selectionEndTime (void);

  //spatial/time integration queries
  std::map<std::string,double> virtual timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container);
  std::map<std::string,double> virtual integrationOfContainer (PajeContainer *container);
  std::map<std::string,double> virtual spatialIntegrationOfContainer (PajeContainer *container);
};

#endif
