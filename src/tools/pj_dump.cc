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

#define VALIDATE_INPUT_SIZE 1
static char doc[] = "Dumps FILE, or standard input, in a CSV-like textual format";
static char args_doc[] = "[FILE]";

#define OPT_TH 400 //TypeHierarchy
#define OPT_EH 401 //EntityHierarchy

static struct argp_option options[] = {
  {"start",                   's', "START", 0, "Dump starts at timestamp START (instead of 0)"},
  {"end",                     'e', "END", 0, "Dump ends at timestamp END (instead of EOF)"},
  {"stop-at",                 'a', "TIME", 0, "Stop the trace simulation at TIME"},
  {"no-strict",               'n', 0, OPTION_ARG_OPTIONAL, "Support old field names in event definitions"},
  {"ignore-incomplete-links", 'z', 0, OPTION_ARG_OPTIONAL, "Ignore incomplete links (not recommended)"},
  {"quiet",                   'q', 0, OPTION_ARG_OPTIONAL, "Do not dump, only simulate"},
  {"flex",                    'f', 0, OPTION_ARG_OPTIONAL, "Use flex-based file reader"},
  {"user-defined",            'u', 0, OPTION_ARG_OPTIONAL, "Dump user-defined fields"},
  {"probabilistic",           'p', "TYPENAME", 0, "Dump global states based on TYPENAME"},
  {"float-precision",         'l', "PRECISION", 0, "Precision of floating point numbers"},
  {"no-imbrication",          'i', 0, OPTION_ARG_OPTIONAL, "No imbrication levels (push and pop become sets)"},
  {"container",               'c', 0, OPTION_ARG_OPTIONAL, "Print container hierarchy in stdout"},
  {"dot",                     'd', 0, OPTION_ARG_OPTIONAL, "Print type hierarchy in dot format in stdout"},
  {"version",                 'v', 0, OPTION_ARG_OPTIONAL, "Print version of this binary"},
  {"time",                    't', 0, OPTION_ARG_OPTIONAL, "Print number of seconds to simulate input"},
  {"header",                  'h', 0, OPTION_ARG_OPTIONAL, "Print CSV header with column names"},
  {"type-hierarchy",       OPT_TH, "FILE", 0, "Output the type hierarchy to FILE (CSV)"},
  {"entity-hierarchy",     OPT_EH, "FILE", 0, "Output the entity hierarchy to FILE (CSV)"},
  {"out-of-core",             'o', 0, OPTION_ARG_OPTIONAL, "Out of core execution (smallest memory footprint)"},
  { 0 }
};

struct arguments {
  char *input;
  double start, end, stopat;
  int noStrict;
  int noImbrication;
  int input_size;
  int ignoreIncompleteLinks;
  int quiet;
  int flex;
  int userDefined;
  int container;
  int dot;
  int time;
  int csvHeader;
  int outofcore;
  char *typeHierarchy;
  char *entityHierarchy;
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
  case 'i': arguments->noImbrication = 1; break;
  case 'z': arguments->ignoreIncompleteLinks = 1; break;
  case 'q': arguments->quiet = 1; break;
  case 'f': arguments->flex = 1; break;
  case 'u': arguments->userDefined = 1; break;
  case 'p': arguments->probabilistic = strdup(arg); break;
  case 'l': dumpFloatingPointPrecision = atoi(arg); break;
  case 'c': arguments->container = 1; break;
  case 'd': arguments->dot = 1; break;
  case 't': arguments->time = 1; break;
  case 'h': arguments->csvHeader = 1; break;
  case 'o': arguments->outofcore = 1; break;
  case OPT_TH: arguments->typeHierarchy = arg; break;
  case OPT_EH: arguments->entityHierarchy = arg; break;
  case 'v': printf("%s\n", LIBPAJE_VERSION_STRING); exit(0); break;
  case ARGP_KEY_ARG:
    if (arguments->input != NULL) {
      /* Too many arguments. */
      argp_usage (state);
    }
    arguments->input = arg;
    break;
  case ARGP_KEY_END:
    //no argument is okay, nothing to check
    break;
  default: return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_options, args_doc, doc };

