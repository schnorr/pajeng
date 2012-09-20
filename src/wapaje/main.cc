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
// #include <QApplication>
// #include <QGraphicsEllipseItem>
// #include <QGraphicsScene>
// #include <QGraphicsView>
// #include <QGLWidget>

// int main( int argc, char **argv )
// {
//   QApplication app(argc, argv);

//   QGraphicsScene scene;
//   scene.setSceneRect( -100.0, -100.0, 200.0, 200.0 );

//   QGraphicsEllipseItem *item = new QGraphicsEllipseItem( 0, &scene );
//   item->setRect( -50.0, -50.0, 100.0, 100.0 );

//   QGraphicsView view( &scene );
//   view.setViewport(new QGLWidget ());
//   view.setRenderHints( QPainter::Antialiasing );
//   view.show();

//   return app.exec();
// }


#include "WapajeApplication.h"

int main(int argc, char **argv)
{
  QApplication::setAttribute( Qt::AA_X11InitThreads );
  WapajeApplication app(argc, argv);
  try {
    app.init();
  }catch (std::string exception){
    std::cout << "Exception: " << exception << std::endl;
    return 1;
  }
  return app.exec();
}

