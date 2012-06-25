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


#include "PajeApplication.h"

int main(int argc, char **argv)
{
  QApplication::setAttribute( Qt::AA_X11InitThreads );
  PajeApplication app(argc, argv);
  try {
    app.init();
  }catch (std::string exception){
    std::cout << "Exception: " << exception << std::endl;
    return 1;
  }
  return app.exec();
}

