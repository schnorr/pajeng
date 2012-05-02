#include "PajeComponent.h"
#include <stdlib.h>

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
