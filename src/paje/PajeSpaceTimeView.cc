#include "PajeSpaceTimeView.h"

PajeSpaceTimeView::PajeSpaceTimeView (QWidget *parent) : QGLWidget(parent)
{
  setMouseTracking(true);
  setAutoBufferSwap(false);

  render_condition = new QWaitCondition;
  render_mutex = new QMutex;
  thread = new PajeRenderingThread (this);

  initRendering();
}

QWaitCondition *PajeSpaceTimeView::renderCondition (void)
{
  return render_condition;
}

QMutex *PajeSpaceTimeView::renderMutex (void)
{
  return render_mutex;
}

void PajeSpaceTimeView::initRendering ()
{
  thread->start ();
  renderCondition()->wakeAll();
}

void PajeSpaceTimeView::finishRendering()
{
  thread->stop();
  renderCondition()->wakeAll();
  thread->wait();
}

void PajeSpaceTimeView::lockGLContext (void)
{
  renderMutex()->lock();
  makeCurrent();
}

void PajeSpaceTimeView::unlockGLContext (void)
{
  doneCurrent();
  renderMutex()->unlock();
}

void PajeSpaceTimeView::render (void)
{
  renderCondition()->wakeAll();
}

void PajeSpaceTimeView::draw (void)
{
  //draw the thing, using gl functions
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1, 1, 1, 0);
  //...
  std::cout << __FUNCTION__ << std::endl;
}

void PajeSpaceTimeView::mousePressEvent (QMouseEvent *event)
{
}

void PajeSpaceTimeView::mouseMoveEvent (QMouseEvent *event)
{
  render();
}

void PajeSpaceTimeView::wheelEvent (QWheelEvent *event)
{
}

void PajeSpaceTimeView::keyPressEvent (QKeyEvent *event)
{
}

void PajeSpaceTimeView::closeEvent (QCloseEvent *event)
{
  finishRendering();
  QGLWidget::closeEvent(event);
}

void PajeSpaceTimeView::paintEvent (QPaintEvent *event)
{
  render();
}

void PajeSpaceTimeView::resizeEvent (QResizeEvent *event)
{
  thread->resizeViewport (event->size());
  render();
}
