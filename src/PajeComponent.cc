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
  outputComponent->inputEntity (data);
}

bool PajeComponent::canEndChunkBefore (PajeObject *data)
{
  outputComponent->canEndChunkBefore (data);
}

void PajeComponent::startChunk (int chunkNumber)
{
  outputComponent->startChunk (chunkNumber);
}

void PajeComponent::endOfChunkLast (bool last)
{
  outputComponent->endOfChunkLast (last);
}
