#include <Qt>
#include <QtGui>
#include "PajeWindow.h"
#include "PajeComponent.h"

class PajeApplication : public QApplication
{
  Q_OBJECT;
  
private:
  PajeWindow *pajeWindow;
  QString filename;
  void connectComponents (PajeComponent *c1, PajeComponent *c2);

public:
  PajeApplication ( int & argc, char ** argv );
  void init (void);
};
