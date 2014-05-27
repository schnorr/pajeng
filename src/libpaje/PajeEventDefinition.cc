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
#include <boost/lexical_cast.hpp>
#include "PajeEventDefinition.h"
#include "PajeException.h"

std::map<std::string,PajeEventId> initPajeEventNamesToID ()
{
  std::map<std::string,PajeEventId> ret;
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
  ret["PajeResetState"] = PajeResetStateEventId;
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
  ret[PajeResetStateEventId] = "PajeResetState";
  ret[PajeSetVariableEventId] = "PajeSetVariable";
  ret[PajeAddVariableEventId] = "PajeAddVariable";
  ret[PajeSubVariableEventId] = "PajeSubVariable";
  ret[PajeStartLinkEventId] = "PajeStartLink";
  ret[PajeEndLinkEventId] = "PajeEndLink";
  return ret;
}

std::map<std::string,PajeField> initPajeFieldNamesToID (bool strict)
{
  std::map<std::string,PajeField> ret;
  ret["Event"] = PAJE_Event;
  ret["Time"] = PAJE_Time;
  ret["Name"] = PAJE_Name;
  ret["Type"] = PAJE_Type;
  ret["Container"] = PAJE_Container;
  ret["StartContainerType"] = PAJE_StartContainerType;
  ret["EndContainerType"] = PAJE_EndContainerType;
  ret["StartContainer"] = PAJE_StartContainer;
  ret["EndContainer"] = PAJE_EndContainer;
  ret["Value"] = PAJE_Value;
  ret["Key"] = PAJE_Key;
  ret["Alias"] = PAJE_Alias;
  ret["Color"] = PAJE_Color;
  ret["Line"] = PAJE_Line;
  ret["File"] = PAJE_File;

  //old field names
  if (!strict){
    ret["ContainerType"] = PAJE_Type;
    ret["EntityType"] = PAJE_Type;
    ret["SourceContainerType"] = PAJE_StartContainerType;
    ret["DestContainerType"] = PAJE_EndContainerType;
    ret["SourceContainer"] = PAJE_StartContainer;
    ret["DestContainer"] = PAJE_EndContainer;
  }
  return ret;
}

std::map<PajeField,std::string> initPajeFieldIDToNames (bool strict)
{
  std::map<std::string,PajeField> input = initPajeFieldNamesToID(strict);
  std::map<PajeField,std::string> ret;
  std::map<std::string,PajeField>::iterator it;
  for (it = input.begin(); it != input.end(); it++){
    ret[(*it).second] = (*it).first;
  }
  return ret;
}

