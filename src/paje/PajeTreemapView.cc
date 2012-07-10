#include "PajeTreemapView.h"

PajeTreemapView::PajeTreemapView (PajeTreemapFrame *frame, QWidget *parent)
  : QGraphicsView(parent)
{
  this->frame = frame;
  setScene (&scene);
  treemap = NULL;
}

void PajeTreemapView::hierarchyChanged (void)
{
  if (treemap){
    delete treemap;
  }
  treemap = new PajeTreemap (NULL, rootInstance(), this);
}

void PajeTreemapView::timeSelectionChanged (void)
{
  if (!treemap){
    hierarchyChanged (); //to create treemap hierarchical data structure
  }

  treemap->recursiveTimeSelectionChanged ();
  treemap->recursiveSetTreemapValue ();

  this->repopulate ();
}

void PajeTreemapView::drawTreemap (PajeTreemap *treemap, PajeTreemapItem *parent)
{
  PajeTreemapItem *item = new PajeTreemapItem (treemap, parent, this);
  if (!parent){
    scene.addItem (item);
  }

  std::vector<PajeTreemap*>::iterator it;
  for (it = treemap->children.begin(); it != treemap->children.end(); it++){
    PajeTreemap *child = *it;
    drawTreemap (child, item);
  }
}

void PajeTreemapView::repopulate (void)
{
  if (!treemap){
    timeSelectionChanged (); //to create something to populate
  }

  QSize s = viewport()->size();
  QRectF bb = QRectF(QPointF(0,0), s);
  treemap->setBB (bb);
  treemap->recursiveCalculateTreemapWithFactor ((bb.width() * bb.height())/treemap->treemapValue);

  //clear
  scene.clear();

  scene.setSceneRect (treemap->rect());
  this->drawTreemap (treemap, NULL);

}

void PajeTreemapView::wheelEvent (QWheelEvent *event)
{
  QGraphicsView::wheelEvent(event);
}

void PajeTreemapView::mouseMoveEvent (QMouseEvent *event)
{
  QGraphicsView::mouseMoveEvent(event);
}

void PajeTreemapView::showEvent (QShowEvent *event)
{
  repopulate();
}

void PajeTreemapView::resizeEvent (QResizeEvent *event)
{
  repopulate();
}
