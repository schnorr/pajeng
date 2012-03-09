#include "PajeEventDecoder.h"

PajeEventDecoder::PajeEventDecoder ()
{
  pajeEventNames = initPajeEventNamesToID ();
  pajeFieldNames = initPajeFieldNamesToID ();
  pajeFieldTypes = initPajeFieldTypesToID ();
  defStatus = OUT_DEF;
  eventCount = 0;

  //TODO FIXME
  //chunkInfo ...
}

char *PajeEventDecoder::break_line (char *s, paje_line *line)
{
  bool in_string = false;
  bool in_word = false;
  char *p;
  line->word_count = 0;

  for (p = s; *p != '\0'; p++) {
    if (*p == '\n') {
      *p = '\0';
      p++;
      break;
    }
    if (in_string) {
      if (*p == '"') {
        *p = '\0';
        in_string = false;
      }
      continue;
    }
    if (*p == '#') {
      *p = '\0';
      while (true) {
        p++;
        if (*p == '\n') {
          p++;
          break;
        } else if (*p == '\0') {
          break;
        }
      }
      break;
    }
    if (in_word && isspace(*p)) {
      *p = '\0';
      in_word = false;
      continue;
    }
    if (!in_word && !isspace(*p)) {
      if (*p == '"') {
        p++;
        in_string = true;
      } else {
        in_word = true;
      }
      if (line->word_count < PAJE_MAX_FIELDS) {
        line->word[line->word_count] = p;
        line->word_count ++;
      }
      continue;
    }
  }
  return p;
}

