#include "PajeEventDecoder.h"

PajeEventDecoder::PajeEventDecoder ()
{
}

void PajeEventDecoder::inputEntity (PajeObject *data)
{
  PajeData *d = (PajeData*)data;
  line x;

  std::cout << d->bytes;
  PajeComponent::outputEntity (data);
}

bool PajeEventDecoder::canEndChunkBefore (PajeObject *data)
{
  PajeComponent::canEndChunkBefore (data);
}

void PajeEventDecoder::startChunk (int chunkNumber)
{
  PajeComponent::startChunk (chunkNumber);
}

void PajeEventDecoder::endOfChunkLast (bool last)
{
  PajeComponent::endOfChunkLast (last);
}
