#include "PajeEvent.h"

PajeEvent::PajeEvent (PajeEventDefinition *def, paje_line *line)
{
  if (line->word_count != def->fieldCount){
    fprintf (stderr, "Field count (%d) does not match definition (%d).\n",
             line->word_count, def->fieldCount);
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }

  valueLine = line;
  pajeEventDefinition = def;
}