std::map<PajeEventId,std::set<PajeField> > initObligatoryFields ()
{
  std::map<PajeEventId,std::set<PajeField> > ret;

  // the obligatory fields
  ret[PajeDefineContainerTypeEventId] = std::set<PajeField>();
  ret[PajeDefineContainerTypeEventId].insert (PAJE_Event);
  ret[PajeDefineContainerTypeEventId].insert (PAJE_Name);
  ret[PajeDefineContainerTypeEventId].insert (PAJE_Type);

  ret[PajeDefineEventTypeEventId] = std::set<PajeField>();
  ret[PajeDefineEventTypeEventId].insert (PAJE_Event);
  ret[PajeDefineEventTypeEventId].insert (PAJE_Name);
  ret[PajeDefineEventTypeEventId].insert (PAJE_Type);

  ret[PajeDefineStateTypeEventId] = std::set<PajeField>();
  ret[PajeDefineStateTypeEventId].insert (PAJE_Event);
  ret[PajeDefineStateTypeEventId].insert (PAJE_Name);
  ret[PajeDefineStateTypeEventId].insert (PAJE_Type);

  ret[PajeDefineVariableTypeEventId]  = std::set<PajeField>();
  ret[PajeDefineVariableTypeEventId].insert (PAJE_Event);
  ret[PajeDefineVariableTypeEventId].insert (PAJE_Name);
  ret[PajeDefineVariableTypeEventId].insert (PAJE_Type);

  ret[PajeDefineLinkTypeEventId] = std::set<PajeField>();
  ret[PajeDefineLinkTypeEventId].insert (PAJE_Event);
  ret[PajeDefineLinkTypeEventId].insert (PAJE_Name);
  ret[PajeDefineLinkTypeEventId].insert (PAJE_Type);
  ret[PajeDefineLinkTypeEventId].insert (PAJE_StartContainerType);
  ret[PajeDefineLinkTypeEventId].insert (PAJE_EndContainerType);

  ret[PajeDefineEntityValueEventId] = std::set<PajeField>();
  ret[PajeDefineEntityValueEventId].insert (PAJE_Event);
  ret[PajeDefineEntityValueEventId].insert (PAJE_Name);
  ret[PajeDefineEntityValueEventId].insert (PAJE_Type);

  ret[PajeCreateContainerEventId] = std::set<PajeField>();
  ret[PajeCreateContainerEventId].insert (PAJE_Event);
  ret[PajeCreateContainerEventId].insert (PAJE_Time);
  ret[PajeCreateContainerEventId].insert (PAJE_Name);
  ret[PajeCreateContainerEventId].insert (PAJE_Type);
  ret[PajeCreateContainerEventId].insert (PAJE_Container);

  ret[PajeDestroyContainerEventId] = std::set<PajeField>();
  ret[PajeDestroyContainerEventId].insert (PAJE_Event);
  ret[PajeDestroyContainerEventId].insert (PAJE_Time);
  ret[PajeDestroyContainerEventId].insert (PAJE_Name);
  ret[PajeDestroyContainerEventId].insert (PAJE_Type);

  ret[PajeNewEventEventId] = std::set<PajeField>();
  ret[PajeNewEventEventId].insert (PAJE_Event);
  ret[PajeNewEventEventId].insert (PAJE_Time);
  ret[PajeNewEventEventId].insert (PAJE_Type);
  ret[PajeNewEventEventId].insert (PAJE_Container);
  ret[PajeNewEventEventId].insert (PAJE_Value);

  ret[PajeSetStateEventId] = std::set<PajeField>();
  ret[PajeSetStateEventId].insert (PAJE_Event);
  ret[PajeSetStateEventId].insert (PAJE_Time);
  ret[PajeSetStateEventId].insert (PAJE_Type);
  ret[PajeSetStateEventId].insert (PAJE_Container);
  ret[PajeSetStateEventId].insert (PAJE_Value);

  ret[PajePushStateEventId] = std::set<PajeField>();
  ret[PajePushStateEventId].insert (PAJE_Event);
  ret[PajePushStateEventId].insert (PAJE_Time);
  ret[PajePushStateEventId].insert (PAJE_Type);
  ret[PajePushStateEventId].insert (PAJE_Container);
  ret[PajePushStateEventId].insert (PAJE_Value);

  ret[PajePopStateEventId] = std::set<PajeField>();
  ret[PajePopStateEventId].insert (PAJE_Event);
  ret[PajePopStateEventId].insert (PAJE_Time);
  ret[PajePopStateEventId].insert (PAJE_Type);
  ret[PajePopStateEventId].insert (PAJE_Container);

  ret[PajeResetStateEventId] = std::set<PajeField>();
  ret[PajeResetStateEventId].insert (PAJE_Event);
  ret[PajeResetStateEventId].insert (PAJE_Time);
  ret[PajeResetStateEventId].insert (PAJE_Type);
  ret[PajeResetStateEventId].insert (PAJE_Container);

  ret[PajeSetVariableEventId] = std::set<PajeField>();
  ret[PajeSetVariableEventId].insert (PAJE_Event);
  ret[PajeSetVariableEventId].insert (PAJE_Time);
  ret[PajeSetVariableEventId].insert (PAJE_Type);
  ret[PajeSetVariableEventId].insert (PAJE_Container);
  ret[PajeSetVariableEventId].insert (PAJE_Value);

  ret[PajeAddVariableEventId] = std::set<PajeField>();
  ret[PajeAddVariableEventId].insert (PAJE_Event);
  ret[PajeAddVariableEventId].insert (PAJE_Time);
  ret[PajeAddVariableEventId].insert (PAJE_Type);
  ret[PajeAddVariableEventId].insert (PAJE_Container);
  ret[PajeAddVariableEventId].insert (PAJE_Value);

  ret[PajeSubVariableEventId] = std::set<PajeField>();
  ret[PajeSubVariableEventId].insert (PAJE_Event);
  ret[PajeSubVariableEventId].insert (PAJE_Time);
  ret[PajeSubVariableEventId].insert (PAJE_Type);
  ret[PajeSubVariableEventId].insert (PAJE_Container);
  ret[PajeSubVariableEventId].insert (PAJE_Value);

  ret[PajeStartLinkEventId] = std::set<PajeField>();
  ret[PajeStartLinkEventId].insert (PAJE_Event);
  ret[PajeStartLinkEventId].insert (PAJE_Time);
  ret[PajeStartLinkEventId].insert (PAJE_Type);
  ret[PajeStartLinkEventId].insert (PAJE_Container);
  ret[PajeStartLinkEventId].insert (PAJE_Value);
  ret[PajeStartLinkEventId].insert (PAJE_StartContainer);
  ret[PajeStartLinkEventId].insert (PAJE_Key);

  ret[PajeEndLinkEventId] = std::set<PajeField>();
  ret[PajeEndLinkEventId].insert (PAJE_Event);
  ret[PajeEndLinkEventId].insert (PAJE_Time);
  ret[PajeEndLinkEventId].insert (PAJE_Type);
  ret[PajeEndLinkEventId].insert (PAJE_Container);
  ret[PajeEndLinkEventId].insert (PAJE_Value);
  ret[PajeEndLinkEventId].insert (PAJE_EndContainer);
  ret[PajeEndLinkEventId].insert (PAJE_Key);

  return ret;
}

