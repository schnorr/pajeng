#include "PajeThreadReader.h"
#include <QApplication>

PajeThreadReader::PajeThreadReader (PajeFileReader *reader)
{
  this->reader = reader;
}

PajeThreadReader::~PajeThreadReader (void)
{
}

void PajeThreadReader::read ()
{
  start();
  wait();
}

void PajeThreadReader::run ()
{
  while(!isFinished() && reader->hasMoreData()){
    static int i = 0;
    reader->startChunk (i++);
    reader->readNextChunk();
    reader->endOfChunkLast (!reader->hasMoreData());
    double percentage = (double)reader->traceRead()/(double)reader->traceSize()*100; 
    fprintf (stderr, "%f\r", percentage);
    emit percentageCompleted(percentage);
  }
  emit percentageCompleted(100);
  emit closeDialogIfStillOpen ();
}
