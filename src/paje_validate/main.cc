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

int main (int argc, char **argv)
{
  if (argc != 2){
    std::cerr << "usage : paje_validate [trace-file]" << std::endl;
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
    std::cout << exception << std::endl;
    return 1;
  }

  simulator->report();

  delete reader;
  delete decoder;
  delete simulator;
  return 0;
}
