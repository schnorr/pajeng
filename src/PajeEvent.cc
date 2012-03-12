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
    throw "Field count does not match definition for line '"+ str + "'.";
  }

  valueLine = line;
  pajeEventDefinition = def;
}
