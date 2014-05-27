%{
#include "PajeEventDefinition.h"

#include "parser.hh"

extern "C" int yylex();
%}

%option yylineno

SPACE  [\ \t\r]
LETTER [^\ \t\r\n]
DIGIT  [0-9]

%%

\n return TK_BREAK;

%EventDef                 return TK_EVENT_DEF_BEGIN;
%                         return TK_EVENT_DEF;
%EndEventDef              return TK_EVENT_DEF_END;
Alias                     return TK_EVENT_DEF_ALIAS;
Type                      return TK_EVENT_DEF_TYPE;
Name                      return TK_EVENT_DEF_NAME;
Color                     return TK_EVENT_DEF_COLOR;
StartContainerType        return TK_EVENT_DEF_START_CONTAINER_TYPE;
EndContainerType          return TK_EVENT_DEF_END_CONTAINER_TYPE;
Container                 return TK_EVENT_DEF_CONTAINER;
Time                      return TK_EVENT_DEF_TIME;
StartContainer            return TK_EVENT_DEF_START_CONTAINER;
EndContainer              return TK_EVENT_DEF_END_CONTAINER;
Value                     return TK_EVENT_DEF_VALUE;
Key                       return TK_EVENT_DEF_KEY;
Line                      return TK_EVENT_DEF_LINE;
File                      return TK_EVENT_DEF_FILE;

"string"                  return TK_EVENT_DEF_FIELD_TYPE_STRING;
"float"                   return TK_EVENT_DEF_FIELD_TYPE_FLOAT;
"double"                  return TK_EVENT_DEF_FIELD_TYPE_DOUBLE;
"int"                     return TK_EVENT_DEF_FIELD_TYPE_INT;
"date"                    return TK_EVENT_DEF_FIELD_TYPE_DATE;
"color"                   return TK_EVENT_DEF_FIELD_TYPE_COLOR;
 
PajeDefineContainerType   return TK_PAJE_DEFINE_CONTAINER_TYPE;
PajeDefineVariableType    return TK_PAJE_DEFINE_VARIABLE_TYPE;
PajeDefineStateType       return TK_PAJE_DEFINE_STATE_TYPE;
PajeDefineEventType       return TK_PAJE_DEFINE_EVENT_TYPE;
PajeDefineLinkType        return TK_PAJE_DEFINE_LINK_TYPE;
PajeDefineEntityValue     return TK_PAJE_DEFINE_ENTITY_VALUE;

PajeCreateContainer       return TK_PAJE_CREATE_CONTAINER;
PajeDestroyContainer      return TK_PAJE_DESTROY_CONTAINER;

PajeSetVariable           return TK_PAJE_SET_VARIABLE;
PajeAddVariable           return TK_PAJE_ADD_VARIABLE;
PajeSubVariable           return TK_PAJE_SUB_VARIABLE;

PajeSetState              return TK_PAJE_SET_STATE;
PajePushState             return TK_PAJE_PUSH_STATE;
PajePopState              return TK_PAJE_POP_STATE;
PajeResetState            return TK_PAJE_RESET_STATE;

PajeStartLink             return TK_PAJE_START_LINK;
PajeEndLink               return TK_PAJE_END_LINK;

PajeNewEvent              return TK_PAJE_NEW_EVENT;

{DIGIT}+"."{DIGIT}+                   return TK_FLOAT;
{DIGIT}+                              { yylval.eventCode = atoi(yytext); return TK_INT; }

\"[^"]*\"                             return TK_STRING;

"#".*\n
{SPACE}+ 

{LETTER}+               return TK_STRING;

%%
