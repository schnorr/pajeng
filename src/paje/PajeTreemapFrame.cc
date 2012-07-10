#include "PajeTreemapFrame.h"
#include <QtGui>
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif
#include <qmath.h>

PajeTreemapFrame::PajeTreemapFrame (QWidget *parent)
  : QFrame(parent)
{
  // setFrameStyle(Sunken | StyledPanel);
  graphicsView = new PajeTreemapView (this, this);
  graphicsView->setRenderHint(QPainter::Antialiasing, false);
  graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
  graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
  graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  QBoxLayout *topLayout = new QBoxLayout (QBoxLayout::TopToBottom, this);
  topLayout->addWidget (graphicsView);

  setLayout (topLayout);
}

PajeTreemapView *PajeTreemapFrame::view () const
{
  return graphicsView;
}

void PajeTreemapFrame::showEvent ( QShowEvent * event )
{
}

void PajeTreemapFrame::resizeEvent ( QResizeEvent * event )
{
}
