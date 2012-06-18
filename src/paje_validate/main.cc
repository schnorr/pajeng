#include <string>
#include <iostream>
#include <exception>
#include "PajeFileReader.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"
#include <argp.h>

#define VALIDATE_INPUT_SIZE 2
static char doc[] = "Checks if <paje.trace> strictly follows the Paje file format definition";
static char args_doc[] = "<paje.trace>";

static struct argp_option options[] = {
  { 0 }
};

struct arguments {
  char *input[VALIDATE_INPUT_SIZE];
  int input_size;
};

static int parse_options (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = (struct arguments*)(state->input);
  switch (key){
  case ARGP_KEY_ARG:
    if (arguments->input_size == VALIDATE_INPUT_SIZE) {
      /* Too many arguments. */
      argp_usage (state);
    }
    arguments->input[state->arg_num] = arg;
    arguments->input_size++;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1) {
      /* Not enough arguments. */
      argp_usage (state);
    }
    break;
  default: return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_options, args_doc, doc };

bool is_readable (const std::string & filename)
{
  std::ifstream file(filename.c_str());
  bool ret = !file.fail();
  file.close();
  return ret;
}

int main (int argc, char **argv)
{
  struct arguments arguments;
  bzero (&arguments, sizeof(struct arguments));
  if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
    fprintf(stderr, "%s, error during the parsing of parameters\n", argv[0]);
    return 1;
  }

  if (!is_readable(std::string(arguments.input[0]))){
    std::cerr << "trace file \"" << argv[1] << "\" not found" << std::endl;
    return 1;
  }

  std::cout << arguments.input[0] << std::endl;

  PajeFileReader *reader = new PajeFileReader (std::string(arguments.input[0]), NULL);
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
