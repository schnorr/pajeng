#ifndef __PAJEEVENTDECODER_H__
#define __PAJEEVENTDECODER_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeComponent.h"

class PajeEventDecoder : public PajeComponent {

public:
  PajeEventDecoder (void);

  void inputEntity (void *data);
  bool canEndChunkBefore (void *data);
      
  void startChunk (int chunkNumber);
  void endOfChunkLast (bool last);
};
#endif
