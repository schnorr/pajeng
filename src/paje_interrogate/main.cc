#include <string>
#include <iostream>
#include <exception>
#include "PajeFileReader.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"

bool is_readable (const std::string & filename)
{
  std::ifstream file(filename.c_str());
  bool ret = !file.fail();
  file.close();
  return ret;
}

void interrogate (PajeSimulator *simulator, std::vector<std::string> commands)
{
  PajeType *type = NULL;
  PajeContainer *container = NULL;
  while (commands.size()){
    std::string command = commands.back();
    bool consumed = false;
    //check if command is a container
    if (!container){
      container = simulator->containerWithName (command);
      if (container){
        commands.pop_back();
        consumed = true;
        command = commands.back();
      }
    }

    //check if command is a type
    if (!type){
      type = simulator->entityTypeWithName (command);
      if (type){
        commands.pop_back();
        consumed = true;
      }
    }

    if (type && container){
      std::cout << "Considering container name: " << container->name() <<std::endl;
      std::cout << "Considering type name: " << type->name << std::endl;
      if (simulator->isContainerType (type)){
        std::vector<PajeContainer*> list = simulator->enumeratorOfContainersTypedInContainer (type, container);
        std::vector<PajeContainer*>::iterator it;
        for (it = list.begin(); it != list.end(); it++){
          std::cout << " " << (*it)->name() << std::endl;
        }
      }else{
        std::cout << "NOT" << std::endl;
      }
      type = NULL;
      container = NULL;

      if (!consumed){
        commands.pop_back();
      }
    }
  }
}

int main (int argc, char **argv)
{
  if (argc < 2){
    std::cerr << "usage : " << argv[0] << " <trace-file> <list-of-commands>" << std::endl;
    return 1;
  }
  if (!is_readable(std::string(argv[1]))){
    std::cerr << "trace file \"" << argv[1] << "\" not found" << std::endl;
    return 1;
  }

  PajeFileReader *reader = new PajeFileReader (std::string(argv[1]), NULL);
  PajeEventDecoder *decoder = new PajeEventDecoder ();
  PajeSimulator *simulator = new PajeSimulator ();

  reader->setOutputComponent (decoder);
  decoder->setInputComponent (reader);
  decoder->setOutputComponent (simulator);
  simulator->setInputComponent (decoder);

  try {
    int i = 0;
    while (reader->hasMoreData()){
      reader->startChunk (i);
      reader->readNextChunk();
      reader->endOfChunkLast (!reader->hasMoreData());
      i++;
    }
  }catch (std::string exception){
    std::cout << "Exception: " << exception << std::endl;
    return 1;
  }

  simulator->report();

  //prepare the command
  std::vector<std::string> command;
  unsigned int i;
  for (i = argc-1; i > 1; i--){ //1 because 0 is binary, 1 is trace filename
    command.push_back (std::string(argv[i]));
  }

  //interrogate the simulator
  try {
    interrogate (simulator, command);
  }catch (std::string exception){
    std::cout << "Exception: " << exception << std::endl;
    return 1;
  }

  delete reader;
  delete decoder;
  delete simulator;
  return 0;
}
