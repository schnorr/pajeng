#include "VivaRenderingThread.h"
#include <QTime>

VivaRenderingThread::VivaRenderingThread(VivaGraphView *gl) 
  : QThread(), glw(gl)
{
  render_flag = true;
  resize_flag = false;
  viewport_size = gl->size();
  glw = gl;
}

void VivaRenderingThread::run()
{
  glw->lockGLContext ();
  initializeGL ();
  while (render_flag){
    if (resize_flag){
      resizeGL (viewport_size.width(), viewport_size.height());
      resize_flag = false;
    }
    paintGL ();
    glw->swapBuffers();
    glw->doneCurrent();
    glw->renderCondition()->wait(glw->renderMutex());
    glw->makeCurrent();
  }
  glw->unlockGLContext ();
  render_flag = true;
}

void VivaRenderingThread::stop()
{
  render_flag = false;
}

void VivaRenderingThread::resizeViewport( const QSize& _size )
{
  viewport_size = _size;
  resize_flag = true;
}

void VivaRenderingThread::initializeGL (void)
{
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_BLEND);
  glEnable(GL_POLYGON_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0, 0, 0, 0);
}

void VivaRenderingThread::resizeGL (int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void VivaRenderingThread::paintGL (void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1, 1, 1, 0);
  draw();
}

void VivaRenderingThread::draw (void)
{  
  glLoadIdentity ();
  glTranslatef (glw->translate.x(), -glw->translate.y(), 0);
  glScalef (glw->ratio, glw->ratio, 1);
  glw->vivagraph->draw();
}

