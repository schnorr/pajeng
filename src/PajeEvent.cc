#include "PajeEvent.h"

PajeEvent::PajeEvent (PajeEventDefinition *def, paje_line *line)
{
  valueLine = line;
  pajeEventDefinition = def;
}
