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
#include "PajeGraphicsItem.h"
#include "PajeWindow.h"

PajeGraphicsItem::PajeGraphicsItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter)
  : QGraphicsItem (parent)
{
  this->layout = layout;
  this->entity = entity;
  this->filter = filter;
  hover = false;
}

PajeContainerItem::PajeContainerItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter)
  : PajeGraphicsItem (layout, entity, parent, filter)
{
  setZValue (100);
}


QRectF PajeContainerItem::boundingRect (void) const
{
  STContainerTypeLayout *containerLayout = dynamic_cast<STContainerTypeLayout*>(layout);
  PajeContainer *container = dynamic_cast<PajeContainer*>(entity);
  QPointF pos = containerLayout->layoutPositionForContainer (container);
  double height = containerLayout->layoutHeightForContainer (container);

  QRectF ret;
  if (height != 0){
    ret.setTop (pos.y());
    ret.setLeft (container->startTime());
    ret.setRight (container->endTime());
    ret.setBottom (pos.y() + height);
  }
  return ret;
}

void PajeContainerItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
  if (!layout->isContainer()){
    throw "You found a bug, congrats. Calling paint on a container with a layout which is not a container layout";
  }

  QRectF rect = boundingRect ();
  if (rect != QRectF()){
    painter->setPen (Qt::lightGray);
    painter->drawLine (rect.bottomLeft(), rect.bottomRight());
    painter->setPen (Qt::black);
    painter->setFont(QFont("Arial", 2));
  }
}

PajeStateItem::PajeStateItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter)
  : PajeGraphicsItem (layout, entity, parent, filter)
{
  setAcceptHoverEvents (true);
  setZValue (entity->imbricationLevel());
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

  PajeColor *color = filter->colorForValueOfEntityType (entity->type(), entity->value());
  QColor c;
  if (color){
    c = QColor (color->r*255, color->g*255, color->b*255, color->a*255);
  }else{
    c = Qt::white;
  }
  QRectF rect = boundingRect();
  painter->fillRect(rect, QBrush(c));
  double height = rect.height();
  double width = rect.width();
  QPointF aux = QPointF(0,height*.01);
  painter->drawLine (rect.topLeft() + aux, rect.bottomLeft() - aux);
  if (hover){
    painter->drawRect (rect);

    double size = width;
    if (height < size) size = height;
    size /= 2;
    QPolygonF pol;
    pol << rect.topLeft()
        << rect.topLeft() + QPointF(size, 0)
        << rect.topLeft() + QPointF(0, size)
        << rect.topLeft();
    painter->setBrush (QBrush(Qt::black));
    painter->drawPolygon (pol);

    QPolygonF pol2;
    pol2 << rect.bottomRight()
         << rect.bottomRight() - QPointF(0, size)
         << rect.bottomRight() - QPointF(size, 0)
         << rect.bottomRight();
    painter->drawPolygon (pol2);

  }
  if (hover){
    //FIXME, TODO: this code is not working
    painter->setFont (QFont("Times", 5));
    painter->setPen (QPen(Qt::black));
    painter->drawText(rect, Qt::AlignLeft, QString::fromStdString (entity->value()->name()));
  }
  painter->restore();
}

void PajeStateItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  hover = true;
  update();
  QString description = QString::fromStdString(entity->description());
  PajeWindow::getInstance()->statusBar()->showMessage (description);
  setToolTip (description);
}

void PajeStateItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  hover = false;
  update();
  PajeWindow::getInstance()->statusBar()->clearMessage ();
  setToolTip (NULL);
}
