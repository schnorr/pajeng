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
#include <stdlib.h>
#include <string>
#include <iostream>
#include <exception>
#include "PajeFileReader.h"
#include "PajeException.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"
#include <argp.h>

#define VALIDATE_INPUT_SIZE 2
static char doc[] = "Dumps FILE, or standard input, in a CSV-like textual format";
static char args_doc[] = "[FILE]";

static struct argp_option options[] = {
  {"start", 's', "START", 0, "Dump starts at timestamp START (instead of 0)"},
  {"end", 'e', "END", 0, "Dump ends at timestamp END (instead of EOF)"},
  {"stop-at", 'a', "TIME", 0, "Stop the trace simulation at TIME"},
  {"no-strict", 'n', 0, OPTION_ARG_OPTIONAL, "Support old field names in event definitions"},
  { 0 }
};

struct arguments {
  char *input[VALIDATE_INPUT_SIZE];
  double start, end, stopat;
  int noStrict;
  int input_size;
};

static int parse_options (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = (struct arguments*)(state->input);
  switch (key){
  case 's': arguments->start = atof(arg); break;
  case 'e': arguments->end = atof(arg); break;
  case 'a': arguments->stopat = atof(arg); break;
  case 'n': arguments->noStrict = 1; break;
  case ARGP_KEY_ARG:
    if (arguments->input_size == VALIDATE_INPUT_SIZE) {
      /* Too many arguments. */
      argp_usage (state);
    }
    arguments->input[state->arg_num] = arg;
    arguments->input_size++;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 0) {
      /* Not enough arguments. */
      argp_usage (state);
    }
    break;
  default: return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_options, args_doc, doc };

void dump (double start, double end, PajeSimulator *simulator)
{
  if (start == -1) start = simulator->startTime();
  if (end == -1) end = simulator->endTime();

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
                                                                    start,
                                                                    end);
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
  struct arguments arguments;
  bzero (&arguments, sizeof(struct arguments));
  arguments.start = arguments.end = arguments.stopat = -1;
  if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
    fprintf(stderr, "%s, error during the parsing of parameters\n", argv[0]);
    return 1;
  }

  PajeFileReader *reader;

  if (arguments.input_size != 0){
    try {
      reader = new PajeFileReader (std::string(arguments.input[0]));
    }catch (PajeException& e){
      e.reportAndExit ();
    }
  }else{
    reader = new PajeFileReader ();
  }

  PajeEventDecoder *decoder = new PajeEventDecoder (!arguments.noStrict);
  PajeSimulator *simulator = new PajeSimulator (arguments.stopat);

  reader->setOutputComponent (decoder);
  decoder->setInputComponent (reader);
  decoder->setOutputComponent (simulator);
  simulator->setInputComponent (decoder);

  try {
    reader->startReading ();
    while (reader->hasMoreData() && simulator->keepSimulating()){
      reader->readNextChunk ();
    }
    reader->finishedReading ();
  }catch (PajeException& e){
    e.reportAndExit();
  }

  dump (arguments.start, arguments.end, simulator);

  delete reader;
  delete decoder;
  delete simulator;
  return 0;
}
