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
void PajeComponent::hierarchyChanged (void)
{
  if (outputComponent) outputComponent->hierarchyChanged ();
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
