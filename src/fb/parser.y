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
%{
#include <stdio.h>
#include <search.h>
#include "PajeEventDefinition.h"
#include "PajeFlexReader.h"
#include "PajeDefinitions.h"

  PajeEventDefinition *eventBeingDefined;
  PajeDefinitions *globalDefinitions;

  extern "C"
  {
    extern FILE *yyin;
    extern int yylineno;
    int yylex(void);
    void yyerror (char const *mensagem);
    int yyparse ();
  }

  static void lineReset ();
  static void lineDef (int identifier);
  static void lineAdd (char *str);
  static void lineSend ();

  PajeEventDefinition **defsv;
  int defsv_current_size;
  PajeEventDefinition *def;
  PajeFlexReader *flexReader;

  PajeTraceEvent *event; //the current paje trace event (a line) being read
%}

%union {
  PajeEventId eventId;
  PajeField fieldId;
  struct Field_Name_Info {
    PajeField fieldId;
    char *fieldName;
  } field_data;
  PajeFieldType fieldType;
  int eventCode;
  struct Argument_Info {
    union {
      float floatValue;
      int intValue;
    };
    char *str;
  } argument_data;
}

%token TK_EVENT_DEF_BEGIN
%token TK_EVENT_DEF
%token TK_EVENT_DEF_END
%token TK_EVENT_DEF_ALIAS
%token TK_EVENT_DEF_TYPE
%token TK_EVENT_DEF_NAME
%token TK_EVENT_DEF_COLOR
%token TK_EVENT_DEF_START_CONTAINER_TYPE
%token TK_EVENT_DEF_END_CONTAINER_TYPE
%token TK_EVENT_DEF_CONTAINER
%token TK_EVENT_DEF_TIME
%token TK_EVENT_DEF_START_CONTAINER
%token TK_EVENT_DEF_END_CONTAINER
%token TK_EVENT_DEF_VALUE
%token TK_EVENT_DEF_KEY
%token TK_EVENT_DEF_LINE
%token TK_EVENT_DEF_FILE
%token TK_EVENT_DEF_FIELD_TYPE_STRING
%token TK_EVENT_DEF_FIELD_TYPE_FLOAT
%token TK_EVENT_DEF_FIELD_TYPE_DOUBLE
%token TK_EVENT_DEF_FIELD_TYPE_INT
%token TK_EVENT_DEF_FIELD_TYPE_HEX
%token TK_EVENT_DEF_FIELD_TYPE_DATE
%token TK_EVENT_DEF_FIELD_TYPE_COLOR
%token TK_PAJE_DEFINE_CONTAINER_TYPE
%token TK_PAJE_DEFINE_VARIABLE_TYPE
%token TK_PAJE_DEFINE_STATE_TYPE
%token TK_PAJE_DEFINE_EVENT_TYPE
%token TK_PAJE_DEFINE_LINK_TYPE
%token TK_PAJE_DEFINE_ENTITY_VALUE
%token TK_PAJE_CREATE_CONTAINER
%token TK_PAJE_DESTROY_CONTAINER
%token TK_PAJE_SET_VARIABLE
%token TK_PAJE_ADD_VARIABLE
%token TK_PAJE_SUB_VARIABLE
%token TK_PAJE_SET_STATE
%token TK_PAJE_PUSH_STATE
%token TK_PAJE_POP_STATE
%token TK_PAJE_RESET_STATE
%token TK_PAJE_START_LINK
%token TK_PAJE_END_LINK
%token TK_PAJE_NEW_EVENT
%token<argument_data> TK_FLOAT
%token<argument_data> TK_INT
%token<argument_data> TK_STRING
%token TK_BREAK

%type<eventId> event_name;
%type<field_data> field_name;
%type<fieldType> field_type;
%type<eventCode> event_id;
%type<argument_data> argument;

%error-verbose
%right TK_INT

%%

paje: declarations events { return 4;};

declarations: declaration declarations | ;
declaration: TK_EVENT_DEF_BEGIN event_name event_id TK_BREAK
             {
               def = new PajeEventDefinition($2, $3, yylineno, globalDefinitions);
             }
             fields TK_EVENT_DEF_END TK_BREAK
             {
               if (def->uniqueIdentifier >= defsv_current_size){
                 defsv_current_size = def->uniqueIdentifier + 1;
                 defsv = (PajeEventDefinition**)realloc (defsv, (defsv_current_size)*sizeof(PajeEventDefinition*));
               }
               defsv[def->uniqueIdentifier] = def;
               def = NULL;
             };
