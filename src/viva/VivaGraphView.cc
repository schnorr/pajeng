#include "VivaGraphView.h"
#include <QDateTime>

static bool valid = false;

VivaGraphView::VivaGraphView (VivaGraph *vivagraph, QWidget *parent) : QGLWidget(parent)
{
  this->vivagraph = vivagraph;
  ratio = 1;
  translate = QPointF(0.0, 0.0);
  mousePosition = QPoint (0, 0);

  setMouseTracking(true);
  setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
  setAutoBufferSwap(false);

  render_condition = new QWaitCondition;
  render_mutex = new QMutex;
  thread = new VivaRenderingThread (this);

  initRendering ();
}

QWaitCondition *VivaGraphView::renderCondition (void)
{
  return render_condition;
}

QMutex *VivaGraphView::renderMutex (void)
{
  return render_mutex;
}

void VivaGraphView::initRendering ()
{
  thread->start ();
  renderCondition()->wakeAll();
}


void VivaGraphView::finishRendering()
{
  thread->stop();
  renderCondition()->wakeAll();
  thread->wait();
}

void VivaGraphView::mousePressEvent(QMouseEvent *event)
{
  QPoint p = QPoint (event->x(), event->y());
  QPointF converted = convertPoint(p);

  if (event->buttons() & Qt::LeftButton){
    leftMouseClickedOverPoint (converted);
  }else if (event->buttons() & Qt::RightButton){
    rightMouseClickedOverPoint (converted);
  }
}

void VivaGraphView::mouseMoveEvent(QMouseEvent *event)
{
  QPoint p = QPoint (event->x(), event->y());

  if (event->buttons() & Qt::LeftButton){
    QPoint dif = p - mousePosition;
    translate = translate + QPointF(dif.x(), dif.y());
    render();
  }else{
    emit mouseOverPoint (convertPoint(p));
  }
  mousePosition = p;
}

void VivaGraphView::wheelEvent(QWheelEvent *event)
{
  double factor = ratio * 0.1;
  if (event->delta() > 0){
    ratio += factor;
  }else{
    ratio -= factor;
  }
  render();
}

void VivaGraphView::keyPressEvent(QKeyEvent* event)
{
}

void VivaGraphView::closeEvent (QCloseEvent *event)
{
  finishRendering();
  QGLWidget::closeEvent(event);
}

void VivaGraphView::resizeEvent (QResizeEvent *event)
{
  thread->resizeViewport (event->size());
  render();
}

void VivaGraphView::paintEvent (QPaintEvent *event)
{
  render();
}

QPointF VivaGraphView::convertPoint (QPoint point)
{
  //detect
  GLdouble   sx,sy,sz;
  GLdouble   depth;
  GLdouble pMat[4*4];
  GLdouble   mMat[4*4];
  GLint viewport[4]; // MinX MinY MaxX MaxY

  lockGLContext();
  glLoadIdentity();
  glTranslatef (translate.x(), -translate.y(), 0);
  glScalef (ratio, ratio, 1);
  glGetDoublev(GL_MODELVIEW_MATRIX, mMat);
  glGetDoublev(GL_PROJECTION_MATRIX, pMat);
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluUnProject((GLint)point.x(), viewport[3] - (GLint)point.y(),  // send the mouse coordinates
               0,
               mMat,pMat,viewport,
               &sx, &sy, &sz);
  unlockGLContext();
  return QPointF (sx, sy);
}


void VivaGraphView::lockGLContext (void)
{
  renderMutex()->lock();
  makeCurrent();
}

void VivaGraphView::unlockGLContext (void)
{
  doneCurrent();
  renderMutex()->unlock();
}

void VivaGraphView::render (void)
{
  renderCondition()->wakeAll();
}
