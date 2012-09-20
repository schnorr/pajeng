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
#include "WapajeApplication.h"

WapajeApplication::WapajeApplication( int &argc, char **argv) : QApplication(argc,argv)
{
  setApplicationName("Paje++");
  filename = arguments().at(1);
}

void WapajeApplication::init (void)
{
  wapajeWindow = WapajeWindow::getInstance ();
  wapajeWindow->setApplication (this);

  reader = new PajeFileReader (filename.toStdString());
  decoder = new PajeEventDecoder ();
  simulator = new PajeSimulator ();

  connectComponents (reader, decoder);
  connectComponents (decoder, simulator);
  connectSpacetime ();

  {
    PajeThreadReader *thread = new PajeThreadReader (reader);
    thread->read (); //this blocks until all file is read or user is bored
    delete thread;
  }

  wapajeWindow->show();
}

void WapajeApplication::connectComponents (PajeComponent *c1, PajeComponent *c2)
{
  c1->setOutputComponent (c2);
  c2->setInputComponent (c1);
}

void WapajeApplication::disconnectComponents (PajeComponent *c1, PajeComponent *c2)
{
  c1->disconnectOutputComponent (c2);
  c2->disconnectFromInput ();
}

void WapajeApplication::disconnectTreemap (void)
{
  disconnectComponents (simulator, wapajeWindow->treemapFrame->view());
}

void WapajeApplication::connectTreemap (void)
{
  connectComponents (simulator, wapajeWindow->treemapFrame->view());
}

void WapajeApplication::disconnectSpacetime (void)
{
  disconnectComponents (simulator, wapajeWindow->spacetimeFrame->view());
}

void WapajeApplication::connectSpacetime (void)
{
  connectComponents (simulator, wapajeWindow->spacetimeFrame->view());
}
