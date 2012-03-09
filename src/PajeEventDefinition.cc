#include "PajeEventDefinition.h"

std::map<std::string,PajeEventId> initPajeEventNamesToID ()
{
  std::map<std::string,PajeEventId> ret;
  ret["PajeStartTrace"] = PajeStartTraceEventId;
  ret["PajeDefineContainerType"] = PajeDefineContainerTypeEventId;
  ret["PajeDefineEventType"] = PajeDefineEventTypeEventId;
  ret["PajeDefineStateType"] = PajeDefineStateTypeEventId;
  ret["PajeDefineVariableType"] = PajeDefineVariableTypeEventId;
  ret["PajeDefineLinkType"] = PajeDefineLinkTypeEventId;
  ret["PajeDefineEntityValue"] = PajeDefineEntityValueEventId;
  ret["PajeCreateContainer"] = PajeCreateContainerEventId;
  ret["PajeDestroyContainer"] = PajeDestroyContainerEventId;
  ret["PajeNewEvent"] = PajeNewEventEventId;
  ret["PajeSetState"] = PajeSetStateEventId;
  ret["PajePushState"] = PajePushStateEventId;
  ret["PajePopState"] = PajePopStateEventId;
  ret["PajeSetVariable"] = PajeSetVariableEventId;
  ret["PajeAddVariable"] = PajeAddVariableEventId;
  ret["PajeSubVariable"] = PajeSubVariableEventId;
  ret["PajeStartLink"] = PajeStartLinkEventId;
  ret["PajeEndLink"] = PajeEndLinkEventId;
  return ret;
}

std::map<PajeEventId,std::string> initPajeEventIDToNames ()
{
  std::map<PajeEventId,std::string> ret;
  ret[PajeStartTraceEventId] = "PajeStartTrace";
  ret[PajeDefineContainerTypeEventId] = "PajeDefineContainerType";
  ret[PajeDefineEventTypeEventId] = "PajeDefineEventType";
  ret[PajeDefineStateTypeEventId] = "PajeDefineStateType";
  ret[PajeDefineVariableTypeEventId] = "PajeDefineVariableType";
  ret[PajeDefineLinkTypeEventId] = "PajeDefineLinkType";
  ret[PajeDefineEntityValueEventId] = "PajeDefineEntityValue";
  ret[PajeCreateContainerEventId] = "PajeCreateContainer";
  ret[PajeDestroyContainerEventId] = "PajeDestroyContainer";
  ret[PajeNewEventEventId] = "PajeNewEvent";
  ret[PajeSetStateEventId] = "PajeSetState";
  ret[PajePushStateEventId] = "PajePushState";
  ret[PajePopStateEventId] = "PajePopState";
  ret[PajeSetVariableEventId] = "PajeSetVariable";
  ret[PajeAddVariableEventId] = "PajeAddVariable";
  ret[PajeSubVariableEventId] = "PajeSubVariable";
  ret[PajeStartLinkEventId] = "PajeStartLink";
  ret[PajeEndLinkEventId] = "PajeEndLink";
  return ret;
}

std::map<std::string,PajeFieldId> initPajeFieldNamesToID ()
{
  std::map<std::string,PajeFieldId> ret;
  ret["EventId"] = PajeEventIdFieldId;
  ret["Time"] = PajeTimeFieldId;
  ret["Name"] = PajeNameFieldId;
  ret["Alias"] = PajeAliasFieldId;
  ret["Type"] = PajeTypeFieldId;
  ret["Container"] = PajeContainerFieldId;
  ret["StartContainerType"] = PajeStartContainerTypeFieldId;
  ret["EndContainerType"] = PajeEndContainerTypeFieldId;
  ret["StartContainer"] = PajeStartContainerFieldId;
  ret["EndContainer"] = PajeEndContainerFieldId;
  ret["Color"] = PajeColorFieldId;
  ret["Value"] = PajeValueFieldId;
  ret["Key"] = PajeKeyFieldId;
  ret["File"] = PajeFileFieldId;
  ret["Line"] = PajeLineFieldId;
  return ret;
}

