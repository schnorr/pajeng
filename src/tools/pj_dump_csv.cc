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
#include <fstream>
#include <iostream>
#include <exception>
#include "PajeUnity.h"
#include "PajeException.h"
#include <argp.h>
#include "libpaje_config.h"

extern int dumpFloatingPointPrecision;

#define VALIDATE_INPUT_SIZE 1
static char doc[] = "Dumps FILE in five separate CSV files, one per entity nature "
                    "(Container, State, Variable, Event, Link). "
                    "Output files are named PREFIX.container.csv, PREFIX.state.csv, "
                    "PREFIX.variable.csv, PREFIX.event.csv, PREFIX.link.csv. "
                    "PREFIX defaults to FILE; it is required when reading from stdin.";
static char args_doc[] = "[FILE]";

static struct argp_option options[] = {
  {"prefix",                  'p', "PREFIX", 0, "Prefix for output CSV files (default: FILE)"},
  {"start",                   's', "START",  0, "Dump starts at timestamp START (instead of 0)"},
  {"end",                     'e', "END",    0, "Dump ends at timestamp END (instead of EOF)"},
  {"stop-at",                 'a', "TIME",   0, "Stop the trace simulation at TIME"},
  {"no-strict",               'n', 0, OPTION_ARG_OPTIONAL, "Support old field names in event definitions"},
  {"ignore-incomplete-links", 'z', 0, OPTION_ARG_OPTIONAL, "Ignore incomplete links (not recommended)"},
  {"flex",                    'f', 0, OPTION_ARG_OPTIONAL, "Use flex-based file reader"},
  {"no-imbrication",          'i', 0, OPTION_ARG_OPTIONAL, "No imbrication levels (push and pop become sets)"},
  {"float-precision",         'l', "PRECISION", 0, "Precision of floating point numbers"},
  {"version",                 'v', 0, OPTION_ARG_OPTIONAL, "Print version of this binary"},
  { 0 }
};

struct arguments {
  char *input;
  char *prefix;
  double start, end, stopat;
  int noStrict;
  int noImbrication;
  int ignoreIncompleteLinks;
  int flex;
};

