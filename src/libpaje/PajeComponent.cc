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
#include "PajeComponent.h"

PajeComponent::PajeComponent (void)
{
  inputComponent = NULL;
}

void PajeComponent::setInputComponent (PajeComponent *component)
{
  inputComponent = component;
}

void PajeComponent::setOutputComponent (PajeComponent *component)
{
  outputComponent.push_back(component);
}

void PajeComponent::disconnectFromInput (void)
{
  inputComponent = NULL;
}

void PajeComponent::disconnectOutputComponent (PajeComponent *component)
{
  std::vector<PajeComponent*>::iterator it;
  it = std::find (outputComponent.begin(), outputComponent.end(), component);
  if (it != outputComponent.end()){
    outputComponent.erase (it);
  }
}

void PajeComponent::inputEntity (PajeObject *data)
{
  throw "PajeComponent::inputEntity should be implemented in the subclass";
}

void PajeComponent::outputEntity (PajeObject *data)
{
  if (outputComponent.size()){
    std::vector<PajeComponent*>::iterator it;
    for (it = outputComponent.begin(); it != outputComponent.end(); it++){
      (*it)->inputEntity (data);
    }
  }
}

void PajeComponent::startReading (void)
{
  if (outputComponent.size()){
    std::vector<PajeComponent*>::iterator it;
    for (it = outputComponent.begin(); it != outputComponent.end(); it++){
      (*it)->startReading ();
    }
  }
}

void PajeComponent::finishedReading (void)
{
  if (outputComponent.size()){
    std::vector<PajeComponent*>::iterator it;
    for (it = outputComponent.begin(); it != outputComponent.end(); it++){
      (*it)->finishedReading ();
    }
  }
}

//notifications
void PajeComponent::timeLimitsChanged (void)
{
  if (outputComponent.size()){
    std::vector<PajeComponent*>::iterator it;
    for (it = outputComponent.begin(); it != outputComponent.end(); it++){
      (*it)->timeLimitsChanged ();
    }
  }
}

void PajeComponent::timeSelectionChanged (void)
{
  if (outputComponent.size()){
    std::vector<PajeComponent*>::iterator it;
    for (it = outputComponent.begin(); it != outputComponent.end(); it++){
      (*it)->timeSelectionChanged ();
    }
  }
}

void PajeComponent::hierarchyChanged (void)
{
  if (outputComponent.size()){
    std::vector<PajeComponent*>::iterator it;
    for (it = outputComponent.begin(); it != outputComponent.end(); it++){
      (*it)->hierarchyChanged ();
    }
  }
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

PajeType *PajeComponent::rootEntityType (void)
{
  if (inputComponent) return inputComponent->rootEntityType ();
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

std::vector<PajeEntity*>  PajeComponent::enumeratorOfEntitiesTypedInContainer (PajeType *type, PajeContainer *container, double start, double end)
{
  std::vector<PajeEntity*> empty;
  if (inputComponent) return inputComponent->enumeratorOfEntitiesTypedInContainer (type, container, start, end);
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

PajeContainer *PajeComponent::containerWithName (std::string name)
{
  if (inputComponent) return inputComponent->containerWithName (name);
    else return NULL;
}

PajeColor *PajeComponent::colorForValueOfEntityType (std::string value, PajeType *type)
{
  if (inputComponent) return inputComponent->colorForValueOfEntityType (value, type);
  else return NULL;
}

PajeColor *PajeComponent::colorForEntityType (PajeType *type)
{
  if (inputComponent) return inputComponent->colorForEntityType (type);
  else return NULL;
}

std::vector<std::string> PajeComponent::valuesForEntityType (PajeType *type)
{
  std::vector<std::string> empty;
  if (inputComponent) return inputComponent->valuesForEntityType (type);
  else return empty;
}

PajeAggregatedDict PajeComponent::timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container)
{
  PajeAggregatedDict empty;
  if (inputComponent) return inputComponent->timeIntegrationOfTypeInContainer (type, container);
  else return empty;
}

PajeAggregatedDict PajeComponent::integrationOfContainer (PajeContainer *container)
{
  PajeAggregatedDict empty;
  if (inputComponent) return inputComponent->integrationOfContainer (container);
  return empty;
}

PajeAggregatedDict PajeComponent::spatialIntegrationOfContainer (PajeContainer *container)
{
  PajeAggregatedDict empty;
  if (inputComponent) return inputComponent->spatialIntegrationOfContainer (container);
  else return empty;
}

std::pair<double,std::vector<PajeContainer*> > PajeComponent::findBestAggregation (PajeContainer *root, PajeAggregatedType *type, double p)
{
  std::pair<double,std::vector<PajeContainer*> > empty;
  if (inputComponent) return inputComponent->findBestAggregation (root, type, p);
  else return empty;
}
