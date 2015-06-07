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
#include "PajeSimulator.h"
#include "PajeException.h"
#include <boost/foreach.hpp>

PajeContainer *PajeSimulator::rootInstance (void)
{
  return root;
}

PajeType *PajeSimulator::rootEntityType (void)
{
  return root->type();
}

std::vector<PajeType*> PajeSimulator::containedTypesForContainerType (PajeType *type)
{
  std::vector<PajeType *> ret;
  if (type->nature() != PAJE_ContainerType){
    throw PajeProtocolException ("Type is not a container type");
  }

  std::map<std::string,PajeType*> c = type->children();
  std::map<std::string,PajeType*>::iterator it;
  for (it = c.begin(); it != c.end(); it++){
    PajeType *t = (*it).second;
    ret.push_back ((*it).second);
  }
  return ret;
}

std::vector<PajeContainer*> PajeSimulator::enumeratorOfContainersInContainer (PajeContainer *container)
{
  std::vector<PajeContainer*> ret;
  std::map<std::string,PajeContainer*>::iterator it;
  for (it = container->children.begin(); it != container->children.end(); it++){
    ret.push_back ((*it).second);
  }
  return ret;
}

std::vector<PajeContainer*> PajeSimulator::enumeratorOfContainersTypedInContainer (PajeType *type, PajeContainer *container)
{
  if (type->nature() != PAJE_ContainerType){
    throw PajeProtocolException ("Type is not a container type");
  }
  std::vector<PajeContainer*> ret;
  std::map<std::string,PajeContainer*>::iterator it;
  for (it = container->children.begin(); it != container->children.end(); it++){
    if (((*it).second)->type() == type){
      ret.push_back ((*it).second);
    }
  }
  return ret;
}

std::vector<PajeEntity*> PajeSimulator::enumeratorOfEntitiesTypedInContainer (PajeType *type, PajeContainer *container, double start, double end)
{
  if (type->nature() == PAJE_ContainerType){
    throw PajeProtocolException ("Type is not an entity type");
  }
  std::vector<PajeEntity*> empty;
  if (container) return container->enumeratorOfEntitiesTyped (start, end, type);
  else return empty;
}

bool PajeSimulator::isContainerType (PajeType *type)
{
  return type->nature() == PAJE_ContainerType;
}

bool PajeSimulator::isVariableType (PajeType *type)
{
  return type->nature() == PAJE_VariableType;
}

double PajeSimulator::startTime (void)
{
  return root->startTime ();
}

double PajeSimulator::endTime (void)
{
  return root->endTime ();
}

double PajeSimulator::selectionStartTime (void)
{
  if (selectionStart >= 0){
    return selectionStart;
  }else{
    return startTime();
  }
}

double PajeSimulator::selectionEndTime (void)
{
  if (selectionEnd >= 0){
    return selectionEnd;
  }else{
    return endTime();
  }
}

PajeType *PajeSimulator::entityTypeWithName (std::string name)
{
  if (typeNamesMap.count(name)){
    return typeNamesMap[name];
  }else{
    return NULL;
  }
}

PajeContainer *PajeSimulator::containerWithName (std::string name)
{
  if (contNamesMap.count(name)){
    return contNamesMap[name];
  }else{
    return NULL;
  }
}

PajeColor *PajeSimulator::colorForValueOfEntityType (PajeType *type, PajeValue *value)
{
  return type->colorForIdentifier (value->identifier());
}

PajeColor *PajeSimulator::colorForEntityType (PajeType *type)
{
  return type->color ();
}

std::vector<PajeValue*> PajeSimulator::valuesForEntityType (PajeType *type)
{
  std::vector<PajeValue*> ret;
  PajeCategorizedType *catType = dynamic_cast<PajeCategorizedType*>(type);
  if (catType){
    std::pair<std::string,PajeValue*> val;
    BOOST_FOREACH (val, catType->values){
      ret.push_back (val.second);
    }
  }
  return ret;
}

PajeAggregatedDict PajeSimulator::timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container)
{
  PajeAggregatedDict ret;
  if (container){
    ret = container->timeIntegrationOfTypeInContainer (selectionStart, selectionEnd, type);
  }
  return ret;
}

PajeAggregatedDict PajeSimulator::integrationOfContainer (PajeContainer *container)
{
  PajeAggregatedDict ret;
  if (container){
    ret = container->integrationOfContainer (selectionStart, selectionEnd);
  }
  return ret;
}

PajeAggregatedDict PajeSimulator::spatialIntegrationOfContainer (PajeContainer *container)
{
  PajeAggregatedDict ret;
  if (container){
    ret = container->spatialIntegrationOfContainer (selectionStart, selectionEnd);
  }
  return ret;
}