static error_t parse_options (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = (struct arguments*)(state->input);
  switch (key){
  case 'p': arguments->prefix = arg; break;
  case 's': arguments->start = atof(arg); break;
  case 'e': arguments->end = atof(arg); break;
  case 'a': arguments->stopat = atof(arg); break;
  case 'n': arguments->noStrict = 1; break;
  case 'i': arguments->noImbrication = 1; break;
  case 'z': arguments->ignoreIncompleteLinks = 1; break;
  case 'f': arguments->flex = 1; break;
  case 'l': dumpFloatingPointPrecision = atoi(arg); break;
  case 'v': printf("%s\n", LIBPAJE_VERSION_STRING); exit(0); break;
  case ARGP_KEY_ARG:
    if (arguments->input != NULL) {
      argp_usage (state);
    }
    arguments->input = arg;
    break;
  case ARGP_KEY_END:
    if (arguments->prefix == NULL) {
      if (arguments->input == NULL) {
        argp_error (state, "A --prefix is required when reading from stdin.");
      }
      arguments->prefix = arguments->input;
    }
    break;
  default: return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = { options, parse_options, args_doc, doc };

static void open_csv (const std::string &path, std::ofstream &stream, const std::string &header)
{
  stream.open(path);
  if (!stream.is_open()){
    fprintf(stderr, "Error: cannot open output file '%s'\n", path.c_str());
    exit(1);
  }
  stream << header << "\n";
}

void dump_csv (struct arguments *arguments, PajeComponent *simulator)
{
  std::string prefix = std::string(arguments->prefix);

  double start = arguments->start;
  double end   = arguments->end;
  if (start == -1) start = simulator->startTime();
  if (end   == -1) end   = simulator->endTime();

  // Collect all entities by nature so we can inspect user field names before writing headers
  std::vector<PajeContainer*> containers;
  std::vector<PajeEntity*> stateEntities, variableEntities, eventEntities, linkEntities;

  std::vector<PajeContainer*> stack;
  stack.push_back(simulator->rootInstance());

  while (!stack.empty()){
    PajeContainer *container = stack.back();
    stack.pop_back();

    containers.push_back(container);

    std::vector<PajeType*> containedTypes =
      simulator->containedTypesForContainerType(container->type());
    for (auto it = containedTypes.begin(); it != containedTypes.end(); it++){
      PajeType *type = *it;
      if (simulator->isContainerType(type)){
        std::vector<PajeContainer*> children =
          simulator->enumeratorOfContainersTypedInContainer(type, container);
        for (auto c = children.begin(); c != children.end(); c++){
          stack.push_back(*c);
        }
      } else {
        std::vector<PajeEntity*> entities =
          simulator->enumeratorOfEntitiesTypedInContainer(type, container, start, end);
        for (auto e = entities.begin(); e != entities.end(); e++){
          PajeEntity *entity = *e;
          if (dynamic_cast<PajeUserLink*>(entity)){
            linkEntities.push_back(entity);
          } else if (dynamic_cast<PajeUserState*>(entity)){
            stateEntities.push_back(entity);
          } else if (dynamic_cast<PajeUserEvent*>(entity)){
            eventEntities.push_back(entity);
          } else if (dynamic_cast<PajeUserVariable*>(entity)){
            variableEntities.push_back(entity);
          }
        }
      }
    }
  }

  // Return extra field names (with leading ", ") from the first entity that has them, or "".
  auto extraHeader = [](const std::vector<PajeEntity*> &entities) -> std::string {
    for (auto e : entities){
      std::string h = e->extraDescriptionHeader(true);
      if (!h.empty()) return h;
    }
    return std::string();
  };

  // Write an entity row always including any extra field values.
  auto writeEntity = [](std::ofstream &file, PajeEntity *entity){
    file << entity->description() << entity->extraDescription(true) << "\n";
  };

  // Container CSV
  {
    std::string header = "Container, Parent, Type, Start, End, Duration, Name";
    for (auto c : containers){
      std::string h = c->extraDescriptionHeader(true);
      if (!h.empty()){ header += h; break; }
    }
    std::ofstream containerFile;
    open_csv(prefix + ".container.csv", containerFile, header);
    for (auto c : containers){
      containerFile << c->description() << c->extraDescription(true) << "\n";
    }
  }

  // State CSV
  {
    std::ofstream stateFile;
    open_csv(prefix + ".state.csv", stateFile,
             "State, Container, Type, Start, End, Duration, Imbrication, Value" + extraHeader(stateEntities));
    for (auto e : stateEntities) writeEntity(stateFile, e);
  }

  // Variable CSV
  {
    std::ofstream variableFile;
    open_csv(prefix + ".variable.csv", variableFile,
             "Variable, Container, Type, Start, End, Duration, Value" + extraHeader(variableEntities));
    for (auto e : variableEntities) writeEntity(variableFile, e);
  }

  // Event CSV
  {
    std::ofstream eventFile;
    open_csv(prefix + ".event.csv", eventFile,
             "Event, Container, Type, Time, Value" + extraHeader(eventEntities));
    for (auto e : eventEntities) writeEntity(eventFile, e);
  }

  // Link CSV
  {
    std::ofstream linkFile;
    open_csv(prefix + ".link.csv", linkFile,
             "Link, Container, Type, Start, End, Duration, Value, StartContainer, EndContainer" + extraHeader(linkEntities));
    for (auto e : linkEntities) writeEntity(linkFile, e);
  }
}

int main (int argc, char **argv)
{
  struct arguments arguments;
  bzero(&arguments, sizeof(struct arguments));
  arguments.start = arguments.end = arguments.stopat = -1;

  if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
    fprintf(stderr, "Error during the parsing of parameters\n");
    return 1;
  }

  PajeUnity *unity;
  try {
    unity = new PajeUnity(arguments.flex,
                          !arguments.noStrict,
                          arguments.input ? std::string(arguments.input) : std::string(),
                          arguments.stopat,
                          arguments.ignoreIncompleteLinks,
                          NULL,   /* probabilistic */
                          arguments.noImbrication,
                          0,      /* userDefined */
                          0,      /* outofcore */
                          NULL,   /* entityHierarchy */
                          0       /* quiet */);
  } catch (PajeException &e) {
    e.report();
    fprintf(stderr, "Error during the parsing of the input file\n");
    return 1;
  }

  dump_csv(&arguments, unity);

  delete unity;
  return 0;
}
