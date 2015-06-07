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

PajeEventDefinition::PajeEventDefinition (PajeEventId id, int unique, int line, PajeDefinitions *definitions)
{
  defs = definitions;
  pajeEventIdentifier = id;
  uniqueIdentifier = unique;
  //the first field is always the event identification
  addField (PAJE_Event, PAJE_int, line);
}

PajeEventDefinition::~PajeEventDefinition (void)
{
}

void PajeEventDefinition::addField (PajeField field, PajeFieldType type, int line)
{
  std::stringstream st;
  st << line;

  if (!defs->validFieldType(type)){
    throw PajeDecodeException ("The type used in the field '" + defs->fieldNameFromID(field) + "' is not recognized in line " + st.str());
  }

  //check if field is already defined
  std::set<PajeField> fieldSet (fields.begin(), fields.end());
  std::set<PajeField>::iterator present = fieldSet.find(field);
  if (present != fieldSet.end() && field != PAJE_Extra){
    throw PajeDecodeException ("The field '" + defs->fieldNameFromID(field) + "' with type '" + defs->fieldTypeNameFromID(type)+"' is already defined when treating line "+st.str());
  }

  fields.push_back (field);
  types.push_back (type);
}

void PajeEventDefinition::addField (PajeField field, PajeFieldType type, int line, std::string userDefinedFieldName)
{
  if (field == PAJE_Extra){
    userDefinedFieldNames.push_back (userDefinedFieldName);
  }
  addField (field, type, line);
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

int PajeEventDefinition::indexForExtraFieldNamed (std::string fieldName)
{
  int i;
  std::vector<std::string>::iterator it;
  for (i = 0, it = userDefinedFieldNames.begin(); it != userDefinedFieldNames.end(); i++, it++){
    if (fieldName == *it) break;
  }
  if (i == fields.size()){
    throw PajeDecodeException ("The user-defined field '"+fieldName+"' was not found in event "+ defs->eventNameFromID(pajeEventIdentifier));
  }

  //we are now looking for the i-th PAJE_Extra field in the fields (the attribute) list
  int j = 0, k;
  std::list<PajeField>::iterator field_iterator;
  for (k = 0, field_iterator = fields.begin(); field_iterator != fields.end(); field_iterator++, k++){
    if (*field_iterator == PAJE_Extra){
      if (j == i) return k;
      j++;
    }
  }
  throw PajeDecodeException ("The user-defined field '"+fieldName+"' was found, but there is no correspondence in the fields vector of event "+ defs->eventNameFromID(pajeEventIdentifier));
}

bool PajeEventDefinition::isValid (void)
{
  //get the obligatory fields for my pajeEventIdentifier
  std::map<PajeEventId,std::set<PajeField> >::iterator itmap;
  itmap = defs->obligatoryFields.find (pajeEventIdentifier);
  if (itmap == defs->obligatoryFields.end()){
    return false;
  }

  std::set<PajeField> obligatory = itmap->second;

  //iterate through the set of obligatory fields for my pajeEventIdentifier
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
  std::cout << "Obligatory fields expected for a " << defs->eventNameFromID(pajeEventIdentifier)
            << " event definition:" << std::endl;
  std::set<PajeField>::iterator iter;
  iter = defs->obligatoryFields[pajeEventIdentifier].begin();
  while (iter != defs->obligatoryFields[pajeEventIdentifier].end()){
    std::cout << defs->fieldNameFromID(*iter) << " ";
    iter++;
  }
  std::cout << std::endl;
}

std::vector<std::string> PajeEventDefinition::extraFields (void)
{
  return userDefinedFieldNames;
}

bool PajeEventDefinition::knownFieldNamed (std::string name)
{
  return defs->validField (defs->idFromFieldName (name));
}

std::ostream &operator<< (std::ostream &output, const PajeEventDefinition &eventDef)
{
  PajeEventDefinition def = (PajeEventDefinition)eventDef;
 
  output << "This is the event definition of the problematic event:" << std::endl;
  output << "  %EventDef " << def.defs->eventNameFromID(def.pajeEventIdentifier) << " " << def.uniqueIdentifier << std::endl;
  std::list<PajeField>::iterator itf = def.fields.begin();
  std::list<PajeFieldType>::iterator itt = def.types.begin();

  //don't print the first field (it is the event type)
  itf++;
  itt++;

  //print
  int extraIndex = 0;
  while (itf != def.fields.end()){
    if (*itf != PAJE_Extra){
      output << "  %    " << def.defs->fieldNameFromID(*itf) << " ";
    }else{
      output << "  %    " << def.userDefinedFieldNames.at(extraIndex++) << " ";;
    }
    output <<  def.defs->fieldTypeNameFromID(*itt) << std::endl;

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
  output << ", Fields: " << line.word_count;
  output << ", Contents: '";
  for (i = 0; i < line.word_count; i++){
    output << std::string(line.word[i]);
    if (i+1 != line.word_count) output << " ";
  }
  output << "')";
  return output;
}
