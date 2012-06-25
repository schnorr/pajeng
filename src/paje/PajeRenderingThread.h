#ifndef __PAJE_RENDERING_THREAD_H
#define __PAJE_RENDERING_THREAD_H
#include <QGLWidget>
#include <QThread>
#include "PajeSpaceTimeView.h"

class PajeSpaceTimeView;

class PajeRenderingThread : public QThread
{
  Q_OBJECT;

public:
  PajeRenderingThread(PajeSpaceTimeView *widget);
  void run();
  void stop();
  void resizeViewport(const QSize &size);

protected:
  void initializeGL();
  void resizeGL(int width, int height);

private:
  volatile bool render_flag;
  volatile bool resize_flag;
  QSize viewport_size;
  PajeSpaceTimeView *st;
};
#endif
