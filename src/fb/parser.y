%{
#include <stdio.h>
#include <search.h>
#include "main.h"

  PajeEventDefinition *eventBeingDefined;

  extern "C"
  {
    extern int yylineno;
    int yylex(void);
    void yyerror (char const *mensagem);
    int yyparse ();
  }

  PajeEventDefinition **defsv;
  PajeEventDefinition *def;

%}

%union {
  PajeEventId eventId;
  PajeField fieldId;
  PajeFieldType fieldType;
  int eventCode;
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
%token TK_FLOAT
%token<eventCode> TK_INT
%token TK_STRING
%token TK_BREAK
%token TOKEN_ERRO

%type<eventId> event_name;
%type<fieldId> field_name;
%type<fieldType> field_type;
%type<eventCode> event_id;

%error-verbose
%right TK_INT


%%

paje: declarations { return 4; };

declarations: declaration declarations | ;
declaration: TK_EVENT_DEF_BEGIN event_name event_id optional_break
             {
               def = new PajeEventDefinition();
               def->pajeEventId = $2;
               def->number = $3;
               def->fields.push_back(PAJE_Event);
             }
             fields TK_EVENT_DEF_END optional_break
             {
               std::cout << *def << std::endl;
               defsv = (PajeEventDefinition**)realloc (defsv, (def->number+1)*sizeof(PajeEventDefinition*));
               defsv[def->number] = def;
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
event_id: TK_INT { $$ = $1; };
fields: field fields | ;
field: TK_EVENT_DEF field_name field_type { def->addField($2, $3, yylineno); } optional_break;
field_name:
        TK_EVENT_DEF_ALIAS { $$ = PAJE_Alias; } |
	TK_EVENT_DEF_TYPE { $$ = PAJE_Type; } |
	TK_EVENT_DEF_NAME { $$ = PAJE_Name; } |
	TK_EVENT_DEF_COLOR { $$ = PAJE_Color; } |
	TK_EVENT_DEF_START_CONTAINER_TYPE { $$ = PAJE_StartContainerType; } |
	TK_EVENT_DEF_END_CONTAINER_TYPE { $$ = PAJE_EndContainerType; } |
	TK_EVENT_DEF_CONTAINER { $$ = PAJE_Container; } |
	TK_EVENT_DEF_TIME { $$ = PAJE_Time; } |
	TK_EVENT_DEF_START_CONTAINER { $$ = PAJE_StartContainer; } |
	TK_EVENT_DEF_END_CONTAINER { $$ = PAJE_EndContainer; } |
	TK_EVENT_DEF_VALUE { $$ = PAJE_Value; } |
	TK_EVENT_DEF_KEY { $$ = PAJE_Key; } |
        TK_EVENT_DEF_LINE { $$ = PAJE_Line; } |
        TK_EVENT_DEF_FILE { $$ = PAJE_File; } |
	TK_STRING { $$ = PAJE_Extra; };
field_type:
TK_EVENT_DEF_FIELD_TYPE_STRING { $$ = PAJE_string; } |
TK_EVENT_DEF_FIELD_TYPE_FLOAT { $$ = PAJE_float; } |
TK_EVENT_DEF_FIELD_TYPE_DOUBLE { $$ = PAJE_double; } |
TK_EVENT_DEF_FIELD_TYPE_INT { $$ = PAJE_int; } |
TK_EVENT_DEF_FIELD_TYPE_HEX { $$ = PAJE_hex; } |
TK_EVENT_DEF_FIELD_TYPE_DATE { $$ = PAJE_date; } |
TK_EVENT_DEF_FIELD_TYPE_COLOR { $$ = PAJE_color; };
optional_break: TK_BREAK | ;
%%
