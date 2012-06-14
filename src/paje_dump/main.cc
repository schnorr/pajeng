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

void dump (PajeSimulator *simulator)
{
  std::vector<PajeContainer*> stack;
  stack.push_back (simulator->rootInstance());

  while (stack.size ()){
    PajeContainer *container = stack.back();
    stack.pop_back ();

    //output container description
    std::cout << container->description() << std::endl;

    std::vector<PajeType*> containedTypes;
    std::vector<PajeType*>::iterator it;
    containedTypes = simulator->containedTypesForContainerType (container->type());
    for (it = containedTypes.begin(); it != containedTypes.end(); it++){
      PajeType *type = *it;
      if (simulator->isContainerType (type)){
        std::vector<PajeContainer*> children;
        std::vector<PajeContainer*>::iterator it;
        children = simulator->enumeratorOfContainersTypedInContainer (type, container);
        for (it = children.begin(); it != children.end(); it++){
          stack.push_back (*it);
        }
      }else{
        std::vector<PajeEntity*> entities;
        std::vector<PajeEntity*>::iterator it;
        entities = simulator->enumeratorOfEntitiesTypedInContainer (type,
                                                                    container,
                                                                    simulator->startTime(),
                                                                    simulator->endTime());
        for (it = entities.begin(); it != entities.end(); it++){
          PajeEntity *entity = *it;

          //output entity description
          std::cout << entity->description() << std::endl;
        }
      }
    }
  }
}

int main (int argc, char **argv)
{
  if (argc < 2){
    std::cerr << "usage : " << argv[0] << " <trace-file>" << std::endl;
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

  std::string x = std::string(argv[0])+": ";
  std::cout << x << "This program dumps the contents of a Paje trace file." << std::endl;
  std::cout << x << "The column description is this:" << std::endl;
  std::cout << x << "Type, container, type, [starttime - endtime], value" << std::endl;

  dump (simulator);

  delete reader;
  delete decoder;
  delete simulator;
  return 0;
}
