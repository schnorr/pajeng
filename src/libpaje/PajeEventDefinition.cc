/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeEventDefinition.h"

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

std::map<PajeEventId,std::set<std::string> > initObligatoryFields ()
{
  std::map<PajeEventId,std::set<std::string> > ret;
  std::string eventid("EventId");
  std::string time("Time");
  std::string name("Name");
  std::string alias("Alias");
  std::string type("Type");
  std::string container("Container");
  std::string startContainerType("StartContainerType");
  std::string endContainerType("EndContainerType");
  std::string startContainer("StartContainer");
  std::string endContainer("EndContainer");
  std::string color("Color");
  std::string value("Value");
  std::string key("Key");
  std::string file("File");
  std::string line("Line");

  // The obligatory fields
  ret[PajeDefineContainerTypeEventId] = std::set<std::string>();
  ret[PajeDefineContainerTypeEventId].insert (eventid);
  ret[PajeDefineContainerTypeEventId].insert (name);
  ret[PajeDefineContainerTypeEventId].insert (type);

  ret[PajeDefineEventTypeEventId] = std::set<std::string>();
  ret[PajeDefineEventTypeEventId].insert (eventid);
  ret[PajeDefineEventTypeEventId].insert (name);
  ret[PajeDefineEventTypeEventId].insert (type);

  ret[PajeDefineStateTypeEventId] = std::set<std::string>();
  ret[PajeDefineStateTypeEventId].insert (eventid);
  ret[PajeDefineStateTypeEventId].insert (name);
  ret[PajeDefineStateTypeEventId].insert (type);

  ret[PajeDefineVariableTypeEventId]  = std::set<std::string>();
  ret[PajeDefineVariableTypeEventId].insert (eventid);
  ret[PajeDefineVariableTypeEventId].insert (name);
  ret[PajeDefineVariableTypeEventId].insert (type);

  ret[PajeDefineLinkTypeEventId] = std::set<std::string>();
  ret[PajeDefineLinkTypeEventId].insert (eventid);
  ret[PajeDefineLinkTypeEventId].insert (name);
  ret[PajeDefineLinkTypeEventId].insert (type);
  ret[PajeDefineLinkTypeEventId].insert (startContainerType);
  ret[PajeDefineLinkTypeEventId].insert (endContainerType);

  ret[PajeDefineEntityValueEventId] = std::set<std::string>();
  ret[PajeDefineEntityValueEventId].insert (eventid);
  ret[PajeDefineEntityValueEventId].insert (name);
  ret[PajeDefineEntityValueEventId].insert (type);

  ret[PajeCreateContainerEventId] = std::set<std::string>();
  ret[PajeCreateContainerEventId].insert (eventid);
  ret[PajeCreateContainerEventId].insert (time);
  ret[PajeCreateContainerEventId].insert (name);
  ret[PajeCreateContainerEventId].insert (type);
  ret[PajeCreateContainerEventId].insert (container);

  ret[PajeDestroyContainerEventId] = std::set<std::string>();
  ret[PajeDestroyContainerEventId].insert (eventid);
  ret[PajeDestroyContainerEventId].insert (time);
  ret[PajeDestroyContainerEventId].insert (name);
  ret[PajeDestroyContainerEventId].insert (type);

  ret[PajeNewEventEventId] = std::set<std::string>();
  ret[PajeNewEventEventId].insert (eventid);
  ret[PajeNewEventEventId].insert (time);
  ret[PajeNewEventEventId].insert (type);
  ret[PajeNewEventEventId].insert (container);
  ret[PajeNewEventEventId].insert (value);

  ret[PajeSetStateEventId] = std::set<std::string>();
  ret[PajeSetStateEventId].insert (eventid);
  ret[PajeSetStateEventId].insert (time);
  ret[PajeSetStateEventId].insert (type);
  ret[PajeSetStateEventId].insert (container);
  ret[PajeSetStateEventId].insert (value);

  ret[PajePushStateEventId] = std::set<std::string>();
  ret[PajePushStateEventId].insert (eventid);
  ret[PajePushStateEventId].insert (time);
  ret[PajePushStateEventId].insert (type);
  ret[PajePushStateEventId].insert (container);
  ret[PajePushStateEventId].insert (value);

  ret[PajePopStateEventId] = std::set<std::string>();
  ret[PajePopStateEventId].insert (eventid);
  ret[PajePopStateEventId].insert (time);
  ret[PajePopStateEventId].insert (type);
  ret[PajePopStateEventId].insert (container);

  ret[PajeResetStateEventId] = std::set<std::string>();
  ret[PajeResetStateEventId].insert (eventid);
  ret[PajeResetStateEventId].insert (time);
  ret[PajeResetStateEventId].insert (type);
  ret[PajeResetStateEventId].insert (container);

  ret[PajeSetVariableEventId] = std::set<std::string>();
  ret[PajeSetVariableEventId].insert (eventid);
  ret[PajeSetVariableEventId].insert (time);
  ret[PajeSetVariableEventId].insert (type);
  ret[PajeSetVariableEventId].insert (container);
  ret[PajeSetVariableEventId].insert (value);

  ret[PajeAddVariableEventId] = std::set<std::string>();
  ret[PajeAddVariableEventId].insert (eventid);
  ret[PajeAddVariableEventId].insert (time);
  ret[PajeAddVariableEventId].insert (type);
  ret[PajeAddVariableEventId].insert (container);
  ret[PajeAddVariableEventId].insert (value);

  ret[PajeSubVariableEventId] = std::set<std::string>();
  ret[PajeSubVariableEventId].insert (eventid);
  ret[PajeSubVariableEventId].insert (time);
  ret[PajeSubVariableEventId].insert (type);
  ret[PajeSubVariableEventId].insert (container);
  ret[PajeSubVariableEventId].insert (value);

  ret[PajeStartLinkEventId] = std::set<std::string>();
  ret[PajeStartLinkEventId].insert (eventid);
  ret[PajeStartLinkEventId].insert (time);
  ret[PajeStartLinkEventId].insert (type);
  ret[PajeStartLinkEventId].insert (container);
  ret[PajeStartLinkEventId].insert (value);
  ret[PajeStartLinkEventId].insert (startContainer);
  ret[PajeStartLinkEventId].insert (key);

  ret[PajeEndLinkEventId] = std::set<std::string>();
  ret[PajeEndLinkEventId].insert (eventid);
  ret[PajeEndLinkEventId].insert (time);
  ret[PajeEndLinkEventId].insert (type);
  ret[PajeEndLinkEventId].insert (container);
  ret[PajeEndLinkEventId].insert (value);
  ret[PajeEndLinkEventId].insert (endContainer);
  ret[PajeEndLinkEventId].insert (key);

  return ret;
}

