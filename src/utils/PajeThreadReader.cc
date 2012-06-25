#include "PajeThreadReader.h"
#include <QApplication>

PajeThreadReader::PajeThreadReader (PajeFileReader *reader)
{
  qRegisterMetaType<std::string>("std::string");

  killed = false;
  dialog = new QProgressDialog ("Loading file...", "Cancel", 0, 100);
  dialog->setWindowModality (Qt::ApplicationModal);
  dialog->setModal (true);
  dialog->setMinimumDuration (0);
  dialog->show();
  dialog->setVisible(true);
  connect(this, SIGNAL(percentageCompleted(int)),
          dialog, SLOT(setValue(int)));
  connect(this, SIGNAL(closeDialogIfStillOpen(void)),
          dialog, SLOT(cancel(void)));
  connect (this, SIGNAL(readingException(std::string)),
           this, SLOT(throwException(std::string)));
  this->reader = reader;
}

PajeThreadReader::~PajeThreadReader (void)
{
  delete dialog;
}

void PajeThreadReader::read ()
{
  start();
  dialog->exec();
  killed = true;
  wait ();
  dialog->close();
}

void PajeThreadReader::run ()
{
  setPriority (QThread::HighestPriority);
  while(!isFinished() && reader->hasMoreData() && !killed){
    static int i = 0;
    try {
      reader->startChunk (i++);
      reader->readNextChunk();
      reader->endOfChunkLast (!reader->hasMoreData());
      double percentage = (double)reader->traceRead()/(double)reader->traceSize(); 
      emit percentageCompleted(percentage*100);
    }catch(std::string exception){
      emit readingException (exception);
      break;
    }
  }
  emit percentageCompleted(100);
  emit closeDialogIfStillOpen ();
}

void PajeThreadReader::throwException (std::string exception)
{
  throw exception;
}
