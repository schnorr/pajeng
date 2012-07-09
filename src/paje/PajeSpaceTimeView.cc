#include "PajeSpaceTimeView.h"

PajeSpaceTimeView::PajeSpaceTimeView (PajeSpaceTimeFrame *frame, QWidget *parent) : QGraphicsView(parent)
{
  this->frame = frame;
  setScene (&scene);
}

STTypeLayout *PajeSpaceTimeView::layoutDescriptorForType (PajeType *type)
{
  if (layoutDescriptors.count (type)){
    return layoutDescriptors[type];
  }else{
    throw "You found a bug, congrats. The layout descriptor for type "+type->name+" was not created.";
  }
}

STTypeLayout *PajeSpaceTimeView::createTypeLayout (PajeType *type, STContainerTypeLayout *containerLayout)
{
  //create the type layout using STTypeLayout factory
  STTypeLayout *typeLayout = STTypeLayout::Create (type, containerLayout);

  //add to parent if parent exists
  if (containerLayout){
    containerLayout->addSubtype (typeLayout);
  }

  //associate the type to its layout
  layoutDescriptors[type] = typeLayout;

  //recurse
  if (isContainerType (type)){
    std::vector<PajeType*> containedTypes = containedTypesForContainerType (type);
    std::vector<PajeType*>::iterator it;
    for (it = containedTypes.begin(); it != containedTypes.end(); it++){
      createTypeLayout (*it, dynamic_cast<STContainerTypeLayout*>(typeLayout));
    }
  }
  return typeLayout;
}

void PajeSpaceTimeView::renewLayoutDescriptors (void)
{
  PajeContainer *container = rootInstance ();
  PajeType *type = container->type();

  layoutDescriptors.clear();

  STContainerTypeLayout *layout = dynamic_cast<STContainerTypeLayout*>(createTypeLayout (type, NULL));
  layout->recursiveSetLayoutPositions (container, this, QPointF());
}

void PajeSpaceTimeView::drawContainer (STTypeLayout *layout, PajeContainer *container, PajeGraphicsItem *parent)
{
  PajeContainerItem *item = new PajeContainerItem (layout, container, NULL, this);
  // if (!parent){
    scene.addItem (item);
  // }

  std::vector<STTypeLayout*> sublayouts = layout->subtypes();
  std::vector<STTypeLayout*>::iterator it;
  for (it = sublayouts.begin(); it != sublayouts.end(); it++){
    STTypeLayout *sublayout = *it;
    if (sublayout->isContainer()){
      //recurse
      std::vector<PajeContainer*> containers;
      std::vector<PajeContainer*>::iterator it;
      containers = enumeratorOfContainersTypedInContainer (sublayout->type(), container);
      for (it = containers.begin(); it != containers.end(); it++){
        PajeContainer *child = *it;
        drawContainer (sublayout, child, item);
      }
    }else{
      //sublayout is not a container, draw things
      std::vector<PajeEntity*> entities;
      std::vector<PajeEntity*>::iterator it;
      entities = enumeratorOfEntitiesTypedInContainer (sublayout->type(),
                                                       container,
                                                       container->startTime(),
                                                       container->endTime());
      for (it = entities.begin(); it != entities.end(); it++){
        PajeEntity *entity = *it;
        PajeStateItem *item = new PajeStateItem (sublayout, entity, NULL, this);
        scene.addItem (item);
      }
    }
  }
}

void PajeSpaceTimeView::hierarchyChanged (void)
{
  //clear the scene
  scene.clear ();

  //scene reconstruction
  renewLayoutDescriptors ();

  PajeContainer *root = rootInstance();
  PajeType *rootType = root->type();
  STContainerTypeLayout *rootLayout = dynamic_cast<STContainerTypeLayout*>(layoutDescriptorForType (rootType));

  double height = rootLayout->layoutHeightForContainer (root);
  double width = root->endTime() - root->startTime();

  QRectF rect = QRectF(0,0,width,height);
  scene.setSceneRect (rect);

  drawContainer (rootLayout, root, NULL);

  double time = (endTime() - startTime());
  double space = scene.sceneRect().height();
  frame->setSpaceTimeLimit (space, time);
}

void PajeSpaceTimeView::wheelEvent (QWheelEvent *event)
{
  if (event->modifiers() & Qt::ShiftModifier){
    if (event->delta() > 0){
      frame->zoomXIn (6);
    }else{
      frame->zoomXOut (6);
    }
    event->accept();
  }else if (event->modifiers() & Qt::ControlModifier){
    if (event->delta() > 0){
      frame->zoomIn(6);
    }else{
      frame->zoomOut(6);
    }
    event->accept();
  }else{
    QGraphicsView::wheelEvent(event);
  }
}

void PajeSpaceTimeView::mouseMoveEvent (QMouseEvent *event)
{
  QPointF sp = mapToScene (event->pos());
  if (sp.x() >= startTime() && sp.x() <= endTime()){
    frame->setCurrentTime (sp.x());
  }else{
    frame->setCurrentTime (-1);
  }
  QGraphicsView::mouseMoveEvent (event);
}
