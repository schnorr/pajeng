#ifndef __PAJECOMPONENT_H__
#define __PAJECOMPONENT_H__
#include <iostream>

class PajeComponent 
{
private:
  PajeComponent *outputComponent;
  PajeComponent *inputComponent;

public:
  PajeComponent (void);

  void setInputComponent (PajeComponent *component);
  void setOutputComponent (PajeComponent *component);

  void virtual inputEntity (void *data);
  void virtual outputEntity (void *data);
  bool virtual canEndChunkBefore (void *data);
              
  void virtual startChunk (int chunkNumber); //a new chunk is starting
  void virtual endOfChunkLast (bool last);   //the current chunk is ending
};

#endif
