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
#include "PajeTreemapItem.h"
#include "PajeWindow.h"

PajeTreemapValueItem::PajeTreemapValueItem (PajeTreemap *treemap, QGraphicsItem *parent, PajeTreemapView *filter)
  : QGraphicsItem (parent)
{
  this->treemap = treemap;
  this->filter = filter;
  hover = false;
  setAcceptHoverEvents (true);
}

QRectF PajeTreemapValueItem::boundingRect (void) const
{
  return treemap->rect();
}

void PajeTreemapValueItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
  QRectF rect = boundingRect();
  PajeColor *color = treemap->type()->color();
  QColor c;
  if (color){
    c = QColor (color->r*255, color->g*255, color->b*255, color->a*255);
  }else{
    c = Qt::white;
  }
  painter->fillRect(rect, QBrush(c));
  if (hover){
    painter->drawRect(rect);
  }
}

void PajeTreemapValueItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  hover = true;
  update();
  QString description = QString::fromStdString(treemap->container->description());
  PajeWindow::getInstance()->statusBar()->showMessage (description);
  setToolTip (description);
}

void PajeTreemapValueItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  hover = false;
  update();
  PajeWindow::getInstance()->statusBar()->clearMessage ();
  setToolTip (NULL);
}
