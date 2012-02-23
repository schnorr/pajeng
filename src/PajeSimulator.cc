#include "PajeSimulator.h"

PajeSimulator::PajeSimulator ()
{
}

void PajeSimulator::inputEntity (void *data)
{
}

bool PajeSimulator::canEndChunkBefore (void *data)
{
}

void PajeSimulator::startChunk (int chunkNumber)
{
  std::cout << __FUNCTION__ << " " << chunkNumber << std::endl;
}

void PajeSimulator::endOfChunkLast (bool last)
{
  std::cout << __FUNCTION__ << " " << last << std::endl;
}

