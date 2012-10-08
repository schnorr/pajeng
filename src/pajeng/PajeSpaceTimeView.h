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
#ifndef __PAJE_SPACE_TIME_VIEW_H_
#define __PAJE_SPACE_TIME_VIEW_H_
#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "PajeComponent.h"
#include "STEntityTypeLayout.h"
#include "PajeGraphicsItem.h"
#include "PajeSpaceTimeFrame.h"

class PajeGraphicsItem;
class PajeSpaceTimeFrame;

class PajeSpaceTimeView : public QGraphicsView, public PajeComponent
{
  Q_OBJECT;

private:
  QGraphicsScene scene;
  PajeSpaceTimeFrame *frame;

public:
  PajeSpaceTimeView (PajeSpaceTimeFrame *frame, QWidget *parent = NULL);

private:
  /* Handling with layout descriptors */
  std::map<PajeType*,STTypeLayout*> layoutDescriptors;
  STTypeLayout *layoutDescriptorForType (PajeType *type);
  STTypeLayout *createTypeLayout (PajeType *type, STContainerTypeLayout *containerLayout);
  void renewLayoutDescriptors (void);
  void drawContainer (STTypeLayout *layout, PajeContainer *container, PajeGraphicsItem *parent);

protected: //from PajeComponent protocol
  void hierarchyChanged (void);
  void timeSelectionChanged (void);

protected: //from QGraphicsView
  void wheelEvent (QWheelEvent *event);
  void mouseMoveEvent (QMouseEvent *event);
  void mousePressEvent (QMouseEvent *event);
  void mouseReleaseEvent (QMouseEvent *event);

private:
  double firstSelectionPoint;
  QGraphicsRectItem *selectionRect;
};

#endif
