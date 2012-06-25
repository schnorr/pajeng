#ifndef __VIVA_GRAPH_VIEW_H
#define __VIVA_GRAPH_VIEW_H
#include <iostream>
#include <QtGui>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QGLWidget>
#include <QWheelEvent>
#include <GL/glu.h>
#include "VivaGraph.h"
#include "VivaRenderingThread.h"

class VivaRenderingThread;
class VivaGraph;

class VivaGraphView : public QGLWidget
{
  Q_OBJECT;

public slots:
  void initRendering (void);
  void finishRendering (void);

public:
  VivaGraphView (VivaGraph *vivagraph, QWidget *parent = NULL);

  void lockGLContext (void);
  void unlockGLContext (void);

  QWaitCondition *renderCondition (void);
  QMutex *renderMutex (void);

public slots:
  void render (void);

protected:
  void mousePressEvent (QMouseEvent *event);
  void mouseMoveEvent (QMouseEvent *event);
  void wheelEvent (QWheelEvent *event);
  void keyPressEvent (QKeyEvent *event);
  void closeEvent (QCloseEvent *event);
  void paintEvent (QPaintEvent *event);
  void resizeEvent (QResizeEvent *event);

private:
  VivaRenderingThread *thread;
  QMutex *render_mutex;
  QWaitCondition *render_condition;
  
public:
  double ratio;
  QPointF translate;
  VivaGraph *vivagraph;

protected:
  QPoint mousePosition;
  QPointF convertPoint (QPoint point);


signals:
  void mouseOverPoint (QPointF point);
  void leftMouseClickedOverPoint (QPointF point);
  void rightMouseClickedOverPoint (QPointF point);
};
#endif
