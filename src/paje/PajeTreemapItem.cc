#include "PajeTreemapItem.h"
#include "PajeWindow.h"

PajeTreemapItem::PajeTreemapItem (PajeTreemap *treemap, QGraphicsItem *parent, PajeTreemapView *filter)
  : QGraphicsItem (parent)
{
  this->treemap = treemap;
  this->filter = filter;
  setAcceptHoverEvents (true);
}

QRectF PajeTreemapItem::boundingRect (void) const
{
  return treemap->rect();
}

void PajeTreemapItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
  // painter->save();

  // PajeColor *color = filter->colorForValueOfEntityType (entity->value(), entity->type());
  // QColor c;
  // if (color){
  //   c = QColor (color->r*255, color->g*255, color->b*255, color->a*255);
  // }else{
  //   c = Qt::white;
  // }
  QRectF rect = boundingRect();
  painter->fillRect(rect, QBrush(Qt::white));
  painter->drawRect(rect);
//  painter->drawText (QPointF(0,0), QString::fromStdString (treemap->container->name()));

//   double height = rect.height();
//   double width = rect.width();
//   QPointF aux = QPointF(0,height*.01);
// //  painter->drawLine (rect.topLeft() + aux, rect.bottomLeft() - aux);
//   if (hover){
//     QColor c_negative = c;
//     c_negative.setHsv ((c.hue() + 200)%400, c.saturation(), c.value());
//     painter->fillRect (rect, QBrush(c_negative));
//     painter->drawRect (rect);

//     double size = width;
//     if (height < size) size = height;
//     size /= 2;
//     QPolygonF pol;
//     pol << rect.topLeft()
//         << rect.topLeft() + QPointF(size, 0)
//         << rect.topLeft() + QPointF(0, size)
//         << rect.topLeft();
//     painter->setBrush (QBrush(Qt::black));
//     painter->drawPolygon (pol);

//     QPolygonF pol2;
//     pol2 << rect.bottomRight()
//          << rect.bottomRight() - QPointF(0, size)
//          << rect.bottomRight() - QPointF(size, 0)
//          << rect.bottomRight();
//     painter->drawPolygon (pol2);

//   }
//   if (hover){
//     //FIXME, TODO: this code is not working
//     painter->setFont (QFont("Times", 5));
//     painter->setPen (QPen(Qt::black));
//     painter->drawText(rect, Qt::AlignLeft, QString::fromStdString (entity->value()));
//   }
//   painter->restore();
}

void PajeTreemapItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  hover = true;
  update();
  QString description = QString::fromStdString(treemap->container->description());
  PajeWindow::getInstance()->statusBar()->showMessage (description);
  setToolTip (description);

  std::cout << treemap->container->description() << std::endl;
  std::map<PajeAggregatedType*,double>::iterator it;
  for (it = treemap->values.begin(); it != treemap->values.end(); it++){
    std::cout << (*it).first << " = " << (*it).second << std::endl;
  }
}

void PajeTreemapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
  hover = false;
  update();
  PajeWindow::getInstance()->statusBar()->clearMessage ();
  setToolTip (NULL);
}

