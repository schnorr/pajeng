/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeRenderingThread.h"
#include <QTime>

PajeRenderingThread::PajeRenderingThread(PajeSpaceTimeView *widget)
  : QThread()
{
  render_flag = true;
  resize_flag = false;
  viewport_size = widget->size();
  st = widget;
}

void PajeRenderingThread::run()
{
  st->lockGLContext ();
  initializeGL ();
  while (render_flag){
    if (resize_flag){
      resizeGL (viewport_size.width(), viewport_size.height());
      resize_flag = false;
    }
    st->draw ();
    st->swapBuffers();
    st->doneCurrent();
    st->renderCondition()->wait(st->renderMutex());
    st->makeCurrent();
  }
  st->unlockGLContext ();
  render_flag = true;
}

void PajeRenderingThread::stop()
{
  render_flag = false;
}

void PajeRenderingThread::resizeViewport( const QSize& _size )
{
  viewport_size = _size;
  resize_flag = true;
}

void PajeRenderingThread::initializeGL (void)
{
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_BLEND);
  glEnable(GL_POLYGON_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0, 0, 0, 0);
}

void PajeRenderingThread::resizeGL (int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h); // set origin to bottom left corner
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
