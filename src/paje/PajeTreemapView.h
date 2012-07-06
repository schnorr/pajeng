#ifndef __PAJE_TREEMAP_VIEW_H_
#define __PAJE_TREEMAP_VIEW_H_
#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "PajeComponent.h"
#include "PajeTreemapFrame.h"

class PajeTreemapFrame;

class PajeTreemapView : public QGraphicsView, public PajeComponent
{
  Q_OBJECT;

private:
  QGraphicsScene scene;
  PajeTreemapFrame *frame;

public:
  PajeTreemapView (PajeTreemapFrame *frame, QWidget *parent = NULL);

protected: //from PajeComponent protocol
  void hierarchyChanged (void);

protected: //from QGraphicsView
  void wheelEvent (QWheelEvent *event);
};

#endif