std::map<PajeEventId,std::set<PajeField> > initOptionalFields ()
{
  std::map<PajeEventId,std::set<PajeField> > ret;

  // The optional fields
  ret[PajeDefineContainerTypeEventId] = std::set<PajeField>();
  ret[PajeDefineContainerTypeEventId].insert (PAJE_Alias);

  ret[PajeDefineEventTypeEventId] = std::set<PajeField>();
  ret[PajeDefineEventTypeEventId].insert (PAJE_Alias);
  ret[PajeDefineEventTypeEventId].insert (PAJE_Color);

  ret[PajeDefineStateTypeEventId] = std::set<PajeField>();
  ret[PajeDefineStateTypeEventId].insert (PAJE_Alias);

  ret[PajeDefineVariableTypeEventId]  = std::set<PajeField>();
  ret[PajeDefineVariableTypeEventId].insert (PAJE_Alias);
  ret[PajeDefineVariableTypeEventId].insert (PAJE_Color);

  ret[PajeDefineLinkTypeEventId] = std::set<PajeField>();
  ret[PajeDefineLinkTypeEventId].insert (PAJE_Alias);

  ret[PajeDefineEntityValueEventId] = std::set<PajeField>();
  ret[PajeDefineEntityValueEventId].insert (PAJE_Alias);
  ret[PajeDefineEntityValueEventId].insert (PAJE_Color);

  ret[PajeCreateContainerEventId] = std::set<PajeField>();
  ret[PajeCreateContainerEventId].insert (PAJE_Alias);

  ret[PajeDestroyContainerEventId] = std::set<PajeField>();

  ret[PajeNewEventEventId] = std::set<PajeField>();
  ret[PajeNewEventEventId].insert (PAJE_File);
  ret[PajeNewEventEventId].insert (PAJE_Line);

  ret[PajeSetVariableEventId] = std::set<PajeField>();
  ret[PajeSetVariableEventId].insert (PAJE_File);
  ret[PajeSetVariableEventId].insert (PAJE_Line);

  ret[PajePushStateEventId] = std::set<PajeField>();
  ret[PajePushStateEventId].insert (PAJE_File);
  ret[PajePushStateEventId].insert (PAJE_Line);

  ret[PajePopStateEventId] = std::set<PajeField>();
  ret[PajePopStateEventId].insert (PAJE_File);
  ret[PajePopStateEventId].insert (PAJE_Line);

  ret[PajeResetStateEventId] = std::set<PajeField>();
  ret[PajeResetStateEventId].insert (PAJE_File);
  ret[PajeResetStateEventId].insert (PAJE_Line);

  ret[PajeSetVariableEventId] = std::set<PajeField>();
  ret[PajeAddVariableEventId] = std::set<PajeField>();
  ret[PajeSubVariableEventId] = std::set<PajeField>();

  ret[PajeStartLinkEventId] = std::set<PajeField>();
  ret[PajeStartLinkEventId].insert (PAJE_File);
  ret[PajeStartLinkEventId].insert (PAJE_Line);

  ret[PajeEndLinkEventId] = std::set<PajeField>();
  ret[PajeEndLinkEventId].insert (PAJE_File);
  ret[PajeEndLinkEventId].insert (PAJE_Line);

  return ret;
}

static std::map<PajeEventId,std::set<PajeField> > pajeObligatoryFields = initObligatoryFields ();
static std::map<PajeEventId,std::set<PajeField> > pajeOptionalFields = initOptionalFields ();

PajeEventDefinition::PajeEventDefinition (PajeEventId pajeEventId, int number, paje_line *line, bool strictHeader)
{
  this->pajeEventId = pajeEventId;
  this->number = number;
  strictDefinition = strictHeader;

  //the first field is always the event identification
  PajeEventDefinition::addField ("Event", "int", line);
}

PajeEventDefinition::~PajeEventDefinition (void)
{
}

