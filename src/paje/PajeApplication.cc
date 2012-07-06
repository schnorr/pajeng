#include "PajeApplication.h"
#include "../utils/PajeThreadReader.h"
#include "PajeFileReader.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"

PajeApplication::PajeApplication( int &argc, char **argv) : QApplication(argc,argv)
{
  setApplicationName("Paje++");
  filename = arguments().at(1);
}

void PajeApplication::init (void)
{
  pajeWindow = PajeWindow::getInstance ();
  PajeFileReader *reader = new PajeFileReader (filename.toStdString(), NULL);
  PajeEventDecoder *decoder = new PajeEventDecoder ();
  PajeSimulator *simulator = new PajeSimulator ();

  connectComponents (reader, decoder);
  connectComponents (decoder, simulator);
  connectComponents (simulator, pajeWindow->spacetimeFrame->view());
  connectComponents (simulator, pajeWindow->treemapFrame->view());

  {
    PajeThreadReader *thread = new PajeThreadReader (reader);
    thread->read (); //this blocks until all file is read or user is bored
    delete thread;
  }

  simulator->report();
  pajeWindow->show();
}

void PajeApplication::connectComponents (PajeComponent *c1, PajeComponent *c2)
{
  c1->setOutputComponent (c2);
  c2->setInputComponent (c1);
}
