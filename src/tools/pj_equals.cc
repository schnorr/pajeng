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
#include "omp.h"
#include "PajeUnity.h"
#include <argp.h>
#include "libpaje_config.h"

extern int dumpFloatingPointPrecision;

#define VALIDATE_INPUT_SIZE 3
static char doc[] = "Returns or prints to stdout 1 if two traces are equal, 0 otherwise.";
static char args_doc[] = "FILE1 FILE2";

static struct argp_option options[] = {
  {"start1", 's', "START1", 0, "Comparison starts at timestamp START1 of trace1 (instead of 0)"},
  {"end1", 'e', "END1", 0, "Comparison ends at timestamp END1 of trace1 (instead of EOF)"},
  {"stop-at1", 'a', "TIME1", 0, "Stop the trace1 simulation at TIME1"},
  {"start2", 't', "START2", 0, "Comparison starts at timestamp START2 of trace2 (instead of 0)"},
  {"end2", 'd', "END2", 0, "Comparison ends at timestamp END2 of trace2 (instead of EOF)"},
  {"stop-at2", 'o', "TIME2", 0, "Stop the trace simulation at TIME2"},
  {"icase", 'i', 0, OPTION_ARG_OPTIONAL, "Ignore case when comparing identifiers"},
  {"no-strict", 'n', 0, OPTION_ARG_OPTIONAL, "Support old field names in event definitions"},
  {"ignore-incomplete-links", 'z', 0, OPTION_ARG_OPTIONAL, "Ignore incomplete links (not recommended)"},
  {"quiet", 'q', 0, OPTION_ARG_OPTIONAL, "Do not output to stdout. Instead, return the result"},
  {"flex", 'f', 0, OPTION_ARG_OPTIONAL, "Use flex-based file reader"},
  {"version", 'v', 0, OPTION_ARG_OPTIONAL, "Print version of this binary"},
  { 0 }
};

struct arguments {
  char *input[VALIDATE_INPUT_SIZE];
  double start1, end1, stopat1;
  double start2, end2, stopat2;
  int icase;
  int noStrict;
  int input_size;
  int ignoreIncompleteLinks;
  int quiet;
  int flex;
};

