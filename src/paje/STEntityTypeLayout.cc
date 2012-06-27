#include "STEntityTypeLayout.h"

STTypeLayout::STTypeLayout (PajeType *type)
{
  this->entityType = type;
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
  return 10;
}

double STTypeLayout::offset (void)
{
  return offset_verticalaxis;
}

void STTypeLayout::setOffset (double val)
{
  offset_verticalaxis = val;
}

void STTypeLayout::setRectInContainer (QRectF *rect, PajeContainer *container)
{
  rectInContainer[container] = rect;
}

STTypeLayout *STTypeLayout::Create (PajeType *type)
{
  switch (type->drawingType()){
  case PajeEventDrawingType: return new STEventTypeLayout (type, 10);
  case PajeStateDrawingType: return new STStateTypeLayout (type, 2);
  case PajeLinkDrawingType: return new STLinkTypeLayout (type);
  case PajeVariableDrawingType: return new STVariableTypeLayout (type);
  case PajeContainerDrawingType: return new STContainerTypeLayout (type);
  }
  return NULL;
}

STEventTypeLayout::STEventTypeLayout (PajeType *type, double width)
  : STTypeLayout (type)
{
  this->width = width;
}

STStateTypeLayout::STStateTypeLayout (PajeType *type, double inset)
  : STTypeLayout (type)
{
  this->inset = inset;
}

STLinkTypeLayout::STLinkTypeLayout (PajeType *type)
  : STTypeLayout (type)
{
}

STVariableTypeLayout::STVariableTypeLayout (PajeType *type)
  : STTypeLayout (type)
{
}

STContainerTypeLayout::STContainerTypeLayout (PajeType *type)
  : STTypeLayout (type)
{
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

void STContainerTypeLayout::setRectOfContainer (PajeContainer *container, QRectF *rect)
{
  rectsOfContainers[container] = rect;
}

QRectF *STContainerTypeLayout::rectOfContainer (PajeContainer *container)
{
  if (rectsOfContainers.count(container)){
    return rectsOfContainers[container];
  }else{
    throw "You found a bug, congrats. The rect of container named "+container->name()+" was not calculated.";
  }
}

void STContainerTypeLayout::setVerticalOffsets (void)
{
  _supEventsOffset = 0;
  //for superior events (TODO, FIXME)
  //for states
  {
    _infEventsOffset = _supEventsOffset;
    std::vector<STStateTypeLayout*>::iterator sit;
    for (sit = stateSubtypes.begin(); sit != stateSubtypes.end(); sit++){
      STStateTypeLayout *stateTypeLayout = *sit;
      if (_infEventsOffset != _supEventsOffset){
        _infEventsOffset += subtypeSeparation ();
      }
      stateTypeLayout->setOffset (_infEventsOffset);
      _infEventsOffset += stateTypeLayout->height ();
    }
  }

  //for inferior events (TODO, FIXME)
  //for variables (TODO, FIXME)

  _subcontainersOffset = _infEventsOffset;

  //recurse
  std::vector<STContainerTypeLayout*>::iterator cit;
  for (cit = containerSubtypes.begin(); cit != containerSubtypes.end(); cit++){
    STContainerTypeLayout *containerTypeLayout = *cit;
    containerTypeLayout->setVerticalOffsets ();
  }
}
