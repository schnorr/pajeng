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

