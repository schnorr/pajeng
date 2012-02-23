#ifndef __PAJEDATA_H__
#define __PAJEDATA_H__
#include "PajeObject.h"

class PajeData : public PajeObject {
public:
  char *bytes;
  int length;

  PajeData (int capacity);
};

#endif
