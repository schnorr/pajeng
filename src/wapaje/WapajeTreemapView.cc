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
#include "WapajeTreemapView.h"

WapajeTreemapView::WapajeTreemapView (WapajeTreemapFrame *frame, QWidget *parent)
  : QGraphicsView(parent)
{
  this->frame = frame;
  setScene (&scene);
  treemap = NULL;
}

void WapajeTreemapView::recreate (void)
{
  if (treemap){
    delete treemap;
  }
  treemap = new WapajeTreemapNode (NULL, this, rootInstance());
}

void WapajeTreemapView::hierarchyChanged (void)
{
  this->recreate();
}

void WapajeTreemapView::timeSelectionChanged (void)
{
  if (!treemap){
    this->recreate(); //to create treemap hierarchical data structure
  }

  treemap->timeSelectionChanged();

  this->repopulate ();
}

void WapajeTreemapView::drawTreemap (WapajeTreemap *t, WapajeTreemapContainerItem *parent)
{

  // WapajeTreemapContainerItem *item = new WapajeTreemapContainerItem (treemap, parent, this);
  // if (!parent){
  //   scene.addItem (item);
  // }

  //aggregated children
  std::vector<WapajeTreemap*> valueChildren = t->valueChildren();
  std::vector<WapajeTreemap*>::iterator it;
  for (it = valueChildren.begin(); it != valueChildren.end(); it++){
    WapajeTreemapValueItem *vitem = new WapajeTreemapValueItem (*it, NULL, this);
    scene.addItem (vitem);
  }

  //normal children
  std::vector<WapajeTreemap*> children = t->children();
  for (it = children.begin(); it != children.end(); it++){
    WapajeTreemap *child = *it;
    this->drawTreemap (child, NULL);
  }
}

void WapajeTreemapView::repopulate (void)
{
  if (!treemap){
    this->recreate (); //to create something to populate
  }

  QSize s = viewport()->size();
  QRectF bb = QRectF(QPointF(0,0), s);
  treemap->setRect (bb);
  treemap->calculateTreemapWithFactor ((bb.width() * bb.height())/treemap->treemapValue());

  //clear
  scene.clear();

  scene.setSceneRect (treemap->rect());
  this->drawTreemap (treemap, NULL);
}

void WapajeTreemapView::wheelEvent (QWheelEvent *event)
{
  QGraphicsView::wheelEvent(event);
}

void WapajeTreemapView::mouseMoveEvent (QMouseEvent *event)
{
  QGraphicsView::mouseMoveEvent(event);
}

void WapajeTreemapView::showEvent (QShowEvent *event)
{
  repopulate();
}

void WapajeTreemapView::resizeEvent (QResizeEvent *event)
{
  repopulate();
}
