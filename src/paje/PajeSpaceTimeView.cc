#include "PajeSpaceTimeView.h"

PajeSpaceTimeView::PajeSpaceTimeView (QWidget *parent) : QGraphicsView(parent)
{
  scene.setSceneRect( -100.0, -100.0, 200.0, 2000.0 );
  setScene (&scene);
  //setViewport (new QGLWidget (this));
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
  STTypeLayout *typeLayout = STTypeLayout::Create (type);
  if (containerLayout){
    containerLayout->addSubtype (typeLayout);
  }

  //associate the type to its layout
  layoutDescriptors[type] = typeLayout;

  //recurse if possible
  if (isContainerType (type)){
    std::vector<PajeType*> containedTypes = containedTypesForContainerType (type);
    std::vector<PajeType*>::iterator it;
    for (it = containedTypes.begin(); it != containedTypes.end(); it++){
      createTypeLayout (*it, dynamic_cast<STContainerTypeLayout*>(typeLayout));
    }
  }
  return typeLayout;
}

QRectF *PajeSpaceTimeView::calcRectOfContainer (PajeContainer *container, STContainerTypeLayout *layout, double minY)
{
  QRectF *rect = new QRectF();
  rect->setLeft (container->startTime());
  rect->setRight (container->endTime());

  //define the space occupied by every sub type except containers
  std::vector<STTypeLayout*> sublayouts = layout->subtypes();
  std::vector<STTypeLayout*>::iterator it;
  for (it = sublayouts.begin(); it != sublayouts.end(); it++){
    STTypeLayout *sublayout = *it;
    if (sublayout->isContainer()) continue;
    rect->setTop (minY + sublayout->offset());
    rect->setHeight (sublayout->height());

    sublayout->setRectInContainer (rect, container);
  }

  double separation = 0;

  rect->setTop (minY);
  rect->setHeight (layout->subcontainersOffset());

  for (it = sublayouts.begin(); it != sublayouts.end(); it++){
    STTypeLayout *sublayout = *it;
    if (!sublayout->isContainer()) continue;

    STContainerTypeLayout *containerSublayout = dynamic_cast<STContainerTypeLayout*>(sublayout);
    double subtypeOffset = rect->bottom() + separation;
    QRectF *r = calcRectOfAllInstances (container, containerSublayout, subtypeOffset);
    if (!r->isNull()){
      rect = new QRectF(rect->united (*r));
      separation = layout->subtypeSeparation();
    }
  }
  layout->setRectOfContainer (container, rect);
  std::cout << __FUNCTION__ << " - " << container->name() << " = [" << 
    rect->x() << ", " <<
    rect->y() << "] [" <<
    rect->width() << ", " <<
    rect->height () << "]"<<std::endl;
  return rect;
}

QRectF *PajeSpaceTimeView::calcRectOfAllInstances (PajeContainer *container, STContainerTypeLayout *layout, double minY)
{
  double separation = 0;
  QRectF *rect = new QRectF();
  rect->setLeft (container->startTime());
  rect->setRight (container->endTime());
  rect->setTop (minY);
  rect->setHeight (0);

  // check all instances on this hierarchy
  std::vector<PajeContainer*> containers;
  std::vector<PajeContainer*>::iterator it;
  containers = enumeratorOfContainersTypedInContainer (layout->type(), container);
  for (it = containers.begin(); it != containers.end(); it++){
    PajeContainer *child = *it;
    
    QRectF *r = calcRectOfContainer (child, layout, rect->bottom() + separation);
    if (!r->isNull()){
      if (rect->isNull()){
        rect = r;
        separation = layout->siblingSeparation();
      }else{
        rect = new QRectF(rect->united (*r));
      }
    }
  }
  layout->setRectInContainer (rect, container);
  return rect;
}


void PajeSpaceTimeView::renewLayoutDescriptors (void)
{
  PajeContainer *container = rootInstance ();
  PajeType *type = container->type();

  layoutDescriptors.clear();
  STContainerTypeLayout *layout = dynamic_cast<STContainerTypeLayout*>(createTypeLayout (type, NULL));
  layout->setVerticalOffsets ();
  calcRectOfContainer (container, layout, 0);
}

void PajeSpaceTimeView::hierarchyChanged (void)
{
  std::cout << __FUNCTION__ << std::endl;
  std::cout << startTime() << " - " << endTime() << std::endl;

  renewLayoutDescriptors ();
  STContainerTypeLayout *layout = dynamic_cast<STContainerTypeLayout*>(layoutDescriptorForType (rootInstance()->type()));
  QRectF *rect = layout->rectOfContainer (rootInstance());
  scene.setSceneRect (*rect);
  scene.addRect (*rect);

  //change X scale
  scale (50000, 1);
}
