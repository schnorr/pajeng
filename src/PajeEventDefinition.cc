#include "PajeEventDefinition.h"

PajeEventDefinition::PajeEventDefinition (PajeEventId id, const char *id2)
{
  pajeEventId = id;
  eventId = strdup (id2);
}

void PajeEventDefinition::addField (PajeFieldId id, PajeFieldType type)
{
}

PajeEventDefinition::~PajeEventDefinition (void)
{
  free (eventId);
}
