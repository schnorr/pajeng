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
#include <sys/time.h>
#include <string>
#include <iostream>
#include <exception>
#include "PajeFileReader.h"
#include "PajeFlexReader.h"
#include "PajeEventDecoder.h"
#include "PajeSimulator.h"
#include "PajeException.h"
#include <argp.h>

#define VALIDATE_INPUT_SIZE 2
static char doc[] = "Checks if FILE, or standard input, strictly follows the Paje file format definition";
static char args_doc[] = "[FILE]";

static struct argp_option options[] = {
  {"no-strict", 'n', 0, OPTION_ARG_OPTIONAL, "Support old field names in event definitions"},
  {"quiet", 'q', 0, OPTION_ARG_OPTIONAL, "Be quiet"},
  {"time", 't', 0, OPTION_ARG_OPTIONAL, "Print number of seconds to simulate input"},
  {"flex", 'f', 0, OPTION_ARG_OPTIONAL, "Use flex-based file reader"},
  { 0 }
};

struct arguments {
  char *input[VALIDATE_INPUT_SIZE];
  int noStrict;
  int input_size;
  int quiet;
  int time;
  int flex;
};

static int parse_options (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = (struct arguments*)(state->input);
  switch (key){
  case 'n': arguments->noStrict = 1; break;
  case 't': arguments->time = 1; break;
  case 'q': arguments->quiet = 1; break;
  case 'f': arguments->flex = 1; break;
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

static double gettime (void)
{
  struct timeval tr;
  gettimeofday(&tr, NULL);
  return (double)tr.tv_sec+(double)tr.tv_usec/1000000;
}

int main (int argc, char **argv)
{
  struct arguments arguments;
  bzero (&arguments, sizeof(struct arguments));
  if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
    fprintf(stderr, "%s, error during the parsing of parameters\n", argv[0]);
    return 1;
  }

  PajeComponent *reader = NULL;
  PajeEventDecoder *decoder = NULL;
  PajeSimulator *simulator = NULL;

  //the global PajeDefinitions object
  PajeDefinitions *definitions = new PajeDefinitions (arguments.noStrict ? false : true); 

  try {
    //alloc reader
    if (arguments.flex){
      if (arguments.input_size == 0){
	reader = new PajeFlexReader(definitions);
      }else{
	reader = new PajeFlexReader(std::string(arguments.input[0]), definitions);
      }
    }else{
      if (arguments.input_size == 0){
	reader = new PajeFileReader();
      }else{
	reader = new PajeFileReader (std::string(arguments.input[0]));
      }
    }

    //alloc decoder and simulator
    if (!arguments.flex){
      decoder = new PajeEventDecoder(definitions);
    }
    simulator = new PajeSimulator ();

    //connect components
    if (arguments.flex){
      reader->setOutputComponent (simulator);
      simulator->setInputComponent (reader);
    }else{
      reader->setOutputComponent (decoder);
      decoder->setInputComponent (reader);
      decoder->setOutputComponent (simulator);
      simulator->setInputComponent (decoder);
    }
  }catch (PajeException& e){
    e.reportAndExit ();
  }

  //read and simulate
  double t1, t2;
  if (arguments.time){
    t1 = gettime();
  }
  try {
    reader->startReading();
    while (reader->hasMoreData()){
      reader->readNextChunk();
    }
    reader->finishedReading();
  }catch (PajeException& e){
    e.reportAndExit();
  }
  if (arguments.time){
    t2 = gettime();
    printf ("%f\n", t2-t1);
  }

  if (!arguments.quiet){
    simulator->report();
  }

  delete reader;
  delete decoder;
  delete simulator;
  return 0;
}
