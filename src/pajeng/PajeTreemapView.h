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
#ifndef __PAJE_TREEMAP_VIEW_H_
#define __PAJE_TREEMAP_VIEW_H_
#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "PajeComponent.h"
#include "PajeTreemapFrame.h"
#include "PajeTreemap.h"
#include "PajeTreemapItem.h"

class PajeTreemapFrame;
class PajeTreemapContainerItem;
class PajeTreemap;
class PajeTreemapNode;

class PajeTreemapView : public QGraphicsView, public PajeComponent
{
  Q_OBJECT;

private:
  QGraphicsScene scene;
  PajeTreemapFrame *frame;
  PajeTreemapNode *treemap;

public:
  PajeTreemapView (PajeTreemapFrame *frame, QWidget *parent = NULL);

protected: //from PajeComponent protocol
  void hierarchyChanged (void);
  void timeSelectionChanged (void);

protected: //from QGraphicsView
  void wheelEvent (QWheelEvent *event);
  void mouseMoveEvent (QMouseEvent *event);
  void showEvent (QShowEvent *event);
  void resizeEvent (QResizeEvent *event);

protected: //from PajeTreemapView
  void drawTreemap (PajeTreemap *treemap, PajeTreemapContainerItem *parent);
  void recreate (void);
  void repopulate (void);
};

#endif
