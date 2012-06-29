#ifndef __PAJE_SPACE_TIME_VIEW_H_
#define __PAJE_SPACE_TIME_VIEW_H_
#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "PajeComponent.h"
#include "STEntityTypeLayout.h"
#include "PajeGraphicsItem.h"

class PajeGraphicsItem;

class PajeSpaceTimeView : public QGraphicsView, public PajeComponent
{
  Q_OBJECT;

private:
  double xratio;
  double yratio;
  QGraphicsScene scene;

public:
  PajeSpaceTimeView (QWidget *parent = NULL);

private:
  /* Handling with layout descriptors */
  std::map<PajeType*,STTypeLayout*> layoutDescriptors;
  STTypeLayout *layoutDescriptorForType (PajeType *type);
  STTypeLayout *createTypeLayout (PajeType *type, STContainerTypeLayout *containerLayout);
  void renewLayoutDescriptors (void);
  void drawContainer (STTypeLayout *layout, PajeContainer *container, PajeGraphicsItem *parent);

protected: //from PajeComponent protocol
  void hierarchyChanged (void);

protected: //from QGraphicsView
  void mouseMoveEvent (QMouseEvent *event);
  void wheelEvent (QWheelEvent *event);
};

#endif
