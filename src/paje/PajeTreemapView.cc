#include "PajeTreemapView.h"

PajeTreemapView::PajeTreemapView (PajeTreemapFrame *frame, QWidget *parent)
  : QGraphicsView(parent)
{
  this->frame = frame;
  setScene (&scene);
}

void PajeTreemapView::hierarchyChanged (void)
{
}

void PajeTreemapView::wheelEvent (QWheelEvent *event)
{
}

