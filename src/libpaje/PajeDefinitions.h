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
#ifndef __PAJE_DEFINITIONS_H__
#define __PAJE_DEFINITIONS_H__
#include <boost/bimap.hpp>
#include <string>
#include <map>

typedef enum {
  PajeDefineContainerTypeEventId,
  PajeDefineEventTypeEventId,
  PajeDefineStateTypeEventId,
  PajeDefineVariableTypeEventId,
  PajeDefineLinkTypeEventId,
  PajeDefineEntityValueEventId,
  PajeCreateContainerEventId,
  PajeDestroyContainerEventId,
  PajeNewEventEventId,
  PajeSetStateEventId,
  PajePushStateEventId,
  PajePopStateEventId,
  PajeResetStateEventId,
  PajeSetVariableEventId,
  PajeAddVariableEventId,
  PajeSubVariableEventId,
  PajeStartLinkEventId,
  PajeEndLinkEventId,
  PajeEventIdCount,
  PajeUnknownEventId,
} PajeEventId;

typedef enum {
  PAJE_Event,
  PAJE_Time,
  PAJE_Name,
  PAJE_Type,
  PAJE_Container,
  PAJE_StartContainerType,
  PAJE_EndContainerType,
  PAJE_StartContainer,
  PAJE_EndContainer,
  PAJE_Value,
  PAJE_Key,
  PAJE_Alias,
  PAJE_Color,
  PAJE_Line,
  PAJE_File,
  PAJE_Extra, //for user-defined fields
  PAJE_Unknown_Field
} PajeField;

typedef enum {
  PAJE_string,
  PAJE_float,
  PAJE_double,
  PAJE_int,
  PAJE_hex,
  PAJE_date,
  PAJE_color,
  PAJE_unknown_field_type
} PajeFieldType;

typedef boost::bimap<std::string,PajeEventId> event_names_dictionary_t;
typedef boost::bimap<std::string,PajeFieldType> field_names_type_dictionary_t;
typedef std::map<std::string,PajeField> field_name_to_id_t;
typedef std::map<PajeField,std::string> id_to_field_name_t;

class PajeDefinitions {
 private:
  field_name_to_id_t fieldNameToID;
  id_to_field_name_t idToFieldName;

  event_names_dictionary_t eventNames;
  field_names_type_dictionary_t fieldTypeNames;
 public:
  std::map<PajeEventId,std::set<PajeField> > obligatoryFields;
  std::map<PajeEventId,std::set<PajeField> > optionalFields;

 public:
  PajeDefinitions (bool strict);
  ~PajeDefinitions ();

  std::string eventNameFromID (PajeEventId event);
  std::string fieldNameFromID (PajeField field);
  std::string fieldTypeNameFromID (PajeFieldType type);

  PajeEventId idFromEventName (std::string event);
  PajeField idFromFieldName (std::string field);
  PajeFieldType idFromFieldTypeName (std::string type);

  bool validEvent (PajeEventId event);
  bool validField (PajeField field);
  bool validFieldType (PajeFieldType field);

 private:
  void initialize (bool strict);
};

#endif
