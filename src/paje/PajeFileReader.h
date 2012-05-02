#ifndef __PAJEFILEREADER_H__
#define __PAJEFILEREADER_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeComponent.h"

#define PAJE_DEFAULT_CHUNK_SIZE 1500000
#define PAJE_DEFAULT_LINE_SIZE 512

class PajeFileReader : public PajeComponent {
private:
  std::string filename;
  std::ifstream file;
  PajeTraceController *controller;
  std::streamsize chunkSize;
  int currentChunk;
  bool moreData;
  std::map<int,std::streampos> chunkInfo;
  unsigned long long length;
  unsigned long long current;

private:
  bool canEndChunk (void);

public:
  PajeFileReader (std::string f, PajeTraceController *c);
  ~PajeFileReader (void);
  void readNextChunk (void);
  void startChunk (int chunkNumber);
  void endOfChunkLast (bool last);
  bool hasMoreData (void);
  void setUserChunkSize (std::streamsize userChunkSize);
  unsigned long long traceSize (void);
  unsigned long long traceRead (void);
};
#endif