std::map<PajeFieldId,std::string> initPajeFieldIDToNames ()
{
  std::map<PajeFieldId,std::string> ret;
  ret[PajeEventIdFieldId] = "EventId";
  ret[PajeTimeFieldId] = "Time";
  ret[PajeNameFieldId] = "Name";
  ret[PajeAliasFieldId] = "Alias";
  ret[PajeTypeFieldId] = "Type";
  ret[PajeContainerFieldId] = "Container";
  ret[PajeStartContainerTypeFieldId] = "StartContainerType";
  ret[PajeEndContainerTypeFieldId] = "EndContainerType";
  ret[PajeStartContainerFieldId] = "StartContainer";
  ret[PajeEndContainerFieldId] = "EndContainer";
  ret[PajeColorFieldId] = "Color";
  ret[PajeValueFieldId] = "Value";
  ret[PajeKeyFieldId] = "Key";
  ret[PajeFileFieldId] = "File";
  ret[PajeLineFieldId] = "Line";
  return ret;
}

std::map<std::string,PajeFieldType> initPajeFieldTypesToID ()
{
  std::map<std::string,PajeFieldType> ret;
  ret["int"] = PajeIntFieldType;
  ret["hex"] = PajeHexFieldType;
  ret["date"] = PajeDateFieldType;
  ret["double"] = PajeDoubleFieldType;
  ret["string"] = PajeStringFieldType;
  ret["color"] = PajeColorFieldType;
  return ret;
}

std::map<PajeFieldType,std::string> initPajeFieldIDToTypes ()
{
  std::map<PajeFieldType,std::string> ret;
  ret[PajeIntFieldType] = "int";
  ret[PajeHexFieldType] = "hex";
  ret[PajeDateFieldType] = "date";
  ret[PajeDoubleFieldType] = "double";
  ret[PajeStringFieldType] = "string";
  ret[PajeColorFieldType] = "color";
  return ret;
}


