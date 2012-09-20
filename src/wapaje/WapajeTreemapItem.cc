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
#include "WapajeTreemapItem.h"
#include "WapajeWindow.h"

WapajeTreemapValueItem::WapajeTreemapValueItem (WapajeTreemap *treemap, QGraphicsItem *parent, WapajeTreemapView *filter)
  : QGraphicsItem (parent)
{
  this->treemap = treemap;
  this->filter = filter;
  hover = false;
  setAcceptHoverEvents (true);
}

QRectF WapajeTreemapValueItem::boundingRect (void) const
{
  return treemap->rect();
}

void WapajeTreemapValueItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
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

void WapajeTreemapValueItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  hover = true;
  update();
  QString description = QString::fromStdString(treemap->container->description());
  WapajeWindow::getInstance()->statusBar()->showMessage (description);
  setToolTip (description);
}

void WapajeTreemapValueItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  hover = false;
  update();
  WapajeWindow::getInstance()->statusBar()->clearMessage ();
  setToolTip (NULL);
}
