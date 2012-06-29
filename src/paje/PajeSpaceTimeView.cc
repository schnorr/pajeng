#include "PajeSpaceTimeView.h"

PajeSpaceTimeView::PajeSpaceTimeView (QWidget *parent) : QGraphicsView(parent)
{
  setScene (&scene);
  setViewport (new QGLWidget (this));
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
  PajeContainerItem *item = new PajeContainerItem (layout, container, parent, this);
  if (!parent){
    scene.addItem (item);
  }

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
        PajeStateItem *item = new PajeStateItem (sublayout, entity, parent, this);
      }
    }
  }
}

void PajeSpaceTimeView::hierarchyChanged (void)
{
  //set an initial X ratio
  double traceDuration = endTime() - startTime();
  xratio = width()/traceDuration;
  yratio = 1;

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

  fitInView (rect);
  //change scale
//  scale (xratio, yratio);
}

void PajeSpaceTimeView::mouseMoveEvent (QMouseEvent *event)
{
  QPointF p = mapToScene (event->pos());
  QGraphicsView::mouseMoveEvent (event);
}

void PajeSpaceTimeView::wheelEvent (QWheelEvent *event)
{
  //save scene position
  QPointF p = mapToScene (event->pos());

  setTransform (QTransform());
  double r;
  if (event->modifiers() & Qt::ControlModifier){
    r = yratio;
  }else{
    r = xratio;
  }

  double factor = r * 0.1;
  if (event->delta() > 0){
    r -= factor;
  }else{
    r += factor;
  }

  if (event->modifiers() & Qt::ControlModifier){
    yratio = r;
  }else{
    xratio = r;
  }

  scale (xratio, yratio);
  centerOn (p);

  event->accept();
}
