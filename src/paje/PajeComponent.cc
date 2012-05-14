#include "PajeComponent.h"

PajeComponent::PajeComponent (void)
{
  inputComponent = NULL;
  outputComponent = NULL;
}

void PajeComponent::setInputComponent (PajeComponent *component)
{
  inputComponent = component;
}

void PajeComponent::setOutputComponent (PajeComponent *component)
{
  outputComponent = component;
}

void PajeComponent::inputEntity (PajeObject *data)
{
  throw "PajeComponent::inputEntity should be implemented in the subclass";
}

void PajeComponent::outputEntity (PajeObject *data)
{
  if (outputComponent) outputComponent->inputEntity (data);
}

bool PajeComponent::canEndChunkBefore (PajeObject *data)
{
  if (outputComponent) return outputComponent->canEndChunkBefore (data);
  else return true;
}

void PajeComponent::startChunk (int chunkNumber)
{
  if (outputComponent) outputComponent->startChunk (chunkNumber);
}

void PajeComponent::endOfChunkLast (bool last)
{
  if (outputComponent) outputComponent->endOfChunkLast (last);
}

//notifications
void PajeComponent::timeLimitsChanged (void)
{
  if (outputComponent) outputComponent->timeLimitsChanged ();
}

void PajeComponent::timeSelectionChanged (void)
{
  if (outputComponent) outputComponent->timeSelectionChanged ();
}

void PajeComponent::hierarchyChanged (void)
{
  if (outputComponent) outputComponent->hierarchyChanged ();
}

//commands
void PajeComponent::setSelectionStartEndTime (double start, double end)
{
  if (inputComponent) inputComponent->setSelectionStartEndTime (start, end);
}

//queries
PajeContainer *PajeComponent::rootInstance (void)
{
  if (inputComponent) return inputComponent->rootInstance ();
  else return NULL;
}

std::vector<PajeType*> PajeComponent::containedTypesForContainerType (PajeType *type)
{
  std::vector<PajeType*> empty;
  if (inputComponent) return inputComponent->containedTypesForContainerType (type);
  else return empty;
}

std::vector<PajeContainer*> PajeComponent::enumeratorOfContainersInContainer (PajeContainer *container)
{
  std::vector<PajeContainer*> empty;
  if (inputComponent) return inputComponent->enumeratorOfContainersInContainer (container);
  else return empty;
}

std::vector<PajeContainer*> PajeComponent::enumeratorOfContainersTypedInContainer (PajeType *type, PajeContainer *container)
{
  std::vector<PajeContainer*> empty;
  if (inputComponent) return inputComponent->enumeratorOfContainersTypedInContainer (type, container);
  else return empty;
}

bool PajeComponent::isContainerType (PajeType *type)
{
  if (inputComponent) return inputComponent->isContainerType (type);
  else return false;
}

double PajeComponent::startTime (void)
{
  if (inputComponent) return inputComponent->startTime ();
  else return -1;
}

double PajeComponent::endTime (void)
{
  if (inputComponent) return inputComponent->endTime ();
  else return -1;
}

double PajeComponent::selectionStartTime (void)
{
  if (inputComponent) return inputComponent->selectionStartTime ();
  else return -1;
}

double PajeComponent::selectionEndTime (void)
{
  if (inputComponent) return inputComponent->selectionEndTime ();
  else return -1;
}

PajeType *PajeComponent::entityTypeWithName (std::string name)
{
  if (inputComponent) return inputComponent->entityTypeWithName (name);
  else return NULL;
}

std::map<std::string,double> PajeComponent::timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container)
{
  std::map<std::string,double> empty;
  if (inputComponent) return inputComponent->timeIntegrationOfTypeInContainer (type, container);
  else return empty;
}

std::map<std::string,double> PajeComponent::integrationOfContainer (PajeContainer *container)
{
  std::map<std::string,double> empty;
  if (inputComponent) return inputComponent->integrationOfContainer (container);
  return empty;
}

std::map<std::string,double> PajeComponent::spatialIntegrationOfContainer (PajeContainer *container)
{
  std::map<std::string,double> empty;
  if (inputComponent) return inputComponent->spatialIntegrationOfContainer (container);
  else return empty;
}