event_name:
        TK_PAJE_DEFINE_CONTAINER_TYPE { $$ = PajeDefineContainerTypeEventId;} |
	TK_PAJE_DEFINE_VARIABLE_TYPE { $$ = PajeDefineVariableTypeEventId;} |
	TK_PAJE_DEFINE_STATE_TYPE { $$ = PajeDefineStateTypeEventId;} |
	TK_PAJE_DEFINE_EVENT_TYPE { $$ = PajeDefineEventTypeEventId;} |
	TK_PAJE_DEFINE_LINK_TYPE { $$ = PajeDefineLinkTypeEventId;} |
	TK_PAJE_DEFINE_ENTITY_VALUE { $$ = PajeDefineEntityValueEventId;} |
	TK_PAJE_CREATE_CONTAINER { $$ = PajeCreateContainerEventId;} |
	TK_PAJE_DESTROY_CONTAINER { $$ = PajeDestroyContainerEventId;} |
	TK_PAJE_SET_VARIABLE { $$ = PajeSetVariableEventId;} |
	TK_PAJE_ADD_VARIABLE { $$ = PajeAddVariableEventId;} |
	TK_PAJE_SUB_VARIABLE { $$ = PajeSubVariableEventId;} |
	TK_PAJE_SET_STATE { $$ = PajeSetStateEventId;} |
	TK_PAJE_PUSH_STATE { $$ = PajePushStateEventId;} |
	TK_PAJE_POP_STATE { $$ = PajePopStateEventId;} |
	TK_PAJE_RESET_STATE { $$ = PajeResetStateEventId;} |
	TK_PAJE_START_LINK { $$ = PajeStartLinkEventId;} |
	TK_PAJE_END_LINK { $$ = PajeEndLinkEventId;} |
	TK_PAJE_NEW_EVENT { $$ = PajeNewEventEventId;};
event_id: TK_INT { $$ = $1.intValue; };
fields: field fields | ;
field: TK_EVENT_DEF field_name field_type {
              if ($2.fieldId == PAJE_Extra){
		def->addField($2.fieldId, $3, yylineno, std::string($2.fieldName));
              }else{
		def->addField($2.fieldId, $3, yylineno);
	      }
	} TK_BREAK;
field_name:
        TK_EVENT_DEF_ALIAS { $$.fieldId = PAJE_Alias; } |
	TK_EVENT_DEF_TYPE { $$.fieldId = PAJE_Type; } |
	TK_EVENT_DEF_NAME { $$.fieldId = PAJE_Name; } |
	TK_EVENT_DEF_COLOR { $$.fieldId = PAJE_Color; } |
	TK_EVENT_DEF_START_CONTAINER_TYPE { $$.fieldId = PAJE_StartContainerType; } |
	TK_EVENT_DEF_END_CONTAINER_TYPE { $$.fieldId = PAJE_EndContainerType; } |
	TK_EVENT_DEF_CONTAINER { $$.fieldId = PAJE_Container; } |
	TK_EVENT_DEF_TIME { $$.fieldId = PAJE_Time; } |
	TK_EVENT_DEF_START_CONTAINER { $$.fieldId = PAJE_StartContainer; } |
	TK_EVENT_DEF_END_CONTAINER { $$.fieldId = PAJE_EndContainer; } |
	TK_EVENT_DEF_VALUE { $$.fieldId = PAJE_Value; } |
	TK_EVENT_DEF_KEY { $$.fieldId = PAJE_Key; } |
        TK_EVENT_DEF_LINE { $$.fieldId = PAJE_Line; } |
        TK_EVENT_DEF_FILE { $$.fieldId = PAJE_File; } |
	TK_STRING { $$.fieldId = PAJE_Extra; $$.fieldName = $1.str; };
field_type:
TK_EVENT_DEF_FIELD_TYPE_STRING { $$ = PAJE_string; } |
TK_EVENT_DEF_FIELD_TYPE_FLOAT { $$ = PAJE_float; } |
TK_EVENT_DEF_FIELD_TYPE_DOUBLE { $$ = PAJE_double; } |
TK_EVENT_DEF_FIELD_TYPE_INT { $$ = PAJE_int; } |
TK_EVENT_DEF_FIELD_TYPE_HEX { $$ = PAJE_hex; } |
TK_EVENT_DEF_FIELD_TYPE_DATE { $$ = PAJE_date; } |
TK_EVENT_DEF_FIELD_TYPE_COLOR { $$ = PAJE_color; };

events: events event | ;
event: non_empty_event | empty_event;
non_empty_event:  { lineReset(); }  TK_INT  { lineDef ($2.intValue); lineAdd($2.str); } arguments TK_BREAK { lineSend (); };
empty_event: TK_BREAK; //empty event
arguments: arguments argument { lineAdd($2.str); } | ;
argument: TK_STRING { $$ = $1; } | TK_FLOAT { $$ = $1; } | TK_INT { $$ = $1; };

%%

static void lineReset ()
{
  event = new PajeTraceEvent (yylineno);
}

static void lineDef (int identifier)
{
  event->setDefinition (defsv[identifier]);
}

static void lineAdd (char *str)
{
  event->addField (str);
}

static void lineSend ()
{
  if (!event->check (NULL)) exit(1);
  flexReader->outputEntity (event);
  delete event;
  event = NULL;
}
