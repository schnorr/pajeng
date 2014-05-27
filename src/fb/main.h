#ifndef __MAIN_H
#define __MAIN_H
#include "PajeEventDefinition.h"

  class PajeEventDef {
  public:
    PajeEventId pajeEventId;
    int number; //unique identifier
    std::list<PajeField> fields; //list of fields
  };

#endif
