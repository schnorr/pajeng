#ifndef PAJE_APPLICATION_H
#define PAJE_APPLICATION_H
#include <Qt>
#include <QtGui>
#include "PajeWindow.h"
#include "PajeComponent.h"
#include "../utils/PajeThreadReader.h"
#include "PajeFileReader.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"

class PajeWindow;

class PajeApplication : public QApplication
{
  Q_OBJECT;
  
private:
  PajeFileReader *reader;
  PajeEventDecoder *decoder;
  PajeSimulator *simulator;
  PajeWindow *pajeWindow;
  QString filename;
  void disconnectComponents (PajeComponent *c1, PajeComponent *c2);
  void connectComponents (PajeComponent *c1, PajeComponent *c2);

public:
  PajeApplication ( int & argc, char ** argv );
  void init (void);
  void disconnectTreemap (void);
  void disconnectSpacetime (void);
  void connectTreemap (void);
  void connectSpacetime (void);

};
#endif
