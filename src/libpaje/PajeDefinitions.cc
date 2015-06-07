/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeDefinitions.h"
#include "PajeException.h"

PajeDefinitions::PajeDefinitions (bool strict)
{
  initialize (strict);
}

PajeDefinitions::~PajeDefinitions ()
{
  //boost::bimap
  eventNames.clear ();
  fieldTypeNames.clear ();

  //std::map
  fieldNameToID.clear();
  idToFieldName.clear ();
  obligatoryFields.clear ();
  optionalFields.clear ();
}

void PajeDefinitions::initialize (bool strict)
{
  //map from event names to internal reference
  eventNames.insert (event_names_dictionary_t::relation ("PajeDefineContainerType", PajeDefineContainerTypeEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeDefineContainerType", PajeDefineContainerTypeEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeDefineEventType", PajeDefineEventTypeEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeDefineStateType", PajeDefineStateTypeEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeDefineVariableType", PajeDefineVariableTypeEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeDefineLinkType", PajeDefineLinkTypeEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeDefineEntityValue", PajeDefineEntityValueEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeCreateContainer", PajeCreateContainerEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeDestroyContainer", PajeDestroyContainerEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeNewEvent", PajeNewEventEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeSetState", PajeSetStateEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajePushState", PajePushStateEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajePopState", PajePopStateEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeResetState", PajeResetStateEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeSetVariable", PajeSetVariableEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeAddVariable", PajeAddVariableEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeSubVariable", PajeSubVariableEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeStartLink", PajeStartLinkEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeEndLink", PajeEndLinkEventId));
  eventNames.insert (event_names_dictionary_t::relation ("PajeTraceFile", PajeTraceFileEventId));

  //map from field names to internal reference
  fieldNameToID.insert (std::pair<std::string,PajeField>("Event", PAJE_Event));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Time", PAJE_Time));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Name", PAJE_Name));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Type", PAJE_Type));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Container", PAJE_Container));
  fieldNameToID.insert (std::pair<std::string,PajeField>("StartContainerType", PAJE_StartContainerType));
  fieldNameToID.insert (std::pair<std::string,PajeField>("EndContainerType", PAJE_EndContainerType));
  fieldNameToID.insert (std::pair<std::string,PajeField>("StartContainer", PAJE_StartContainer));
  fieldNameToID.insert (std::pair<std::string,PajeField>("EndContainer", PAJE_EndContainer));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Value", PAJE_Value));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Key", PAJE_Key));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Alias", PAJE_Alias));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Color", PAJE_Color));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Line", PAJE_Line));
  fieldNameToID.insert (std::pair<std::string,PajeField>("File", PAJE_File));
  fieldNameToID.insert (std::pair<std::string,PajeField>("Filename", PAJE_Filename));
  if (strict == false){ //old field names (don't use them)
    fieldNameToID.insert (std::pair<std::string,PajeField>("ContainerType", PAJE_Type));
    fieldNameToID.insert (std::pair<std::string,PajeField>("EntityType", PAJE_Type));
    fieldNameToID.insert (std::pair<std::string,PajeField>("SourceContainerType", PAJE_StartContainerType));
    fieldNameToID.insert (std::pair<std::string,PajeField>("DestContainerType", PAJE_EndContainerType));
    fieldNameToID.insert (std::pair<std::string,PajeField>("SourceContainer", PAJE_StartContainer));
    fieldNameToID.insert (std::pair<std::string,PajeField>("DestContainer", PAJE_EndContainer));
  }

  //map from internal reference to field names
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Event             ,"Event"              ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Time              ,"Time"               ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Name              ,"Name"               ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Type              ,"Type"               ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Container         ,"Container"          ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_StartContainerType,"StartContainerType" ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_EndContainerType  ,"EndContainerType"   ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_StartContainer    ,"StartContainer"     ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_EndContainer      ,"EndContainer"       ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Value             ,"Value"              ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Key               ,"Key"                ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Alias             ,"Alias"              ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Color             ,"Color"              ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Line              ,"Line"               ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_File              ,"File"               ));
  idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Filename          ,"Filename"           ));
  if (strict == false){ //old field names (don't use them)
    idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Type              ,"ContainerType"       ));
    idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_Type              ,"EntityType"          ));
    idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_StartContainerType,"SourceContainerType" ));
    idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_EndContainerType  ,"DestContainerType"   ));
    idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_StartContainer    ,"SourceContainer"     ));
    idToFieldName.insert (std::pair<PajeField,std::string>(PAJE_EndContainer      ,"DestContainer"       ));
  }

  //map from type names to internal reference
  fieldTypeNames.insert (field_names_type_dictionary_t::relation ("string", PAJE_string));
  fieldTypeNames.insert (field_names_type_dictionary_t::relation ("float", PAJE_float));
  fieldTypeNames.insert (field_names_type_dictionary_t::relation ("double", PAJE_double));
  fieldTypeNames.insert (field_names_type_dictionary_t::relation ("int", PAJE_int));
  fieldTypeNames.insert (field_names_type_dictionary_t::relation ("hex", PAJE_hex));
  fieldTypeNames.insert (field_names_type_dictionary_t::relation ("date", PAJE_date));
  fieldTypeNames.insert (field_names_type_dictionary_t::relation ("color", PAJE_color));

  //The obligatory fields
  obligatoryFields[PajeDefineContainerTypeEventId] = std::set<PajeField>();
  obligatoryFields[PajeDefineContainerTypeEventId].insert (PAJE_Event);
  obligatoryFields[PajeDefineContainerTypeEventId].insert (PAJE_Name);
  obligatoryFields[PajeDefineContainerTypeEventId].insert (PAJE_Type);

  obligatoryFields[PajeDefineEventTypeEventId] = std::set<PajeField>();
  obligatoryFields[PajeDefineEventTypeEventId].insert (PAJE_Event);
  obligatoryFields[PajeDefineEventTypeEventId].insert (PAJE_Name);
  obligatoryFields[PajeDefineEventTypeEventId].insert (PAJE_Type);

  obligatoryFields[PajeDefineStateTypeEventId] = std::set<PajeField>();
  obligatoryFields[PajeDefineStateTypeEventId].insert (PAJE_Event);
  obligatoryFields[PajeDefineStateTypeEventId].insert (PAJE_Name);
  obligatoryFields[PajeDefineStateTypeEventId].insert (PAJE_Type);

  obligatoryFields[PajeDefineVariableTypeEventId]  = std::set<PajeField>();
  obligatoryFields[PajeDefineVariableTypeEventId].insert (PAJE_Event);
  obligatoryFields[PajeDefineVariableTypeEventId].insert (PAJE_Name);
  obligatoryFields[PajeDefineVariableTypeEventId].insert (PAJE_Type);

  obligatoryFields[PajeDefineLinkTypeEventId] = std::set<PajeField>();
  obligatoryFields[PajeDefineLinkTypeEventId].insert (PAJE_Event);
  obligatoryFields[PajeDefineLinkTypeEventId].insert (PAJE_Name);
  obligatoryFields[PajeDefineLinkTypeEventId].insert (PAJE_Type);
  obligatoryFields[PajeDefineLinkTypeEventId].insert (PAJE_StartContainerType);
  obligatoryFields[PajeDefineLinkTypeEventId].insert (PAJE_EndContainerType);

  obligatoryFields[PajeDefineEntityValueEventId] = std::set<PajeField>();
  obligatoryFields[PajeDefineEntityValueEventId].insert (PAJE_Event);
  obligatoryFields[PajeDefineEntityValueEventId].insert (PAJE_Name);
  obligatoryFields[PajeDefineEntityValueEventId].insert (PAJE_Type);

  obligatoryFields[PajeCreateContainerEventId] = std::set<PajeField>();
  obligatoryFields[PajeCreateContainerEventId].insert (PAJE_Event);
  obligatoryFields[PajeCreateContainerEventId].insert (PAJE_Time);
  obligatoryFields[PajeCreateContainerEventId].insert (PAJE_Name);
  obligatoryFields[PajeCreateContainerEventId].insert (PAJE_Type);
  obligatoryFields[PajeCreateContainerEventId].insert (PAJE_Container);

  obligatoryFields[PajeDestroyContainerEventId] = std::set<PajeField>();
  obligatoryFields[PajeDestroyContainerEventId].insert (PAJE_Event);
  obligatoryFields[PajeDestroyContainerEventId].insert (PAJE_Time);
  obligatoryFields[PajeDestroyContainerEventId].insert (PAJE_Name);
  obligatoryFields[PajeDestroyContainerEventId].insert (PAJE_Type);

  obligatoryFields[PajeNewEventEventId] = std::set<PajeField>();
  obligatoryFields[PajeNewEventEventId].insert (PAJE_Event);
  obligatoryFields[PajeNewEventEventId].insert (PAJE_Time);
  obligatoryFields[PajeNewEventEventId].insert (PAJE_Type);
  obligatoryFields[PajeNewEventEventId].insert (PAJE_Container);
  obligatoryFields[PajeNewEventEventId].insert (PAJE_Value);

  obligatoryFields[PajeSetStateEventId] = std::set<PajeField>();
  obligatoryFields[PajeSetStateEventId].insert (PAJE_Event);
  obligatoryFields[PajeSetStateEventId].insert (PAJE_Time);
  obligatoryFields[PajeSetStateEventId].insert (PAJE_Type);
  obligatoryFields[PajeSetStateEventId].insert (PAJE_Container);
  obligatoryFields[PajeSetStateEventId].insert (PAJE_Value);

  obligatoryFields[PajePushStateEventId] = std::set<PajeField>();
  obligatoryFields[PajePushStateEventId].insert (PAJE_Event);
  obligatoryFields[PajePushStateEventId].insert (PAJE_Time);
  obligatoryFields[PajePushStateEventId].insert (PAJE_Type);
  obligatoryFields[PajePushStateEventId].insert (PAJE_Container);
  obligatoryFields[PajePushStateEventId].insert (PAJE_Value);

  obligatoryFields[PajePopStateEventId] = std::set<PajeField>();
  obligatoryFields[PajePopStateEventId].insert (PAJE_Event);
  obligatoryFields[PajePopStateEventId].insert (PAJE_Time);
  obligatoryFields[PajePopStateEventId].insert (PAJE_Type);
  obligatoryFields[PajePopStateEventId].insert (PAJE_Container);

  obligatoryFields[PajeResetStateEventId] = std::set<PajeField>();
  obligatoryFields[PajeResetStateEventId].insert (PAJE_Event);
  obligatoryFields[PajeResetStateEventId].insert (PAJE_Time);
  obligatoryFields[PajeResetStateEventId].insert (PAJE_Type);
  obligatoryFields[PajeResetStateEventId].insert (PAJE_Container);

  obligatoryFields[PajeSetVariableEventId] = std::set<PajeField>();
  obligatoryFields[PajeSetVariableEventId].insert (PAJE_Event);
  obligatoryFields[PajeSetVariableEventId].insert (PAJE_Time);
  obligatoryFields[PajeSetVariableEventId].insert (PAJE_Type);
  obligatoryFields[PajeSetVariableEventId].insert (PAJE_Container);
  obligatoryFields[PajeSetVariableEventId].insert (PAJE_Value);

  obligatoryFields[PajeAddVariableEventId] = std::set<PajeField>();
  obligatoryFields[PajeAddVariableEventId].insert (PAJE_Event);
  obligatoryFields[PajeAddVariableEventId].insert (PAJE_Time);
  obligatoryFields[PajeAddVariableEventId].insert (PAJE_Type);
  obligatoryFields[PajeAddVariableEventId].insert (PAJE_Container);
  obligatoryFields[PajeAddVariableEventId].insert (PAJE_Value);

  obligatoryFields[PajeSubVariableEventId] = std::set<PajeField>();
  obligatoryFields[PajeSubVariableEventId].insert (PAJE_Event);
  obligatoryFields[PajeSubVariableEventId].insert (PAJE_Time);
  obligatoryFields[PajeSubVariableEventId].insert (PAJE_Type);
  obligatoryFields[PajeSubVariableEventId].insert (PAJE_Container);
  obligatoryFields[PajeSubVariableEventId].insert (PAJE_Value);

  obligatoryFields[PajeStartLinkEventId] = std::set<PajeField>();
  obligatoryFields[PajeStartLinkEventId].insert (PAJE_Event);
  obligatoryFields[PajeStartLinkEventId].insert (PAJE_Time);
  obligatoryFields[PajeStartLinkEventId].insert (PAJE_Type);
  obligatoryFields[PajeStartLinkEventId].insert (PAJE_Container);
  obligatoryFields[PajeStartLinkEventId].insert (PAJE_Value);
  obligatoryFields[PajeStartLinkEventId].insert (PAJE_StartContainer);
  obligatoryFields[PajeStartLinkEventId].insert (PAJE_Key);

  obligatoryFields[PajeEndLinkEventId] = std::set<PajeField>();
  obligatoryFields[PajeEndLinkEventId].insert (PAJE_Event);
  obligatoryFields[PajeEndLinkEventId].insert (PAJE_Time);
  obligatoryFields[PajeEndLinkEventId].insert (PAJE_Type);
  obligatoryFields[PajeEndLinkEventId].insert (PAJE_Container);
  obligatoryFields[PajeEndLinkEventId].insert (PAJE_Value);
  obligatoryFields[PajeEndLinkEventId].insert (PAJE_EndContainer);
  obligatoryFields[PajeEndLinkEventId].insert (PAJE_Key);

  obligatoryFields[PajeTraceFileEventId] = std::set<PajeField>();
  obligatoryFields[PajeTraceFileEventId].insert (PAJE_Container);
  obligatoryFields[PajeTraceFileEventId].insert (PAJE_Type);
  obligatoryFields[PajeTraceFileEventId].insert (PAJE_Filename);

  // The optional fields
  optionalFields[PajeDefineContainerTypeEventId] = std::set<PajeField>();
  optionalFields[PajeDefineContainerTypeEventId].insert (PAJE_Alias);

  optionalFields[PajeDefineEventTypeEventId] = std::set<PajeField>();
  optionalFields[PajeDefineEventTypeEventId].insert (PAJE_Alias);
  optionalFields[PajeDefineEventTypeEventId].insert (PAJE_Color);

  optionalFields[PajeDefineStateTypeEventId] = std::set<PajeField>();
  optionalFields[PajeDefineStateTypeEventId].insert (PAJE_Alias);

  optionalFields[PajeDefineVariableTypeEventId]  = std::set<PajeField>();
  optionalFields[PajeDefineVariableTypeEventId].insert (PAJE_Alias);
  optionalFields[PajeDefineVariableTypeEventId].insert (PAJE_Color);

  optionalFields[PajeDefineLinkTypeEventId] = std::set<PajeField>();
  optionalFields[PajeDefineLinkTypeEventId].insert (PAJE_Alias);

  optionalFields[PajeDefineEntityValueEventId] = std::set<PajeField>();
  optionalFields[PajeDefineEntityValueEventId].insert (PAJE_Alias);
  optionalFields[PajeDefineEntityValueEventId].insert (PAJE_Color);

  optionalFields[PajeCreateContainerEventId] = std::set<PajeField>();
  optionalFields[PajeCreateContainerEventId].insert (PAJE_Alias);

  optionalFields[PajeDestroyContainerEventId] = std::set<PajeField>();

  optionalFields[PajeNewEventEventId] = std::set<PajeField>();
  optionalFields[PajeNewEventEventId].insert (PAJE_File);
  optionalFields[PajeNewEventEventId].insert (PAJE_Line);

  optionalFields[PajeSetVariableEventId] = std::set<PajeField>();
  optionalFields[PajeSetVariableEventId].insert (PAJE_File);
  optionalFields[PajeSetVariableEventId].insert (PAJE_Line);

  optionalFields[PajePushStateEventId] = std::set<PajeField>();
  optionalFields[PajePushStateEventId].insert (PAJE_File);
  optionalFields[PajePushStateEventId].insert (PAJE_Line);

  optionalFields[PajePopStateEventId] = std::set<PajeField>();
  optionalFields[PajePopStateEventId].insert (PAJE_File);
  optionalFields[PajePopStateEventId].insert (PAJE_Line);

  optionalFields[PajeResetStateEventId] = std::set<PajeField>();
  optionalFields[PajeResetStateEventId].insert (PAJE_File);
  optionalFields[PajeResetStateEventId].insert (PAJE_Line);

  optionalFields[PajeSetVariableEventId] = std::set<PajeField>();
  optionalFields[PajeAddVariableEventId] = std::set<PajeField>();
  optionalFields[PajeSubVariableEventId] = std::set<PajeField>();

  optionalFields[PajeStartLinkEventId] = std::set<PajeField>();
  optionalFields[PajeStartLinkEventId].insert (PAJE_File);
  optionalFields[PajeStartLinkEventId].insert (PAJE_Line);

  optionalFields[PajeEndLinkEventId] = std::set<PajeField>();
  optionalFields[PajeEndLinkEventId].insert (PAJE_File);
  optionalFields[PajeEndLinkEventId].insert (PAJE_Line);
}

