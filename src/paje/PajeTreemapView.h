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
class PajeTreemapItem;
class PajeTreemap;

class PajeTreemapView : public QGraphicsView, public PajeComponent
{
  Q_OBJECT;

private:
  QGraphicsScene scene;
  PajeTreemapFrame *frame;
  PajeTreemap *treemap;

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
  void drawTreemap (PajeTreemap *treemap, PajeTreemapItem *parent);
  void repopulate (void);
};

#endif
