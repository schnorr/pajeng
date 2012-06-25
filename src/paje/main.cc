#include <QtGui>
#include "PajeWindow.h"

int main(int argc, char **argv)
{
//Q_INIT_RESOURCE(application);

     QApplication app(argc, argv);
     app.setOrganizationName("Trolltech");
     app.setApplicationName("Application Example");
     PajeWindow mainWin;
 #if defined(Q_OS_SYMBIAN)
     mainWin.showMaximized();
 #else
     mainWin.show();
 #endif
     return app.exec();
}
