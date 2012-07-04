#include "STEntityTypeLayout.h"

STTypeLayout::STTypeLayout (PajeType *type, STContainerTypeLayout *parentLayout)
{
  this->entityType = type;
  this->parentContainerLayout = parentLayout;
}

STContainerTypeLayout *STTypeLayout::parentLayout (void)
{
  return parentContainerLayout;
}

PajeType *STTypeLayout::type (void)
{
  return entityType;
}

std::vector<STTypeLayout*> STTypeLayout::subtypes (void)
{
  throw "this method should be implemented by subclasses";
  std::vector<STTypeLayout*> ret;
  return ret;
}

bool STTypeLayout::isContainer (void)
{
  return false;
}

double STTypeLayout::height (void)
{
  return 3;
}

double STTypeLayout::inset (void)
{
  return 0;
}

QPointF STTypeLayout::layoutPositionForContainer (PajeContainer *container)
{
  if (layoutPos.count(container)){
    return layoutPos[container];
  }else{
    std::cout << __FUNCTION__ << " exception, asking for layoutPosition of container " << container->name() << std::endl;
    throw "error";
  }
}

void STTypeLayout::setLayoutPositionForContainer (PajeContainer *container, QPointF pos)
{
  if (layoutPos.count(container)){
    std::cout << __FUNCTION__ << " exception" << std::endl;
    throw "error";
  }else{
    layoutPos[container] = pos;
  }
}

double STTypeLayout::layoutHeightForContainer (PajeContainer *container)
{
  if (layoutHeight.count(container)){
    return layoutHeight[container];
  }else{
    std::cout << __FUNCTION__ << " exception" << std::endl;
    throw "error";
  }
}

void STTypeLayout::setLayoutHeightForContainer (PajeContainer *container, double val)
{
  if (layoutHeight.count(container)){
    std::cout << __FUNCTION__ << " exception" << std::endl;
    throw "error";
  }else{
    layoutHeight[container] = val;
  }
}

STTypeLayout *STTypeLayout::Create (PajeType *type, STContainerTypeLayout *parentLayout)
{
  switch (type->drawingType()){
  case PajeEventDrawingType: return new STEventTypeLayout (type, parentLayout, 10);
  case PajeStateDrawingType: return new STStateTypeLayout (type, parentLayout, 1);
  case PajeLinkDrawingType: return new STLinkTypeLayout (type, parentLayout);
  case PajeVariableDrawingType: return new STVariableTypeLayout (type, parentLayout);
  case PajeContainerDrawingType: return new STContainerTypeLayout (type, parentLayout);
  }
  return NULL;
}

STEventTypeLayout::STEventTypeLayout (PajeType *type, STContainerTypeLayout *parentLayout, double width)
  : STTypeLayout (type, parentLayout)
{
  this->width = width;
}

STStateTypeLayout::STStateTypeLayout (PajeType *type, STContainerTypeLayout *parentLayout, double inset)
  : STTypeLayout (type, parentLayout)
{
  this->_inset = inset;
}

double STStateTypeLayout::inset (void)
{
  return _inset;
}

STLinkTypeLayout::STLinkTypeLayout (PajeType *type, STContainerTypeLayout *parentLayout)
  : STTypeLayout (type, parentLayout)
{
}

STVariableTypeLayout::STVariableTypeLayout (PajeType *type, STContainerTypeLayout *parentLayout)
  : STTypeLayout (type, parentLayout)
{
}

STContainerTypeLayout::STContainerTypeLayout (PajeType *type, STContainerTypeLayout *parentLayout)
  : STTypeLayout (type, parentLayout)
{
  _siblingSeparation = 4; // separation from other containers of same type in same container
  _subtypeSeparation = 1; // separation between two subtypes
}

QPointF STContainerTypeLayout::recursiveSetLayoutPositions (PajeContainer *container, PajeComponent *filter, QPointF current)
{
  QPointF top = current;
  setLayoutPositionForContainer (container, top);

  //for states
  {
    std::vector<STStateTypeLayout*>::iterator sit, aux;
    for (sit = stateSubtypes.begin(); sit != stateSubtypes.end(); sit++){
      STStateTypeLayout *stateTypeLayout = *sit;
      stateTypeLayout->setLayoutPositionForContainer (container, top);

      //check if there are entities of this type
      std::vector<PajeEntity*> subc;
      subc = filter->enumeratorOfEntitiesTypedInContainer (stateTypeLayout->type(),
                                                           container,
                                                           container->startTime(),
                                                           container->endTime());
      double h = 0;
      if (subc.size()){
        h = stateTypeLayout->height();
      }
      stateTypeLayout->setLayoutHeightForContainer (container, h);
      top += QPointF(0, h);

      {
        sit++;
        if (sit != stateSubtypes.end()){
          top += QPointF(0, subtypeSeparation());
        }
        sit--;
      }
    }
  }

  //for containers, recurse on each container type layout for each container instance
  {
    std::vector<STContainerTypeLayout*>::iterator cit;
    for (cit = containerSubtypes.begin(); cit != containerSubtypes.end(); cit++){
      STContainerTypeLayout *containerTypeLayout = *cit;
      QPointF container_top = top;
      containerTypeLayout->setLayoutPositionForContainer (container, container_top);

      //check if there are entities of this type
      std::vector<PajeContainer*> subc;
      std::vector<PajeContainer*>::iterator it;
      subc = filter->enumeratorOfContainersTypedInContainer (containerTypeLayout->type(), container);
      for (it = subc.begin(); it != subc.end(); it++){
        PajeContainer *subcontainer = *it;
        if (!subcontainer) throw "error";
        top = containerTypeLayout->recursiveSetLayoutPositions (subcontainer, filter, top);
        top += QPointF(0, siblingSeparation());
      }
      double h = 0;
      if (subc.size()){
        h = container_top.y() - top.y();
      }
      containerTypeLayout->setLayoutHeightForContainer (container, 0);

      {
        cit++;
        if (cit != containerSubtypes.end()){
          top += QPointF(0, subtypeSeparation());  
        }
        cit--;
      } 
    }
  }

  double h = top.y() - current.y();
  setLayoutHeightForContainer (container, h);
  return top;
}


std::vector<STTypeLayout*> STContainerTypeLayout::subtypes (void)
{
  std::vector<STTypeLayout*> ret;
  ret.insert (ret.begin(), stateSubtypes.begin(), stateSubtypes.end());
  ret.insert (ret.begin(), containerSubtypes.begin(), containerSubtypes.end());
  return ret;
}

void STContainerTypeLayout::addSubtype (STTypeLayout *sub)
{
  switch (sub->type()->drawingType()){
  case PajeEventDrawingType: break;
  case PajeStateDrawingType: stateSubtypes.push_back (dynamic_cast<STStateTypeLayout*>(sub)); break;
  case PajeLinkDrawingType: break;
  case PajeVariableDrawingType: break;
  case PajeContainerDrawingType: containerSubtypes.push_back (dynamic_cast<STContainerTypeLayout*>(sub)); break;
  }
}

bool STContainerTypeLayout::isContainer (void)
{
  return true;
}

double STContainerTypeLayout::subcontainersOffset (void)
{
  return _subcontainersOffset;
}

double STContainerTypeLayout::subtypeSeparation (void)
{
  return _subtypeSeparation;
}

double STContainerTypeLayout::siblingSeparation (void)
{
  return _siblingSeparation;
}
