
#include "PajeEventDefinition.h"

PajeEventDefinition::PajeEventDefinition (PajeEventId id, const char *id2)
{
  eventId = strdup (id2);
  pajeEventId = id;
  fieldCount = 1;
  extraFieldCount = 0;
}

PajeEventDefinition::~PajeEventDefinition (void)
{
  free (eventId);
}

void PajeEventDefinition::addField (PajeFieldId name, PajeFieldType type)
{
  fieldNames.push_back (name);
  fieldTypes.push_back (type);

  //search for an existing index for this field
  std::map<PajeFieldId,int>::iterator it;
  it = fieldIndexes.find (name);
  if ((it = fieldIndexes.find (name)) != fieldIndexes.end()){
//    fprintf (stderr, "Repeated field named '%s' in event definition %s.\n");
    // pajeFieldNameFromId(fieldId), eventId);
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }else{
    fieldIndexes[name] = fieldCount;
  }

  if (PajeEventDefinition::isObligatoryOrOptionalFieldId(name)){
    extraFieldNames.push_back (name);
    extraFieldCount++;
  }
  fieldCount++;
}

bool PajeEventDefinition::isObligatoryOrOptionalFieldId (PajeFieldId name)
{
  fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
  exit(1);
  return false;
}
