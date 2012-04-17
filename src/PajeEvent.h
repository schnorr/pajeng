#ifndef __PAJEEVENT_H__
#define __PAJEEVENT_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeEventDefinition.h"
#include "PajeObject.h"

class PajeEvent : public PajeObject {
private:
  paje_line *valueLine;
  PajeEventDefinition *pajeEventDefinition;

public:
  PajeEvent (PajeEventDefinition *def, paje_line *line);
  PajeEventId pajeEventId (void);
};

#endif

