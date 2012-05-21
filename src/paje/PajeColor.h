#ifndef __PAJE_COLOR_H
#define __PAJE_COLOR_H
#include "PajeObject.h"

class PajeColor : public PajeObject
{
public:
  float r;
  float g;
  float b;
  float a;
  PajeColor (float r, float g, float b, float a);
};

#endif
