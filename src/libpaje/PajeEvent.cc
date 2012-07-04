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
#include "PajeEvent.h"

PajeEvent::PajeEvent (PajeEventDefinition *def, paje_line *line)
{
  if (line->word_count != def->fieldCount){
    int i;
    std::string str;
    for (i = 0; i < line->word_count; i++){
      str += std::string(line->word[i]);
      str += " ";
    }
    std::cout << *def << std::endl;
    std::cout << "Line field count: " << line->word_count << std::endl;
    std::cout << "Definition field count: " << def->fieldCount << std::endl;
    throw "Field count does not match definition for line '"+ str + "' in line .";
  }
  valueLine = line;
  pajeEventDefinition = def;
  lineNumber = line->lineNumber;
}

PajeEventId PajeEvent::pajeEventId (void)
{
  return pajeEventDefinition->pajeEventId;
}

std::string PajeEvent::valueForFieldId (std::string name)
{
  int index = pajeEventDefinition->indexForFieldId (name);
  if (index == -1){
    return std::string("");
  }else{
    return std::string(valueLine->word[index]);
  }
}

long long PajeEvent::getLineNumber (void) const
{
  return lineNumber;
}

std::string PajeEvent::description (void) const
{
  std::stringstream output;
  unsigned int i;
  output << "(Line: " << valueLine->lineNumber;
  output << ", Contents: '";
  for (i = 0; i < valueLine->word_count; i++){
    output << std::string(valueLine->word[i]);
    if (i+1 != valueLine->word_count) output << " ";
  }
  output << "')";
  return output.str();
}

std::ostream &operator<< (std::ostream &output, const PajeEvent &event)
{
  output << event.description();
  return output;
}
