#include "PajeEventDecoder.h"

PajeEventDecoder::PajeEventDecoder ()
{
}

void PajeEventDecoder::inputEntity (void *data)
{
  PajeComponent::outputEntity (data);
}

bool PajeEventDecoder::canEndChunkBefore (void *data)
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
