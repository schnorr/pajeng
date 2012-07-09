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
