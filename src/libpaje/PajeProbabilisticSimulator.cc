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
#include <iomanip>
#include "PajeProbabilisticSimulator.h"

extern int dumpFloatingPointPrecision;

PajeProbabilisticSimulator::PajeProbabilisticSimulator (char *filteredTypeName) : PajeSimulator()
{
  filter = std::string (filteredTypeName);
}

void PajeProbabilisticSimulator::setLastKnownTime (PajeTraceEvent *event)
{
  PajeSimulator::setLastKnownTime (event);
  static double lasttime = -1;

  if (lasttime == -1)
    lasttime = lastKnownTime;
    
  if (lastKnownTime > lasttime){
    //mark change on timestamp

    PajeProbabilisticSimulator::reportCurrentState (lasttime, lastKnownTime);
    lasttime = lastKnownTime;
  }  
}

void PajeProbabilisticSimulator::reportCurrentState (double start, double end)
{
  std::cout << std::fixed << std::setprecision (dumpFloatingPointPrecision) << start << "," << end << ",";
  std::cout << end-start;

  std::vector<PajeContainer*> stack;
  stack.push_back (this->rootInstance());

  while (!stack.empty()){
    PajeContainer *container = stack.back();
    stack.pop_back ();

    std::vector<PajeType*> containedTypes;
    std::vector<PajeType*>::iterator it;
    containedTypes = this->containedTypesForContainerType (container->type());
    for (it = containedTypes.begin(); it != containedTypes.end(); it++){
      PajeType *type = *it;
      if (this->isContainerType (type)){
	std::vector<PajeContainer*> children;
	std::vector<PajeContainer*>::iterator it;
	children = this->enumeratorOfContainersTypedInContainer (type, container);
	for (it = children.begin(); it != children.end(); it++){
	  stack.push_back (*it);
	}
      }else{
	std::vector<PajeEntity*> entities;
	std::vector<PajeEntity*>::iterator it;
	entities = this->enumeratorOfEntitiesTypedInContainer (type,
							       container,
							       start,
							       end);
	it = entities.begin();
	if (it != entities.end()){
	  PajeEntity *entity = *it;
	  if (entity->type()){
	    if (entity->type()->name() == filter){
	      std::cout << ",";
	      std::cout << std::fixed << std::setprecision (dumpFloatingPointPrecision);
	      std::cout << (entity->container()? entity->container()->name() : "NULL") << "/";
	      std::cout << (entity->type()? entity->type()->name() : "NULL") << "/";
	      if (dynamic_cast<PajeUserVariable*>(entity)){
		std::cout << entity->doubleValue();
	      }else{
		std::cout << (entity->value()? entity->value()->name() : "NULL");
	      }
	    }
	  }
	}
      }
    }
  }
  std::cout << std::endl;
}
