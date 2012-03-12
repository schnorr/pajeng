#include "PajeEventDecoder.h"

static std::map<std::string,PajeEventId> knownPajeEvents = initPajeEventNamesToID ();
static PajeEventId getPajeEventId (std::string eventName)
{
  std::map<std::string,PajeEventId>::iterator it;
  if ((it = knownPajeEvents.find (eventName)) != knownPajeEvents.end()){
    return it->second;
  }else{
    return PajeUnknownEventId;
  }
}

PajeEventDecoder::PajeEventDecoder ()
{
  defStatus = OUT_DEF;

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
    fprintf (stderr, "Line should start with a '%%'\n");
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

    //check if this event definition has a good start
    if (n != line->word_count || strcmp(str, "EventDef") != 0) {
      fprintf (stderr, "'EventDef <event name> <event id>' expected.\n");
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    //check if this event definition has been already defined
    if (eventDefinitions[eventId]){
      fprintf (stderr, "redefinition of event with id '%s'\n", eventId);
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    //check if we know the event name found in the trace file
    PajeEventId pajeEventId = getPajeEventId (eventName);
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
      throw "Incomplete line, missing field name";
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

    if (n >= line->word_count) {
      std::map<PajeEventId,std::string> eventNames = initPajeEventIDToNames();
      std::string name = eventNames[eventBeingDefined->pajeEventId];

      throw "Incomplete line, missing field type for " + name +
        " with id " + eventBeingDefined->number;
    }

    fieldType = line->word[n++];
    eventBeingDefined->addField (fieldName, fieldType);
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

  return event;
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
