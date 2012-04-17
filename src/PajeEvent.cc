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
