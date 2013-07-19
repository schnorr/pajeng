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
#include "PajeException.h"

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
  void disconnectSpacetime (void);
  void connectSpacetime (void);

};
#endif
