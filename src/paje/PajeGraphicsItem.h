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
