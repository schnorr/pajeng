#include "PajeTreemapFrame.h"
#include <QtGui>
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif
#include <qmath.h>

PajeTreemapFrame::PajeTreemapFrame (QWidget *parent)
  : QFrame(parent)
{
//  setFrameStyle(Sunken | StyledPanel);
  graphicsView = new PajeTreemapView (this);
  graphicsView->setRenderHint(QPainter::Antialiasing, false);
  graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
  graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
  graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

PajeTreemapView *PajeTreemapFrame::view () const
{
  return graphicsView;
}