const std::string &PajeDefinitions::eventNameFromID (PajeEventId event)
{
  if (event == PajeUnknownEventId){
    throw PajeDefinitionsException ();
  }
  event_names_dictionary_t::right_map::const_iterator found;
  found = eventNames.right.find (event);
  return found->second;
}

const std::string &PajeDefinitions::fieldNameFromID (PajeField field)
{
  if (field == PAJE_Extra || field == PAJE_Unknown_Field){
    throw PajeDefinitionsException ();
  }
  id_to_field_name_t::iterator found;
  found = idToFieldName.find (field);
  return found->second;
}

const std::string &PajeDefinitions::fieldTypeNameFromID (PajeFieldType type)
{
  if (type == PAJE_unknown_field_type){
    throw PajeDefinitionsException ();
  }
  field_names_type_dictionary_t::right_map::const_iterator found;
  found = fieldTypeNames.right.find (type);
  return found->second;
}

PajeEventId PajeDefinitions::idFromEventName (std::string event)
{
  event_names_dictionary_t::left_map::const_iterator found;
  found = eventNames.left.find (event);
  if (found == eventNames.left.end()){
    return PajeUnknownEventId;
  }
  return found->second;
}

PajeField PajeDefinitions::idFromFieldName (std::string field)
{
  field_name_to_id_t::iterator found;
  found = fieldNameToID.find (field);
  if (found == fieldNameToID.end()){
    return PAJE_Unknown_Field;
  }
  return found->second;
}

PajeFieldType PajeDefinitions::idFromFieldTypeName (std::string type)
{
  field_names_type_dictionary_t::left_map::const_iterator found;
  found = fieldTypeNames.left.find (type);
  if (found == fieldTypeNames.left.end()){
    return PAJE_unknown_field_type;
  }
  return found->second;
}

bool PajeDefinitions::validEvent (PajeEventId event)
{
  if (eventNameFromID (event).size() != 0){
    return true;
  }else{
    return false;
  }
}

bool PajeDefinitions::validField (PajeField field)
{
  if (fieldNameFromID (field).size() != 0){
    return true;
  }else{
    return false;
  }
}

bool PajeDefinitions::validFieldType (PajeFieldType type)
{
  if (fieldTypeNameFromID (type).size() != 0){
    return true;
  }else{
    return false;
  }
}
