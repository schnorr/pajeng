#ifndef __PAJE_SPACE_TIME_VIEW_H_
#define __PAJE_SPACE_TIME_VIEW_H_
#include <iostream>
#include <QtGui>
#include <QGLWidget>
#include <GL/glu.h>
#include "PajeComponent.h"
#include "PajeRenderingThread.h"

class PajeRenderingThread;

class PajeSpaceTimeView : public QGLWidget, public PajeComponent
{
  Q_OBJECT;
  friend class PajeRenderingThread;

public:
  PajeSpaceTimeView (QWidget *parent = NULL);

protected:
  void mousePressEvent (QMouseEvent *event);
  void mouseMoveEvent (QMouseEvent *event);
  void wheelEvent (QWheelEvent *event);
  void keyPressEvent (QKeyEvent *event);
  void closeEvent (QCloseEvent *event);
  void paintEvent (QPaintEvent *event);
  void resizeEvent (QResizeEvent *event);

protected: //methods to deal with the PajeRenderingThread
  void lockGLContext (void);
  void unlockGLContext (void);
  QWaitCondition *renderCondition (void);
  QMutex *renderMutex (void);

private:
  PajeRenderingThread *thread;
  QMutex *render_mutex;
  QWaitCondition *render_condition;
  void initRendering (void);
  void finishRendering (void);
  void render (void);
  void draw (void);

};

#endif