void PajeEventDefinition::addField (std::string name, std::string type, paje_line *line)
{
  std::stringstream st;
  st << *line;
  std::string lreport = st.str();

  //check if the type is valid
  std::set<std::string> set;
  set.insert("int");
  set.insert("hex");
  set.insert("date");
  set.insert("double");
  set.insert("string");
  set.insert("color");
  std::set<std::string>::iterator found;
  found = set.find (type);
  if (found == set.end()){
    throw PajeDecodeException ("The type '"+type+"' used in the field '"+name+"' is not recognized in line "+lreport);
  }else{
    set.clear();
  }

  //if we do NOT know this field name, it is an extra field name
  PajeField newField;
  if (!knownFieldNamed (name)){
    extraFieldNames.push_back (name);
    newField = PAJE_Extra;
  }else{
    //check if the new field is already defined
    newField = initPajeFieldNamesToID(strictDefinition)[name];
    std::set<PajeField> fieldSet (fields.begin(), fields.end());
    std::set<PajeField>::iterator present = fieldSet.find(newField);
    if (present != fieldSet.end()){
      std::stringstream st;
      st << line->lineNumber;
      throw PajeDecodeException ("The field '"+name+"' with type '"+type+"' is already defined when treating line "+lreport);    
    }
  }
  fields.push_back (newField);
  names.push_back (name);
  types.push_back (type);
  return;
}

int PajeEventDefinition::indexForField (PajeField field)
{
  int i;
  std::list<PajeField>::iterator it;
  for (i = 0, it = fields.begin(); it != fields.end(); i++, it++){
    if (field == *it) return i;
  }
  if (i == fields.size()) return -1;
  else return i;
}

int PajeEventDefinition::indexForField (std::string fieldName)
{
  int i;
  std::list<std::string>::iterator it;
  for (i = 0, it = names.begin(); it != names.end(); i++, it++){
    if (fieldName == *it) return i;
  }
  if (i == fields.size()) return -1;
  else return i;
}

bool PajeEventDefinition::isValid (void)
{
  //get the obligatory fields for my pajeEventId
  std::map<PajeEventId,std::set<PajeField> >::iterator itmap;
  itmap = pajeObligatoryFields.find (pajeEventId);
  if (itmap == pajeObligatoryFields.end()){
    return false;
  }

  std::set<PajeField> obligatory = itmap->second;

  //iterate through the set of obligatory fields for my pajeEventId
  std::set<PajeField>::iterator itset;
  for (itset = obligatory.begin (); itset != obligatory.end(); itset++){
    PajeField obligatoryField = *itset;

    //search each  the obligatory fields in the fields list of this definition
    bool myFieldIsPresent = false;
    std::list<PajeField>::iterator itlist;
    for (itlist = fields.begin(); itlist != fields.end(); itlist++){
      if (*itlist == obligatoryField){
        myFieldIsPresent = true;
        break;
      }
    }
    if (!myFieldIsPresent){
      return false;
    }
  }
  return true;
}

int PajeEventDefinition::fieldCount (void)
{
  return fields.size();
}

void PajeEventDefinition::showObligatoryFields (void)
{
  std::map<PajeEventId,std::string> eventNames = initPajeEventIDToNames();
  std::map<PajeField,std::string> fieldNames = initPajeFieldIDToNames(strictDefinition);
  std::cout << "Obligatory fields expected for a " << eventNames[pajeEventId]
            << " event definition:" << std::endl;
  std::set<PajeField>::iterator iter;
  iter = pajeObligatoryFields[pajeEventId].begin();
  while (iter != pajeObligatoryFields[pajeEventId].end()){
    std::cout << fieldNames[*iter] << " ";
    iter++;
  }
  std::cout << std::endl;
}

std::vector<std::string> PajeEventDefinition::extraFields (void)
{
  return extraFieldNames;
}

bool PajeEventDefinition::knownFieldNamed (std::string name)
{
  std::map<std::string,PajeField> knownFields = initPajeFieldNamesToID(strictDefinition);
  std::map<std::string,PajeField>::iterator found = knownFields.find (name);
  if (found == knownFields.end()){
    return false;
  }else{
    return true;
  }
}

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef)
{
  PajeEventDefinition def = (PajeEventDefinition)eventDef;
  std::map<PajeEventId,std::string> eventNames = initPajeEventIDToNames();
 
  output << "  %EventDef " << eventNames[def.pajeEventId] << " " << def.number << std::endl;
  std::list<std::string>::iterator itf = def.names.begin();
  std::list<std::string>::iterator itt = def.types.begin();

  //don't print the first field (it is the event type)
  itf++;
  itt++;

  //print
  while (itf != def.names.end()){
    output << "  %    " << *itf << " " << *itt << std::endl;
    itf++;
    itt++;
  }
  output << "  %EndEventDef";
  return output;
}

std::ostream &operator<< (std::ostream &output, const paje_line &line)
{
  int i;
  output << "(Line: " << line.lineNumber;
  output << ", Contents: '";
  for (i = 0; i < line.word_count; i++){
    output << std::string(line.word[i]);
    if (i+1 != line.word_count) output << " ";
  }
  output << "')";
  return output;
}
