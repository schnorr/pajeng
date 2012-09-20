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
#include "WapajeTreemapFrame.h"
#include <QtGui>
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif
#include <qmath.h>

WapajeTreemapFrame::WapajeTreemapFrame (QWidget *parent)
  : QFrame(parent)
{
  // setFrameStyle(Sunken | StyledPanel);
  graphicsView = new WapajeTreemapView (this, this);
  graphicsView->setRenderHint(QPainter::Antialiasing, false);
  graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
  graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
  graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  QBoxLayout *topLayout = new QBoxLayout (QBoxLayout::TopToBottom, this);
  topLayout->addWidget (graphicsView);

  setLayout (topLayout);
}

WapajeTreemapView *WapajeTreemapFrame::view () const
{
  return graphicsView;
}

void WapajeTreemapFrame::showEvent ( QShowEvent * event )
{
}

void WapajeTreemapFrame::resizeEvent ( QResizeEvent * event )
{
}
