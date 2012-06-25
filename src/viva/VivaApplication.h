#include <iostream>
#include <QtGui>
#include "VivaWindow.h"
#include "PajeComponent.h"

class VivaApplication : public QApplication
{
  Q_OBJECT

private:
  QString filename;
  QString configuration;
  void connectComponents (PajeComponent *c1, PajeComponent *c2);

public:
  VivaApplication ( int & argc, char ** argv );
  void init (void);
};