std::map<PajeEventId,std::set<std::string> > initOptionalFields ()
{
  std::map<PajeEventId,std::set<std::string> > ret;
  std::string alias = "Alias";
  std::string color = "Color";
  std::string line = "Line";
  std::string file = "File";

  // The optional fields
  ret[PajeDefineContainerTypeEventId] = std::set<std::string>();
  ret[PajeDefineContainerTypeEventId].insert (alias);

  ret[PajeDefineEventTypeEventId] = std::set<std::string>();
  ret[PajeDefineEventTypeEventId].insert (alias);

  ret[PajeDefineStateTypeEventId] = std::set<std::string>();
  ret[PajeDefineStateTypeEventId].insert (alias);

  ret[PajeDefineVariableTypeEventId]  = std::set<std::string>();
  ret[PajeDefineVariableTypeEventId].insert (alias);

  ret[PajeDefineLinkTypeEventId] = std::set<std::string>();
  ret[PajeDefineLinkTypeEventId].insert (alias);

  ret[PajeDefineEntityValueEventId] = std::set<std::string>();
  ret[PajeDefineEntityValueEventId].insert (alias);
  ret[PajeDefineEntityValueEventId].insert (color);

  ret[PajeCreateContainerEventId] = std::set<std::string>();
  ret[PajeCreateContainerEventId].insert (alias);

  ret[PajeDestroyContainerEventId] = std::set<std::string>();

  ret[PajeNewEventEventId] = std::set<std::string>();
  ret[PajeNewEventEventId].insert (file);
  ret[PajeNewEventEventId].insert (line);

  ret[PajeSetVariableEventId] = std::set<std::string>();
  ret[PajeSetVariableEventId].insert (file);
  ret[PajeSetVariableEventId].insert (line);

  ret[PajePushStateEventId] = std::set<std::string>();
  ret[PajePushStateEventId].insert (file);
  ret[PajePushStateEventId].insert (line);

  ret[PajePopStateEventId] = std::set<std::string>();
  ret[PajePopStateEventId].insert (file);
  ret[PajePopStateEventId].insert (line);

  ret[PajeResetStateEventId] = std::set<std::string>();
  ret[PajeResetStateEventId].insert (file);
  ret[PajeResetStateEventId].insert (line);

  ret[PajeSetVariableEventId] = std::set<std::string>();
  ret[PajeAddVariableEventId] = std::set<std::string>();
  ret[PajeSubVariableEventId] = std::set<std::string>();

  ret[PajeStartLinkEventId] = std::set<std::string>();
  ret[PajeStartLinkEventId].insert (file);
  ret[PajeStartLinkEventId].insert (line);

  ret[PajeEndLinkEventId] = std::set<std::string>();
  ret[PajeEndLinkEventId].insert (file);
  ret[PajeEndLinkEventId].insert (line);

  return ret;
}