void typeHierarchyDump (struct arguments *arguments, PajeComponent *simulator)
{
  if (!arguments->typeHierarchy) return;

  std::ofstream outputFile;
  outputFile.open (arguments->typeHierarchy);
  outputFile << "Parent, Name, Nature" << std::endl;

  std::vector<PajeType*> stack;
  stack.push_back (simulator->rootEntityType());
  while (!stack.empty()){
    PajeType *parent = stack.back();
    stack.pop_back();

    //push back more types
    if (simulator->isContainerType (parent)){
      std::vector<PajeType*> children = simulator->containedTypesForContainerType(parent);
      while (!children.empty()){
        PajeType *child = children.back();

	outputFile << child->parent()->name() << ", " << child->name() << ", " << child->kind() << std::endl;

	if (child->isCategorizedType()){
	  //Report values of the categorized type
	  for (auto const &value : child->values()){
	    outputFile << child->name() << ", " << value.second->name() << ", Value" << std::endl;
	  }
	}

        stack.push_back (child);
        children.pop_back();
      }
    }
  }
  outputFile.close();
}

void entityHierarchyDump (struct arguments *arguments, PajeComponent *simulator)
{
  if (!arguments->entityHierarchy) return;

  std::ofstream outputFile;
  outputFile.open (arguments->entityHierarchy);
  outputFile << "Parent, Name, Type, Nature" << std::endl;

  std::vector<PajeContainer*> stack;
  stack.push_back (simulator->rootInstance());

  while (!stack.empty()){
    PajeContainer *container = stack.back();
    stack.pop_back ();

    outputFile <<
      (container->container()? container->container()->name() : "0") << ", " <<
      container->name() << ", " <<
      container->type()->name() << ", " <<
      "Container" << std::endl;

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
	outputFile <<
	  container->name() << ", " <<
	  type->name() << ", " <<
	  type->name() << ", " <<
	  type->kind() << std::endl;
      }
    }
  }
  outputFile.close();
}

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

    container->dump();

    //recurse on children
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
	  entity->dump();
	}
      }
    }
  }
}

static void dump_header_from_entity (struct arguments *arguments, PajeEntity *entity)
{
  std::cout << entity->descriptionHeader();
  if (arguments->userDefined){
    std::cout << entity->extraDescriptionHeader(true);
  }
  std::cout << std::endl;
}

static void dump_header (struct arguments *arguments, PajeComponent *simulator)
{
  bool containerHeader = true;
  bool linkHeader = true;
  bool stateHeader = true;
  bool eventHeader = true;
  bool variableHeader = true;

  if (!arguments->csvHeader) return;

  std::vector<PajeContainer*> stack;
  stack.push_back (simulator->rootInstance());

  while (!stack.empty()){
    PajeContainer *container = stack.back();
    stack.pop_back ();

    //Output container header
    if (containerHeader){
      dump_header_from_entity (arguments, container);
      containerHeader = false;
    }

    //Output remaining headers
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

	  if (linkHeader && dynamic_cast<PajeUserLink*>(entity)){
	    dump_header_from_entity (arguments, entity);
	    linkHeader = false;
	  }else if (stateHeader && dynamic_cast<PajeUserState*>(entity)){
	    dump_header_from_entity (arguments, entity);
	    stateHeader = false;
	  }else if (eventHeader && dynamic_cast<PajeUserEvent*>(entity)){
	    dump_header_from_entity (arguments, entity);
	    eventHeader = false;
	  }else if (variableHeader && dynamic_cast<PajeUserVariable*>(entity)){
	    dump_header_from_entity (arguments, entity);
	    variableHeader = false;
	  }
	  break;
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
				    arguments.input ? std::string(arguments.input) : std::string(),
				    arguments.stopat,
				    arguments.ignoreIncompleteLinks,
				    arguments.probabilistic,
                                    arguments.noImbrication,
				    arguments.userDefined,
				    arguments.outofcore);

  if (arguments.time){
    printf ("%f\n", unity->getTime());
  }

  if (arguments.dot){
    unity->reportDot();
    delete unity;
    return 0;
  }

  if (arguments.container){
    unity->reportContainer();
    delete unity;
    return 0;
  }
  
  if (arguments.probabilistic){
    delete unity;
    return 0;
  }

  //Three types of dumps (types, entities, timestamped objects)
  if (arguments.typeHierarchy){
    typeHierarchyDump (&arguments, unity);
  }

  if (arguments.entityHierarchy){
    entityHierarchyDump (&arguments, unity);
  }

  if (!arguments.quiet){
    dump_header (&arguments, unity);
    dump (&arguments, unity);
  }

  delete unity;
  return 0;
}
