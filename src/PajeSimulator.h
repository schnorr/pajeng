#ifndef __PAJESIMULATOR_H__
#define __PAJESIMULATOR_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeComponent.h"

class PajeSimulator : public PajeComponent {
public:
  PajeSimulator();
  
  void inputEntity (PajeObject *data);
  bool canEndChunkBefore (PajeObject *data);
      
  void startChunk (int chunkNumber);
  void endOfChunkLast (bool last);
};
#endif
