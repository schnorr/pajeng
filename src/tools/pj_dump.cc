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
#include <iomanip>
#include <exception>
#include "PajeUnity.h"
#include <argp.h>
#include "libpaje_config.h"

extern int dumpFloatingPointPrecision;

#define VALIDATE_INPUT_SIZE 2
static char doc[] = "Dumps FILE, or standard input, in a CSV-like textual format";
static char args_doc[] = "[FILE]";

static struct argp_option options[] = {
  {"start", 's', "START", 0, "Dump starts at timestamp START (instead of 0)"},
  {"end", 'e', "END", 0, "Dump ends at timestamp END (instead of EOF)"},
  {"stop-at", 'a', "TIME", 0, "Stop the trace simulation at TIME"},
  {"no-strict", 'n', 0, OPTION_ARG_OPTIONAL, "Support old field names in event definitions"},
  {"ignore-incomplete-links", 'z', 0, OPTION_ARG_OPTIONAL, "Ignore incomplete links (not recommended)"},
  {"quiet", 'q', 0, OPTION_ARG_OPTIONAL, "Do not dump, only simulate"},
  {"flex", 'f', 0, OPTION_ARG_OPTIONAL, "Use flex-based file reader"},
  {"user-defined", 'u', 0, OPTION_ARG_OPTIONAL, "Dump user-defined fields"},
  {"probabilistic", 'p', "TYPENAME", 0, "Dump global states based on TYPENAME"},
  {"float-precision", 'l', "PRECISION", 0, "Precision of floating point numbers"},
  {"version", 'v', 0, OPTION_ARG_OPTIONAL, "Print version of this binary"},
  { 0 }
};

struct arguments {
  char *input[VALIDATE_INPUT_SIZE];
  double start, end, stopat;
  int noStrict;
  int input_size;
  int ignoreIncompleteLinks;
  int quiet;
  int flex;
  int userDefined;
  char *probabilistic;
};

static error_t parse_options (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = (struct arguments*)(state->input);
  switch (key){
  case 's': arguments->start = atof(arg); break;
  case 'e': arguments->end = atof(arg); break;
  case 'a': arguments->stopat = atof(arg); break;
  case 'n': arguments->noStrict = 1; break;
  case 'z': arguments->ignoreIncompleteLinks = 1; break;
  case 'q': arguments->quiet = 1; break;
  case 'f': arguments->flex = 1; break;
  case 'u': arguments->userDefined = 1; break;
  case 'p': arguments->probabilistic = strdup(arg); break;
  case 'l': dumpFloatingPointPrecision = atoi(arg); break;
  case 'v': printf("%s\n", LIBPAJE_VERSION_STRING); exit(0); break;
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

void dump (struct arguments *arguments, PajeComponent *simulator)
{
  double start = arguments->start;
  double end = arguments->end;
  if (start == -1) start = simulator->startTime();
  if (end == -1) end = simulator->endTime();

  std::vector<PajeContainer*> stack;
  stack.push_back (simulator->rootInstance());

  while (!stack.empty()){
    PajeContainer *container = stack.back();
    stack.pop_back ();

    //output container description
    std::cout << container->description();
    if (arguments->userDefined){
      std::cout << container->extraDescription(true);
    }
    std::cout << std::endl;

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
          std::cout << entity->description();
	  if (arguments->userDefined){
	    std::cout << entity->extraDescription(true);
	  }
	  std::cout << std::endl;
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

  PajeUnity *unity = new PajeUnity (arguments.flex,
				    !arguments.noStrict,
				    std::string(arguments.input[0]),
				    arguments.stopat,
				    arguments.ignoreIncompleteLinks,
				    arguments.probabilistic);

  if (arguments.probabilistic){
    delete unity;
    return 0;
  }
  
  if (!arguments.quiet){
    dump (&arguments, unity);
  }

  delete unity;
  return 0;
}