std::map<PajeEventId,std::set<PajeFieldId> > initObligatoryFields ()
{
  std::map<PajeEventId,std::set<PajeFieldId> > ret;

  // The obligatory fields
  ret[PajeStartTraceEventId] = std::set<PajeFieldId>();

  ret[PajeDefineContainerTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineContainerTypeEventId].insert (PajeNameFieldId);
  ret[PajeDefineContainerTypeEventId].insert (PajeTypeFieldId);

  ret[PajeDefineEventTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineEventTypeEventId].insert (PajeNameFieldId);
  ret[PajeDefineEventTypeEventId].insert (PajeTypeFieldId);

  ret[PajeDefineStateTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineStateTypeEventId].insert (PajeNameFieldId);
  ret[PajeDefineStateTypeEventId].insert (PajeTypeFieldId);

  ret[PajeDefineVariableTypeEventId]  = std::set<PajeFieldId>();
  ret[PajeDefineVariableTypeEventId].insert (PajeNameFieldId);
  ret[PajeDefineVariableTypeEventId].insert (PajeTypeFieldId);

  ret[PajeDefineLinkTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineLinkTypeEventId].insert (PajeNameFieldId);
  ret[PajeDefineLinkTypeEventId].insert (PajeTypeFieldId);
  ret[PajeDefineLinkTypeEventId].insert (PajeStartContainerTypeFieldId);
  ret[PajeDefineLinkTypeEventId].insert (PajeEndContainerTypeFieldId);

  ret[PajeDefineEntityValueEventId] = std::set<PajeFieldId>();
  ret[PajeDefineEntityValueEventId].insert (PajeNameFieldId);
  ret[PajeDefineEntityValueEventId].insert (PajeTypeFieldId);

  ret[PajeCreateContainerEventId] = std::set<PajeFieldId>();
  ret[PajeCreateContainerEventId].insert (PajeTimeFieldId);
  ret[PajeCreateContainerEventId].insert (PajeNameFieldId);
  ret[PajeCreateContainerEventId].insert (PajeTypeFieldId);
  ret[PajeCreateContainerEventId].insert (PajeContainerFieldId);

  ret[PajeDestroyContainerEventId] = std::set<PajeFieldId>();
  ret[PajeDestroyContainerEventId].insert (PajeTimeFieldId);
  ret[PajeDestroyContainerEventId].insert (PajeNameFieldId);
  ret[PajeDestroyContainerEventId].insert (PajeTypeFieldId);

  ret[PajeNewEventEventId] = std::set<PajeFieldId>();
  ret[PajeNewEventEventId].insert (PajeTimeFieldId);
  ret[PajeNewEventEventId].insert (PajeTypeFieldId);
  ret[PajeNewEventEventId].insert (PajeContainerFieldId);
  ret[PajeNewEventEventId].insert (PajeValueFieldId);

  ret[PajeSetStateEventId] = std::set<PajeFieldId>();
  ret[PajeSetStateEventId].insert (PajeTimeFieldId);
  ret[PajeSetStateEventId].insert (PajeTypeFieldId);
  ret[PajeSetStateEventId].insert (PajeContainerFieldId);
  ret[PajeSetStateEventId].insert (PajeValueFieldId);

  ret[PajePushStateEventId] = std::set<PajeFieldId>();
  ret[PajePushStateEventId].insert (PajeTimeFieldId);
  ret[PajePushStateEventId].insert (PajeTypeFieldId);
  ret[PajePushStateEventId].insert (PajeContainerFieldId);
  ret[PajePushStateEventId].insert (PajeValueFieldId);

  ret[PajePopStateEventId] = std::set<PajeFieldId>();
  ret[PajePopStateEventId].insert (PajeTimeFieldId);
  ret[PajePopStateEventId].insert (PajeTypeFieldId);
  ret[PajePopStateEventId].insert (PajeContainerFieldId);

  ret[PajeSetVariableEventId] = std::set<PajeFieldId>();
  ret[PajeSetVariableEventId].insert (PajeTimeFieldId);
  ret[PajeSetVariableEventId].insert (PajeTypeFieldId);
  ret[PajeSetVariableEventId].insert (PajeContainerFieldId);
  ret[PajeSetVariableEventId].insert (PajeValueFieldId);

  ret[PajeAddVariableEventId] = std::set<PajeFieldId>();
  ret[PajeAddVariableEventId].insert (PajeTimeFieldId);
  ret[PajeAddVariableEventId].insert (PajeTypeFieldId);
  ret[PajeAddVariableEventId].insert (PajeContainerFieldId);
  ret[PajeAddVariableEventId].insert (PajeValueFieldId);

  ret[PajeSubVariableEventId] = std::set<PajeFieldId>();
  ret[PajeSubVariableEventId].insert (PajeTimeFieldId);
  ret[PajeSubVariableEventId].insert (PajeTypeFieldId);
  ret[PajeSubVariableEventId].insert (PajeContainerFieldId);
  ret[PajeSubVariableEventId].insert (PajeValueFieldId);

  ret[PajeStartLinkEventId] = std::set<PajeFieldId>();
  ret[PajeStartLinkEventId].insert (PajeTimeFieldId);
  ret[PajeStartLinkEventId].insert (PajeTypeFieldId);
  ret[PajeStartLinkEventId].insert (PajeContainerFieldId);
  ret[PajeStartLinkEventId].insert (PajeValueFieldId);
  ret[PajeStartLinkEventId].insert (PajeStartContainerFieldId);
  ret[PajeStartLinkEventId].insert (PajeKeyFieldId);

  ret[PajeEndLinkEventId] = std::set<PajeFieldId>();
  ret[PajeEndLinkEventId].insert (PajeTimeFieldId);
  ret[PajeEndLinkEventId].insert (PajeTypeFieldId);
  ret[PajeEndLinkEventId].insert (PajeContainerFieldId);
  ret[PajeEndLinkEventId].insert (PajeValueFieldId);
  ret[PajeEndLinkEventId].insert (PajeEndContainerFieldId);
  ret[PajeEndLinkEventId].insert (PajeKeyFieldId);

  return ret;
}

