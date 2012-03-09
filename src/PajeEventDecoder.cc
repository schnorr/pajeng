#include "PajeEventDecoder.h"

PajeEventDecoder::PajeEventDecoder ()
{
  /*
    Initialize the tables about the Paje file format.
  */

  // The event names
  pajeEventNames["PajeStartTrace"] = PajeStartTraceEventId;
  pajeEventNames["PajeDefineContainerType"] = PajeDefineContainerTypeEventId;
  pajeEventNames["PajeDefineEventType"] = PajeDefineEventTypeEventId;
  pajeEventNames["PajeDefineStateType"] = PajeDefineStateTypeEventId;
  pajeEventNames["PajeDefineVariableType"] = PajeDefineVariableTypeEventId;
  pajeEventNames["PajeDefineLinkType"] = PajeDefineLinkTypeEventId;
  pajeEventNames["PajeDefineEntityValue"] = PajeDefineEntityValueEventId;
  pajeEventNames["PajeCreateContainer"] = PajeCreateContainerEventId;
  pajeEventNames["PajeDestroyContainer"] = PajeDestroyContainerEventId;
  pajeEventNames["PajeNewEvent"] = PajeNewEventEventId;
  pajeEventNames["PajeSetState"] = PajeSetStateEventId;
  pajeEventNames["PajePushState"] = PajePushStateEventId;
  pajeEventNames["PajePopState"] = PajePopStateEventId;
  pajeEventNames["PajeSetVariable"] = PajeSetVariableEventId;
  pajeEventNames["PajeAddVariable"] = PajeAddVariableEventId;
  pajeEventNames["PajeSubVariable"] = PajeSubVariableEventId;
  pajeEventNames["PajeStartLink"] = PajeStartLinkEventId;
  pajeEventNames["PajeEndLink"] = PajeEndLinkEventId;

  // The field names = 
  pajeFieldNames["EventId"] = PajeEventIdFieldId;
  pajeFieldNames["Time"] = PajeTimeFieldId;
  pajeFieldNames["Name"] = PajeNameFieldId;
  pajeFieldNames["Alias"] = PajeAliasFieldId;
  pajeFieldNames["Type"] = PajeTypeFieldId;
  pajeFieldNames["Container"] = PajeContainerFieldId;
  pajeFieldNames["StartContainerType"] = PajeStartContainerTypeFieldId;
  pajeFieldNames["EndContainerType"] = PajeEndContainerTypeFieldId;
  pajeFieldNames["StartContainer"] = PajeStartContainerFieldId;
  pajeFieldNames["EndContainer"] = PajeEndContainerFieldId;
  pajeFieldNames["Color"] = PajeColorFieldId;
  pajeFieldNames["Value"] = PajeValueFieldId;
  pajeFieldNames["Key"] = PajeKeyFieldId;
  pajeFieldNames["File"] = PajeFileFieldId;
  pajeFieldNames["Line"] = PajeLineFieldId;

  // The field types
  pajeFieldTypes["int"] = PajeIntFieldType;
  pajeFieldTypes["hex"] = PajeHexFieldType;
  pajeFieldTypes["date"] = PajeDateFieldType;
  pajeFieldTypes["double"] = PajeDoubleFieldType;
  pajeFieldTypes["string"] = PajeStringFieldType;
  pajeFieldTypes["color"] = PajeColorFieldType;

  // The obligatory fields
  pajeObligatoryFields[PajeStartTraceEventId] = std::set<PajeFieldId>();

  pajeObligatoryFields[PajeDefineContainerTypeEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeDefineContainerTypeEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeDefineContainerTypeEventId].insert (PajeTypeFieldId);

  pajeObligatoryFields[PajeDefineEventTypeEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeDefineEventTypeEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeDefineEventTypeEventId].insert (PajeTypeFieldId);

  pajeObligatoryFields[PajeDefineStateTypeEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeDefineStateTypeEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeDefineStateTypeEventId].insert (PajeTypeFieldId);

  pajeObligatoryFields[PajeDefineVariableTypeEventId]  = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeDefineVariableTypeEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeDefineVariableTypeEventId].insert (PajeTypeFieldId);

  pajeObligatoryFields[PajeDefineLinkTypeEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeDefineLinkTypeEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeDefineLinkTypeEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeDefineLinkTypeEventId].insert (PajeStartContainerTypeFieldId);
  pajeObligatoryFields[PajeDefineLinkTypeEventId].insert (PajeEndContainerTypeFieldId);

  pajeObligatoryFields[PajeDefineEntityValueEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeDefineEntityValueEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeDefineEntityValueEventId].insert (PajeTypeFieldId);

  pajeObligatoryFields[PajeCreateContainerEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeCreateContainerEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeCreateContainerEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeCreateContainerEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeCreateContainerEventId].insert (PajeContainerFieldId);

  pajeObligatoryFields[PajeDestroyContainerEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeDestroyContainerEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeDestroyContainerEventId].insert (PajeNameFieldId);
  pajeObligatoryFields[PajeDestroyContainerEventId].insert (PajeTypeFieldId);

  pajeObligatoryFields[PajeNewEventEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeNewEventEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeNewEventEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeNewEventEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajeNewEventEventId].insert (PajeValueFieldId);

  pajeObligatoryFields[PajeSetVariableEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeValueFieldId);

  pajeObligatoryFields[PajePushStateEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajePushStateEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajePushStateEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajePushStateEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajePushStateEventId].insert (PajeValueFieldId);

  pajeObligatoryFields[PajePopStateEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajePopStateEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajePopStateEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajePopStateEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajePopStateEventId].insert (PajeValueFieldId);

  pajeObligatoryFields[PajeSetVariableEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajeSetVariableEventId].insert (PajeValueFieldId);

  pajeObligatoryFields[PajeAddVariableEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeAddVariableEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeAddVariableEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeAddVariableEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajeAddVariableEventId].insert (PajeValueFieldId);

  pajeObligatoryFields[PajeSubVariableEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeSubVariableEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeSubVariableEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeSubVariableEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajeSubVariableEventId].insert (PajeValueFieldId);

  pajeObligatoryFields[PajeStartLinkEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeStartLinkEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeStartLinkEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeStartLinkEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajeStartLinkEventId].insert (PajeValueFieldId);
  pajeObligatoryFields[PajeStartLinkEventId].insert (PajeStartContainerFieldId);
  pajeObligatoryFields[PajeStartLinkEventId].insert (PajeKeyFieldId);

  pajeObligatoryFields[PajeEndLinkEventId] = std::set<PajeFieldId>();
  pajeObligatoryFields[PajeEndLinkEventId].insert (PajeTimeFieldId);
  pajeObligatoryFields[PajeEndLinkEventId].insert (PajeTypeFieldId);
  pajeObligatoryFields[PajeEndLinkEventId].insert (PajeContainerFieldId);
  pajeObligatoryFields[PajeEndLinkEventId].insert (PajeValueFieldId);
  pajeObligatoryFields[PajeEndLinkEventId].insert (PajeEndContainerFieldId);
  pajeObligatoryFields[PajeEndLinkEventId].insert (PajeKeyFieldId);

  // The optional fields
  pajeOptionalFields[PajeStartTraceEventId] = std::set<PajeFieldId>();

  pajeOptionalFields[PajeDefineContainerTypeEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeDefineContainerTypeEventId].insert (PajeAliasFieldId);

  pajeOptionalFields[PajeDefineEventTypeEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeDefineEventTypeEventId].insert (PajeAliasFieldId);

  pajeOptionalFields[PajeDefineStateTypeEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeDefineStateTypeEventId].insert (PajeAliasFieldId);

  pajeOptionalFields[PajeDefineVariableTypeEventId]  = std::set<PajeFieldId>();
  pajeOptionalFields[PajeDefineVariableTypeEventId].insert (PajeAliasFieldId);

  pajeOptionalFields[PajeDefineLinkTypeEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeDefineLinkTypeEventId].insert (PajeAliasFieldId);

  pajeOptionalFields[PajeDefineEntityValueEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeDefineEntityValueEventId].insert (PajeAliasFieldId);
  pajeOptionalFields[PajeDefineEntityValueEventId].insert (PajeColorFieldId);

  pajeOptionalFields[PajeCreateContainerEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeCreateContainerEventId].insert (PajeAliasFieldId);

  pajeOptionalFields[PajeDestroyContainerEventId] = std::set<PajeFieldId>();

  pajeOptionalFields[PajeNewEventEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeNewEventEventId].insert (PajeFileFieldId);
  pajeOptionalFields[PajeNewEventEventId].insert (PajeLineFieldId);

  pajeOptionalFields[PajeSetVariableEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeSetVariableEventId].insert (PajeFileFieldId);
  pajeOptionalFields[PajeSetVariableEventId].insert (PajeLineFieldId);

  pajeOptionalFields[PajePushStateEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajePushStateEventId].insert (PajeFileFieldId);
  pajeOptionalFields[PajePushStateEventId].insert (PajeLineFieldId);

  pajeOptionalFields[PajePopStateEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajePopStateEventId].insert (PajeFileFieldId);
  pajeOptionalFields[PajePopStateEventId].insert (PajeLineFieldId);

  pajeOptionalFields[PajeSetVariableEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeAddVariableEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeSubVariableEventId] = std::set<PajeFieldId>();

  pajeOptionalFields[PajeStartLinkEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeStartLinkEventId].insert (PajeFileFieldId);
  pajeOptionalFields[PajeStartLinkEventId].insert (PajeLineFieldId);

  pajeOptionalFields[PajeEndLinkEventId] = std::set<PajeFieldId>();
  pajeOptionalFields[PajeEndLinkEventId].insert (PajeFileFieldId);
  pajeOptionalFields[PajeEndLinkEventId].insert (PajeLineFieldId);  

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
      // TODO: verify if all obligatory fields are defined
//      fprintf (stderr, "verify if all obligatory fields are defined\n");
      //     fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
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
