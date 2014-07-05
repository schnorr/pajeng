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
#include "PajeApplication.h"

PajeApplication::PajeApplication( int &argc, char **argv) : QApplication(argc,argv)
{
  setApplicationName("Paje++");
  filename.clear();
  if (arguments().count() > 1){
    filename = arguments().at(1);
  }
}

void PajeApplication::init (void)
{
  pajeWindow = PajeWindow::getInstance ();
  pajeWindow->setApplication (this);

  if (filename.isEmpty()){
    reader = new PajeFileReader ();
  }else{
    reader = new PajeFileReader (filename.toStdString());
  }

  decoder = new PajeEventDecoder (new PajeDefinitions (true));
  simulator = new PajeSimulator ();

  connectComponents (reader, decoder);
  connectComponents (decoder, simulator);
  connectSpacetime ();

  {
    PajeThreadReader *thread = new PajeThreadReader (reader);
    thread->read (); //this blocks until all file is read or user is bored
    delete thread;
  }

  pajeWindow->show();
}

void PajeApplication::connectComponents (PajeComponent *c1, PajeComponent *c2)
{
  c1->setOutputComponent (c2);
  c2->setInputComponent (c1);
}

void PajeApplication::disconnectComponents (PajeComponent *c1, PajeComponent *c2)
{
  c1->disconnectOutputComponent (c2);
  c2->disconnectFromInput ();
}

void PajeApplication::disconnectSpacetime (void)
{
  disconnectComponents (simulator, pajeWindow->spacetimeFrame->view());
}

void PajeApplication::connectSpacetime (void)
{
  connectComponents (simulator, pajeWindow->spacetimeFrame->view());
}