static error_t parse_options (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = (struct arguments*)(state->input);
  switch (key){
  case 's': arguments->start1 = atof(arg); break;
  case 'e': arguments->end1 = atof(arg); break;
  case 'a': arguments->stopat1 = atof(arg); break;
  case 't': arguments->start2 = atof(arg); break;
  case 'd': arguments->end2 = atof(arg); break;
  case 'o': arguments->stopat2 = atof(arg); break;
  case 'i': arguments->icase = 1; break;
  case 'n': arguments->noStrict = 1; break;
  case 'z': arguments->ignoreIncompleteLinks = 1; break;
  case 'q': arguments->quiet = 1; break;
  case 'f': arguments->flex = 1; break;
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

#define VALUE(e) ((PajeValueEntity*)(e))->value()
#define VALUE_ID(e) VALUE(e)->identifier()

bool
cmp(PajeEntity *a, PajeEntity *b)
{
  if (a && b) {
    if (!VALUE(a) && !VALUE(b))
      return 0;
    if (VALUE(a) && VALUE(b))
      return !(VALUE_ID(a) == VALUE_ID(b));
    return 1;
  }
  return 1;
}

bool
tcmp(PajeType *a, PajeType *b)
{
  return !(a && b && a->kind() == b->kind());
}

bool
itcmp(PajeType *a, PajeType *b)
{
  return !((a && b) && !strcasecmp(a->kind().c_str(), b->kind().c_str()));
}

bool
icmp(PajeEntity *a, PajeEntity *b)
{
  if (a && b) {
    if (!VALUE(a) && !VALUE(b))
      return 0;
    if (VALUE(a) && VALUE(b))
      return strcasecmp(VALUE_ID(a).c_str(), VALUE_ID(b).c_str());
    return 1;
  }
  return 1;
}

int
equals(struct arguments *arguments, PajeComponent *s1, PajeComponent *s2)
{
  std::vector<PajeContainer*> stack1, stack2;
  stack1.push_back(s1->rootInstance());
  stack2.push_back(s2->rootInstance());
  // TODO start1/2 end1/2
  double start1 = arguments->start1;
  double start2 = arguments->start2;
  double end1 = arguments->end1;
  double end2 = arguments->end2;
  if (start1 == -1) start1 = s1->startTime();
  if (start2 == -1) start2 = s2->startTime();
  if (end1 == -1) end1 = s1->endTime();
  if (end2 == -1) end2 = s2->endTime();
  while (!stack1.empty() && !stack2.empty()) {
    PajeContainer *c1 = stack1.back();
    PajeContainer *c2 = stack2.back();
    stack1.pop_back();
    stack2.pop_back();
    std::vector<PajeType*> containedTypes1;
    containedTypes1 = s1->containedTypesForContainerType(c1->type());
    std::vector<PajeType*> containedTypes2;
    containedTypes2 = s2->containedTypesForContainerType(c2->type());
    if (containedTypes1.size() != containedTypes2.size())
      return 0;
    for (size_t i = 0; i < containedTypes1.size(); ++i) {
      PajeType *t1 = containedTypes1.at(i);
      PajeType *t2 = containedTypes2.at(i);
      if (arguments->icase) {
        if (itcmp(t1, t2))
          return 0;
      } else if (tcmp(t1, t2)) {
        return 0;
      }
      if (s1->isContainerType(t1)) {
        std::vector<PajeContainer*> children;
        std::vector<PajeContainer*>::iterator it;
#pragma omp parallel sections num_threads(2) private(children, it)
        {
#pragma omp section
          {
            children = s1->enumeratorOfContainersTypedInContainer(t1, c1);
            for (it = children.begin(); it != children.end(); ++it)
              stack1.push_back(*it);
          }
#pragma omp section
          {
            children = s2->enumeratorOfContainersTypedInContainer(t2, c2);
            for (it = children.begin(); it != children.end(); ++it)
              stack2.push_back(*it);
          }
        }
      } else {
        std::vector<PajeEntity*> entities1, entities2;
        std::vector<PajeEntity*>::iterator it1, it2;
#pragma omp parallel sections num_threads(2)
        {
#pragma omp section
          {
            entities1 = s1->enumeratorOfEntitiesTypedInContainer(t1, c1, start1, end1);
          }
#pragma omp section
          {
            entities2 = s2->enumeratorOfEntitiesTypedInContainer(t2, c2, start2, end2);
          }
        }
        if (entities1.size() != entities2.size())
          return 0;
        for (size_t j = 0; j < entities1.size(); ++j)
          if (arguments->icase) {
            if (icmp(entities1.at(j), entities2.at(j)))
              return 0;
          } else if (cmp(entities1.at(j), entities2.at(j))) {
            return 0;
          }
      }
    }
  }
  if (stack1.empty() && stack2.empty())
    return 1;
  return 0;
}

int
main(int argc, char **argv)
{
  struct arguments arguments;
  bzero(&arguments, sizeof(struct arguments));
  arguments.start1 = arguments.end1 = arguments.stopat1 = -1;
  arguments.start2 = arguments.end2 = arguments.stopat2 = -1;
  if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
    fprintf(stderr, "%s, error during the parsing of parameters\n", argv[0]);
    exit(2);
  }
  PajeUnity *s1;
  PajeUnity *s2;
#pragma omp parallel sections num_threads(2)
  {
#pragma omp section
    {
      s1 = new PajeUnity(arguments.flex, !arguments.noStrict,
          std::string(arguments.input[0]), arguments.stopat1,
          arguments.ignoreIncompleteLinks, 0);
    }
#pragma omp section
    {
      s2 = new PajeUnity(arguments.flex, !arguments.noStrict,
          std::string(arguments.input[1]), arguments.stopat2,
          arguments.ignoreIncompleteLinks, 0);
    }
  }

  int ans = equals(&arguments, s1, s2);

#pragma omp parallel sections num_threads(2)
  {
#pragma omp section
    {
      delete s1;
    }
#pragma omp section
    {
      delete s2;
    }
  }

  if (arguments.quiet)
    return ans;
  else
    std::cout << ans << std::endl;

  return 0;

}