std::map<PajeEventId,std::set<PajeFieldId> > initOptionalFields ()
{
  std::map<PajeEventId,std::set<PajeFieldId> > ret;

  // The optional fields
  ret[PajeStartTraceEventId] = std::set<PajeFieldId>();

  ret[PajeDefineContainerTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineContainerTypeEventId].insert (PajeAliasFieldId);

  ret[PajeDefineEventTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineEventTypeEventId].insert (PajeAliasFieldId);

  ret[PajeDefineStateTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineStateTypeEventId].insert (PajeAliasFieldId);

  ret[PajeDefineVariableTypeEventId]  = std::set<PajeFieldId>();
  ret[PajeDefineVariableTypeEventId].insert (PajeAliasFieldId);

  ret[PajeDefineLinkTypeEventId] = std::set<PajeFieldId>();
  ret[PajeDefineLinkTypeEventId].insert (PajeAliasFieldId);

  ret[PajeDefineEntityValueEventId] = std::set<PajeFieldId>();
  ret[PajeDefineEntityValueEventId].insert (PajeAliasFieldId);
  ret[PajeDefineEntityValueEventId].insert (PajeColorFieldId);

  ret[PajeCreateContainerEventId] = std::set<PajeFieldId>();
  ret[PajeCreateContainerEventId].insert (PajeAliasFieldId);

  ret[PajeDestroyContainerEventId] = std::set<PajeFieldId>();

  ret[PajeNewEventEventId] = std::set<PajeFieldId>();
  ret[PajeNewEventEventId].insert (PajeFileFieldId);
  ret[PajeNewEventEventId].insert (PajeLineFieldId);

  ret[PajeSetVariableEventId] = std::set<PajeFieldId>();
  ret[PajeSetVariableEventId].insert (PajeFileFieldId);
  ret[PajeSetVariableEventId].insert (PajeLineFieldId);

  ret[PajePushStateEventId] = std::set<PajeFieldId>();
  ret[PajePushStateEventId].insert (PajeFileFieldId);
  ret[PajePushStateEventId].insert (PajeLineFieldId);

  ret[PajePopStateEventId] = std::set<PajeFieldId>();
  ret[PajePopStateEventId].insert (PajeFileFieldId);
  ret[PajePopStateEventId].insert (PajeLineFieldId);

  ret[PajeSetVariableEventId] = std::set<PajeFieldId>();
  ret[PajeAddVariableEventId] = std::set<PajeFieldId>();
  ret[PajeSubVariableEventId] = std::set<PajeFieldId>();

  ret[PajeStartLinkEventId] = std::set<PajeFieldId>();
  ret[PajeStartLinkEventId].insert (PajeFileFieldId);
  ret[PajeStartLinkEventId].insert (PajeLineFieldId);

  ret[PajeEndLinkEventId] = std::set<PajeFieldId>();
  ret[PajeEndLinkEventId].insert (PajeFileFieldId);
  ret[PajeEndLinkEventId].insert (PajeLineFieldId);

  return ret;
}


static std::map<PajeEventId,std::set<PajeFieldId> > pajeObligatoryFields = initObligatoryFields ();
static std::map<PajeEventId,std::set<PajeFieldId> > pajeOptionalFields = initOptionalFields ();

static bool isObligatoryOrOptionalField (PajeEventId eventId, PajeFieldId fieldId)
{
  std::map<PajeEventId,std::set<PajeFieldId> >::iterator itmap;
  itmap = pajeObligatoryFields.find (eventId);
  if (itmap != pajeObligatoryFields.end()){
    std::set<PajeFieldId>::iterator itset;
    itset = itmap->second.find (fieldId);
    if (itset != itmap->second.end()){
      return true;
    }
  }
  itmap = pajeOptionalFields.find (eventId);
  if (itmap != pajeOptionalFields.end()){
    std::set<PajeFieldId>::iterator itset;
    itset = itmap->second.find (fieldId);
    if (itset != itmap->second.end()){
      return true;
    }
  }
  return false;
}

