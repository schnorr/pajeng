#ifndef __VIVA_RENDERING_THREAD_H
#define __VIVA_RENDERING_THREAD_H
#include <QGLWidget>
#include <QThread>
#include "VivaGraphView.h"

class VivaGraphView;

class VivaRenderingThread : public QThread
{
  Q_OBJECT;

public:
  VivaRenderingThread(VivaGraphView *glWidget);
  void run();
  void stop();
  void resizeViewport(const QSize &size);

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

private:
  void draw (void);

  volatile bool render_flag;
  volatile bool resize_flag;
  QSize viewport_size;
  VivaGraphView *glw;
};
#endif
