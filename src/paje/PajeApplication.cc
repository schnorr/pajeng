#include "PajeApplication.h"

PajeApplication::PajeApplication( int &argc, char **argv) : QApplication(argc,argv)
{
  setApplicationName("Paje++");
  filename = arguments().at(1);
}

void PajeApplication::init (void)
{
  pajeWindow = PajeWindow::getInstance ();
  pajeWindow->setApplication (this);

  reader = new PajeFileReader (filename.toStdString(), NULL);
  decoder = new PajeEventDecoder ();
  simulator = new PajeSimulator ();

  connectComponents (reader, decoder);
  connectComponents (decoder, simulator);
  connectSpacetime ();
  connectTreemap ();

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

void PajeApplication::disconnectTreemap (void)
{
  disconnectComponents (simulator, pajeWindow->treemapFrame->view());
}

void PajeApplication::connectTreemap (void)
{
  connectComponents (simulator, pajeWindow->treemapFrame->view());
}

void PajeApplication::disconnectSpacetime (void)
{
  disconnectComponents (simulator, pajeWindow->spacetimeFrame->view());
}

void PajeApplication::connectSpacetime (void)
{
  connectComponents (simulator, pajeWindow->spacetimeFrame->view());
}
