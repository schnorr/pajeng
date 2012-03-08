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

class PajeEvent {
private:
  paje_line *valueLine;
  PajeEventDefinition *pajeEventDefinition;
};

#endif

