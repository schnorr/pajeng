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
