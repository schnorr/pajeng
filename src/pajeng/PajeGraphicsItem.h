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
#ifndef __PAJE_GRAPHICS_ITEM_H
#define __PAJE_GRAPHICS_ITEM_H
#include <QPainter>
#include <QGraphicsItem>
#include "STEntityTypeLayout.h"
#include "PajeEntity.h"
#include "PajeSpaceTimeView.h"

class PajeSpaceTimeView;

class PajeGraphicsItem : public QGraphicsItem
{
protected:
  bool hover;
  STTypeLayout *layout;
  PajeEntity *entity;
  PajeSpaceTimeView *filter;

public:
  PajeGraphicsItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter);
};

class PajeContainerItem : public PajeGraphicsItem
{
public:
  PajeContainerItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter);
  QRectF boundingRect (void) const;
  void paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget);

};


class PajeStateItem : public PajeGraphicsItem
{
public:
  PajeStateItem (STTypeLayout *layout, PajeEntity *entity, QGraphicsItem *parent, PajeSpaceTimeView *filter);
  QRectF boundingRect (void) const;
  void paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget);
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif
