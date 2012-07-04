#include "PajeGraphicsItem.h"
#include "PajeWindow.h"

PajeGraphicsItem::PajeGraphicsItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter)
  : QGraphicsItem (parent)
{
  this->layout = layout;
  this->entity = entity;
  this->filter = filter;
}

PajeContainerItem::PajeContainerItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter)
  : PajeGraphicsItem (layout, entity, parent, filter)
{
//  setOpacity (0.9);
}


QRectF PajeContainerItem::boundingRect (void) const
{
  STContainerTypeLayout *containerLayout = dynamic_cast<STContainerTypeLayout*>(layout);
  PajeContainer *container = dynamic_cast<PajeContainer*>(entity);
  QPointF pos = containerLayout->layoutPositionForContainer (container);
  double height = containerLayout->layoutHeightForContainer (container);

  // std::cout << __FUNCTION__ << ": " << container->name() << " " << pos.x() << "," << pos.y() << " - h: "<<height<<std::endl;

  QRectF ret;
  ret.setTop (pos.y());
  ret.setLeft (container->startTime());
  ret.setRight (container->endTime());
  ret.setBottom (pos.y() + height);
  return ret;
}

void PajeContainerItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
  if (!layout->isContainer()){
    throw "You found a bug, congrats. Calling paint on a container with a layout which is not a container layout";
  }

  QRectF rect = boundingRect ();
  painter->setPen (Qt::lightGray);
  painter->drawRect(rect);
  // painter->drawText(rect.center(), QString (entity->name().c_str()));

 //  painter->fillRect(*rect, QColor(1,1,1));
//   painter->drawRect(*rect);
//   painter->drawText(rect->center(), QString (entity->name().c_str()));
}

PajeStateItem::PajeStateItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter)
  : PajeGraphicsItem (layout, entity, parent, filter)
{
//  setOpacity (1);
  setAcceptHoverEvents (true);
  hover = false;
}

QRectF PajeStateItem::boundingRect (void) const
{
  PajeContainer *container = entity->container();
  int imbric = entity->imbricationLevel();
  QPointF pos = layout->layoutPositionForContainer (container);
  double height = layout->layoutHeightForContainer (container);


  QRectF ret;
  ret.setTop (pos.y());
  ret.setLeft (entity->startTime());
  ret.setRight (entity->endTime());
  ret.setBottom (pos.y() + height - layout->inset()*imbric);
  return ret;
}

void PajeStateItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
  painter->save();

  PajeColor *color = filter->colorForValueOfEntityType (entity->value(), entity->type());
  QColor c;
  if (color){
    c = QColor (color->r*255, color->g*255, color->b*255, color->a*255);
  }else{
    c = Qt::white;
  }
  QRectF rect = boundingRect();
  painter->fillRect(rect, QBrush(c));
  painter->drawRect(rect);
  if (hover){
    //FIXME, TODO: this code is not working
    painter->setFont (QFont("Times", 5));
    painter->setPen (QPen(Qt::black));
    painter->drawText(rect, Qt::AlignLeft, QString::fromStdString (entity->value()));
  }
  painter->restore();
}

void PajeStateItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  hover = true;
  update();
  PajeWindow::getInstance()->statusBar()->showMessage (QString::fromStdString(entity->description()));
}

void PajeStateItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  hover = false;
  update();
  PajeWindow::getInstance()->statusBar()->clearMessage ();
}