void PajeEventDecoder::scanDefinitionLine (paje_line *line)
{
  char *str;
  int n = 0;
  char *eventName;
  char *eventId;
  char *fieldName;
  char *fieldType;

  str = line->word[n++];
  if (*str++ != '%') {
    // [self raise:@"Line should start with a '%%'"];
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }
  if (*str == '\0') {
    str = line->word[n++];
  }

  switch (defStatus) {
  case OUT_DEF:
  {
    eventName = line->word[n++];
    eventId   = line->word[n++];
    if (n != line->word_count || strcmp(str, "EventDef") != 0) {
      fprintf (stderr, "'EventDef <event name> <event id>' expected.\n");
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    if (eventDefinitions[eventId]){
      fprintf (stderr, "redefinition of event with id '%s'\n", eventId);
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    PajeEventId pajeEventId = PajeEventDecoder::getPajeEventId (eventName);
    if (pajeEventId == PajeUnknownEventId) {
      fprintf (stderr, "unknown event name '%s'\n", eventName);
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    eventBeingDefined = new PajeEventDefinition (pajeEventId, eventId);
    eventDefinitions[eventId] = eventBeingDefined;
    defStatus = IN_DEF;
  }
  break;
  case IN_DEF:
  {
    fieldName = str;

    if (n > line->word_count) {
      fprintf (stderr, "Incomplete line, missing field name\n");
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }
    if (strcmp(fieldName, "EndEventDef") == 0) {
      if (!eventBeingDefined->isValid()){
        std::cout << "This event definition is not valid:" << std::endl;
        std::cout << *eventBeingDefined << std::endl;
        eventBeingDefined->showObligatoryFields();
        exit(1);
      }
      defStatus = OUT_DEF;
      break;
    }

    PajeFieldId fieldId = PajeEventDecoder::getPajeFieldId (fieldName);
    if (fieldId == PajeUnknownFieldId){
      fprintf (stderr, "Unrecognised field name '%s'\n", fieldName);
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    if (n >= line->word_count) {
      fprintf (stderr, "Incomplete line, missing field type\n");
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }
    fieldType = line->word[n++];

    PajeFieldType fieldTypeId = PajeEventDecoder::getPajeFieldType (fieldType);

    if (fieldTypeId == PajeUnknownFieldType) {
      fprintf (stderr, "Unrecognised field type '%s'\n", fieldType);
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }
    eventBeingDefined->addField (fieldId, fieldTypeId);
  }
  break;
  default:
    fprintf (stderr, "Internal error, invalid status\n");
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }
}

PajeEvent *PajeEventDecoder::scanEventLine (paje_line *line)
{
  char *eventId = NULL;
  PajeEvent *event = NULL;
  PajeEventDefinition *eventDefinition = NULL;

  eventId = line->word[0];
  if (*eventId == '%') {
    fprintf (stderr, "Line should not start with a '%%'\n");
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }
  eventDefinition = eventDefinitions[eventId];
  if (eventDefinition == NULL) {
    fprintf (stderr, "Event with id '%s' has not been defined\n", eventId);
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }

  // event = [PajeEvent eventWithDefinition:eventDefinition line:line];

  eventCount++;
  return event;
}

PajeEventId PajeEventDecoder::getPajeEventId (std::string eventName)
{
  std::map<std::string,PajeEventId>::iterator it;
  if ((it = pajeEventNames.find (eventName)) != pajeEventNames.end()){
    return it->second;
  }else{
    return PajeUnknownEventId;
  }
}

PajeFieldId PajeEventDecoder::getPajeFieldId (std::string fieldName)
{
  std::map<std::string,PajeFieldId>::iterator it;
  if ((it = pajeFieldNames.find (fieldName)) != pajeFieldNames.end()){
    return it->second;
  }else{
    return PajeUnknownFieldId;
  }
}

PajeFieldType PajeEventDecoder::getPajeFieldType (std::string fieldType)
{
  std::map<std::string,PajeFieldType>::iterator it;
  if ((it = pajeFieldTypes.find (fieldType)) != pajeFieldTypes.end()){
    return it->second;
  }else{
    return PajeUnknownFieldType;
  } 
}

void PajeEventDecoder::inputEntity (PajeObject *data)
{
  PajeData *d = (PajeData*)data;
  paje_line line;

  char *dataPointer = d->bytes;
  char *initDataPointer = d->bytes;
  int length = d->length;
  while ((dataPointer - initDataPointer) < length){
    dataPointer = PajeEventDecoder::break_line (dataPointer, &line);
    if (line.word_count == 0) {
      continue;
    }
    if (line.word[0][0] == '%') {
      PajeEventDecoder::scanDefinitionLine (&line);
    } else {
      PajeEvent *event = PajeEventDecoder::scanEventLine (&line);
      if (event != NULL){
        PajeComponent::outputEntity (event);
      }
    }
  }
}

bool PajeEventDecoder::canEndChunkBefore (PajeObject *data)
{
  PajeData *d = (PajeData*)data;
  paje_line line;

  bool canEndChunk = true;

  char *dataPointer = d->bytes;
  char *initDataPointer = d->bytes;
  int length = d->length;
  while ((dataPointer - initDataPointer) < length){
    dataPointer = PajeEventDecoder::break_line (dataPointer, &line);
    if (line.word_count == 0) {
      continue;
    }
    if (line.word[0][0] == '%') {
      PajeEventDecoder::scanDefinitionLine (&line);
      canEndChunk = false;
      break;
    }else{
      PajeEvent *event = PajeEventDecoder::scanEventLine (&line);
      if (event != NULL){
        canEndChunk = PajeComponent::canEndChunkBefore (event);
        break;
      }
    }
  }
  if (canEndChunk){
    return canEndChunk;
  }
  // the chunk cannot be ended -- process all remaining data
  while ((dataPointer - initDataPointer) < length) {
    dataPointer = break_line(dataPointer, &line);
    if (line.word_count == 0) {
      continue;
    }
    if (line.word[0][0] == '%') {
      PajeEventDecoder::scanDefinitionLine (&line);
    }else{
      PajeEvent *event = PajeEventDecoder::scanEventLine (&line);
      if (event != NULL){
        PajeComponent::outputEntity (event);
      }
    }
  }
  return false;
}
