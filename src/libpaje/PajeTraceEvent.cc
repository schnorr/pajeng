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
#include "PajeTraceEvent.h"

PajeTraceEvent::PajeTraceEvent ()
{
  pajeEventDefinition = NULL;
  this->line = -1;
}

PajeTraceEvent::PajeTraceEvent (int line)
{
  pajeEventDefinition = NULL;
  this->line = line;
}

/*
 * PajeTraceEvent (PajeEventDefinition *, paje_line *): if this
 * constructor is used, the method addField shoult not be used, since
 * the initialization extracts all the information from the paje_line.
 */
PajeTraceEvent::PajeTraceEvent (PajeEventDefinition *def, paje_line *line)
{
  int i;
  for (i = 0; i < line->word_count; i++){
    addField (line->word[i]);
  }
  pajeEventDefinition = def;
  this->line = line->lineNumber;
  this->check (line);
}

PajeTraceEvent::~PajeTraceEvent ()
{
  fields.clear();
}

PajeEventId PajeTraceEvent::pajeEventId (void)
{
  return pajeEventDefinition->pajeEventIdentifier;
}

void PajeTraceEvent::addField (char *field)
{
  fields.push_back (std::string(field));
}

void PajeTraceEvent::clear (void)
{
  pajeEventDefinition = NULL;
  fields.clear();
}

bool PajeTraceEvent::check (paje_line *line)
{
  if (!pajeEventDefinition) return false;
  if (fields.size() != pajeEventDefinition->fieldCount()){
    std::stringstream st;
    if (line){
      st << *line;
    }else{
      st << this->line;
    }
    std::string lreport = st.str();
    std::cout << *pajeEventDefinition << std::endl;
    std::cout << "Line field count: " << fields.size() << std::endl;
    std::cout << "Definition field count: " << pajeEventDefinition->fieldCount() << std::endl;
    std::cout << "Field count does not match definition for line "+lreport << std::endl;
    return false;
  }else{
    return true;
  }
}

std::string PajeTraceEvent::valueForField (PajeField field)
{
  int index = pajeEventDefinition->indexForField (field);
  if (index == -1){
    return std::string("");
  }else{
    return fields.at(index);
  }
}

std::string PajeTraceEvent::valueForExtraField (std::string fieldName)
{
  int index = pajeEventDefinition->indexForExtraFieldNamed (fieldName);
  if (index == -1){
    return std::string("");
  }else{
    return fields.at(index);
  }
}

long long PajeTraceEvent::getLineNumber (void) const
{
  return line;
}

std::string PajeTraceEvent::description (void) const
{
  std::stringstream output;
  unsigned int i;
  output << "(Line: " << line;
  output << ", Fields: '" << fields.size();
  output << ", Contents: '";
  for (i = 0; i < fields.size(); i++){
    output << fields.at(i);
    if (i+1 != fields.size()) output << " ";
  }
  output << "')";
  return output.str();
}

PajeEventDefinition *PajeTraceEvent::definition (void)
{
  return pajeEventDefinition;
}

void PajeTraceEvent::setDefinition (PajeEventDefinition *def)
{
  pajeEventDefinition = def;
}

std::ostream &operator<< (std::ostream &output, const PajeTraceEvent &event)
{
  output << event.description();
  return output;
}
