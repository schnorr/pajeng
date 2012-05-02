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
    throw "Field count does not match definition for line '"+ str + "'.";
  }

  valueLine = line;
  pajeEventDefinition = def;
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

std::ostream &operator<< (std::ostream &output, const PajeEvent &event)
{
  int i;
  output << "(Line: " << event.lineNumber;
  output << ", Contents: '";
  for (i = 0; i < event.valueLine->word_count; i++){
    output << std::string(event.valueLine->word[i]);
    if (i+1 != event.valueLine->word_count) output << " ";
  }
  output << "')";
  return output;
}
