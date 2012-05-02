#include "PajeData.h"

PajeData::PajeData (int capacity)
{
  bytes = new char [capacity];
  length = capacity;
}

PajeData::~PajeData (void)
{
  if (bytes) delete [] bytes;
}
