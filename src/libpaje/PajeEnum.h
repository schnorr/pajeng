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
#ifndef __PAJE_ENUM_H__
#define __PAJE_ENUM_H__

#define PAJE_MAX_FIELDS 20
typedef struct {
    char *word[PAJE_MAX_FIELDS];
    int word_count;
    long long lineNumber;
} paje_line;

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
  PajeTraceFileEventId,
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
  PAJE_Filename,
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

#endif
