#include "PajeSimulator.h"

PajeContainer *PajeSimulator::rootInstance (void)
{
  return root;
}

std::vector<PajeType*> PajeSimulator::containedTypesForContainerType (PajeType *type)
{
  std::vector<PajeType *> ret;
  PajeContainerType *contType = dynamic_cast<PajeContainerType*>(type);
  if (!contType){
    throw "Type is not a container type";
  }
  std::map<std::string,PajeType*>::iterator it;
  for (it = contType->children.begin(); it != contType->children.end(); it++){
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
  std::vector<PajeContainer*> ret;
  std::map<std::string,PajeContainer*>::iterator it;
  for (it = container->children.begin(); it != container->children.end(); it++){
    if (((*it).second)->type == type){
      ret.push_back ((*it).second);
    }
  }
  return ret;
}

bool PajeSimulator::isContainerType (PajeType *type)
{
  PajeContainerType *contType = dynamic_cast<PajeContainerType*>(type);
  if (contType) return true;
  else return false;
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
  return typeNamesMap[name];
}

PajeColor *PajeSimulator::colorForValueOfEntityType (std::string value, PajeType *type)
{
  PajeVariableType *varType = dynamic_cast<PajeVariableType*>(type);
  if (varType){
    return varType->color;
  }else{
    return NULL;
  }
}

PajeColor *PajeSimulator::colorForEntityType (PajeType *type)
{
  return colorForValueOfEntityType (std::string(), type);
}

std::map<std::string,double> PajeSimulator::timeIntegrationOfTypeInContainer (PajeType *type, PajeContainer *container)
{
  std::map<std::string,double> ret;
  if (container){
    ret = container->timeIntegrationOfTypeInContainer (selectionStart, selectionEnd, type);
  }
  return ret;
}

std::map<std::string,double> PajeSimulator::integrationOfContainer (PajeContainer *container)
{
  std::map<std::string,double> ret;
  if (container){
    ret = container->integrationOfContainer (selectionStart, selectionEnd);
  }
  return ret;
}

std::map<std::string,double> PajeSimulator::spatialIntegrationOfContainer (PajeContainer *container)
{
  std::map<std::string,double> ret;
  if (container){
    ret = container->spatialIntegrationOfContainer (selectionStart, selectionEnd);
  }
  return ret;
}