static std::map<PajeEventId,std::set<std::string> > pajeObligatoryFields = initObligatoryFields ();
static std::map<PajeEventId,std::set<std::string> > pajeOptionalFields = initOptionalFields ();

PajeEventDefinition::PajeEventDefinition (PajeEventId pajeEventId, std::string number)
{
  this->pajeEventId = pajeEventId;
  this->number = number;
  fieldCount = 0;
  // extraFieldCount = 0;

  //the first field is always the event identification
  PajeEventDefinition::addField ("EventId", "int");
}

PajeEventDefinition::~PajeEventDefinition (void)
{
}

void PajeEventDefinition::addField (std::string name, std::string type)
{
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
    throw "The type '"+type+"' used in the field '"+name+"' is not recognized.";
  }else{
    set.clear();
  }

  //check if the name has already being registered
  std::list<std::string>::iterator it;
  for (it = fields.begin(); it != fields.end(); it++){
    set.insert (*it);
  }
  found = set.find (name);
  if (found != set.end()){
    throw "The field '"+name+"' with type '"+type+"' has been previously defined.";
  }else{
    set.clear();
  }

  //add the field and its type
  fields.push_back (name);
  types.push_back (type);
  fieldCount++;
  return;
}

int PajeEventDefinition::indexForFieldId (std::string name)
{
  int i;
  std::list<std::string>::iterator it;
  for (i = 0, it = fields.begin(); it != fields.end(); i++, it++){
    if (name == *it) return i;
  }
  if (i == fields.size()) return -1;
  else return i;
}

bool PajeEventDefinition::isValid (void)
{
  //get the obligatory fields for my pajeEventId
  std::map<PajeEventId,std::set<std::string> >::iterator itmap;
  itmap = pajeObligatoryFields.find (pajeEventId);
  if (itmap == pajeObligatoryFields.end()){
    return false;
  }

  std::set<std::string> obligatory = itmap->second;

  //iterate through the set of obligatory fields for my pajeEventId
  std::set<std::string>::iterator itset;
  for (itset = obligatory.begin (); itset != obligatory.end(); itset++){
    std::string obligatoryField = *itset;

    //search each  the obligatory fields in the fields list of this definition
    bool myFieldIsPresent = false;
    std::list<std::string>::iterator itlist;
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

void PajeEventDefinition::showObligatoryFields (void)
{
  std::map<PajeEventId,std::string> eventNames = initPajeEventIDToNames();
  std::cout << "Obligatory fields expected for a " << eventNames[pajeEventId]
            << " event definition:" << std::endl;
  std::set<std::string>::iterator iter;
  iter = pajeObligatoryFields[pajeEventId].begin();
  while (iter != pajeObligatoryFields[pajeEventId].end()){
    std::cout << *iter << " ";
    iter++;
  }
  std::cout << std::endl;
}

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef)
{
  PajeEventDefinition def = (PajeEventDefinition)eventDef;
  std::map<PajeEventId,std::string> eventNames = initPajeEventIDToNames();
 
  output << "  %EventDef " << eventNames[def.pajeEventId] << " " << def.number << std::endl;
  std::list<std::string>::iterator itf = def.fields.begin();
  std::list<std::string>::iterator itt = def.types.begin();

  //don't print the first field (it is the event type)
  itf++;
  itt++;

  //print
  while (itf != def.fields.end()){
    output << "  %    " << *itf << " " << *itt << std::endl;
    itf++;
    itt++;
  }
  output << "  %EndEventDef";
  return output;
}