PajeEventDefinition::PajeEventDefinition (PajeEventId id, const char *id2)
{
  eventId = strdup (id2);
  pajeEventId = id;
  fieldCount = 1;
  extraFieldCount = 0;

  //the first field is always the event identification
  PajeEventDefinition::addField (PajeEventIdFieldId, PajeIntFieldType);
}

PajeEventDefinition::~PajeEventDefinition (void)
{
}

void PajeEventDefinition::addField (PajeFieldId fieldId, PajeFieldType fieldType)
{
  fields.push_back (fieldId);
  types.push_back (fieldType);

  //search for an existing index for this field
  std::map<PajeFieldId,int>::iterator it;
  if ((it = fieldIndexes.find (fieldId)) != fieldIndexes.end()){
    fprintf (stderr, "Event definition '%s' has repeated field named.\n", eventId.c_str());
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }else{
    fieldIndexes[fieldId] = fieldCount;
  }

  if (isObligatoryOrOptionalField(pajeEventId, fieldId)){
    extraFieldNames.push_back (fieldId);
    extraFieldCount++;
  }
  fieldCount++;
}

int PajeEventDefinition::indexForFieldId (PajeFieldId fieldId)
{
  std::map<PajeFieldId,int>::iterator it;
  if ((it = fieldIndexes.find (fieldId)) != fieldIndexes.end()){
    return it->second;
  }else{
    fprintf (stderr, "fieldId not present in this definition.\n");
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
    return -1;
  }
}

bool PajeEventDefinition::isValid (void)
{
  //get the obligatory fields for my pajeEventId
  std::map<PajeEventId,std::set<PajeFieldId> >::iterator itmap;
  itmap = pajeObligatoryFields.find (pajeEventId);
  if (itmap != pajeObligatoryFields.end()){
    //iterate through the set of obligatory fields for my pajeEventId
    std::set<PajeFieldId>::iterator itset;
    for (itset = itmap->second.begin (); itset != itmap->second.end(); itset++){
      PajeFieldId obligatoryFieldId = *itset;
      //search each  the obligatory fields in the fields list of this definition
      bool myFieldIsPresent = false;
      std::list<PajeFieldId>::iterator itlist;
      for (itlist = fields.begin(); itlist != fields.end(); itlist++){
        if (*itlist == obligatoryFieldId){
          myFieldIsPresent = true;
          break;
        }
      }
      if (!myFieldIsPresent){
        return false;
      }
    }
    return true;
  }else{
    return false;
  }
}

void PajeEventDefinition::showObligatoryFields (void)
{
  std::map<PajeEventId,std::string> eventNames = initPajeEventIDToNames();
  std::map<PajeFieldId,std::string> fieldNames = initPajeFieldIDToNames();

  std::cout << "These are the fields expected for a " << eventNames[pajeEventId]
            << " event definition:" << std::endl;
  std::set<PajeFieldId>::iterator iter;
  iter = pajeObligatoryFields[pajeEventId].begin();
  while (iter != pajeObligatoryFields[pajeEventId].end()){
    std::cout << fieldNames[*iter] << " ";
    iter++;
  }
  std::cout << std::endl;
}

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef)
{
  PajeEventDefinition def = (PajeEventDefinition)eventDef;
  std::map<PajeEventId,std::string> eventNames = initPajeEventIDToNames();
  std::map<PajeFieldId,std::string> fieldNames = initPajeFieldIDToNames();
  std::map<PajeFieldType,std::string> fieldTypes = initPajeFieldIDToTypes();
 
  output << "  %EventDef " << eventNames[def.pajeEventId] << " " << def.eventId << std::endl;
  std::list<PajeFieldId>::iterator itf = def.fields.begin();
  std::list<PajeFieldType>::iterator itt = def.types.begin();

  //don't print the first field (it is the event type)
  itf++;
  itt++;

  //print
  while (itf != def.fields.end()){
    output << "  %    " << fieldNames[*itf] << " " << fieldTypes[*itt] << std::endl;
    itf++;
    itt++;
  }
  output << "  %EndEventDef";
  return output;
}
