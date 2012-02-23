#include "PajeFileReader.h"
#include <sys/time.h>

double gettime ()
{
  struct timeval tr;
  gettimeofday(&tr, NULL);
  return (double)tr.tv_sec+(double)tr.tv_usec/1000000;
}

PajeFileReader::PajeFileReader (std::string f, PajeTraceController *c)
{
  chunkSize = PAJE_DEFAULT_CHUNK_SIZE;
  moreData = true;
  currentChunk = 0;
  filename = f;
  file.open (filename.c_str());
  // std::cout << filename << std::endl;
  // std::cout << chunkSize << std::endl;
  // file.open (filename.c_str());//, std::ifstream::in);
  // while (!file.eof()){
  //   std::string line;
  //   std::getline (file, line);
  //   std::cout << line << std::endl;
  // }
}


void PajeFileReader::startChunk (int chunkNumber)
{
  if (chunkNumber != currentChunk){
    if (chunkNumber >= chunkInfo.size()){
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }

    std::streampos position = chunkInfo.find (chunkNumber)->second;
    file.seekg (position);
    currentChunk = chunkNumber;
    moreData = true;
  }else{
    if (chunkInfo.size() == 0){
      std::streampos position = file.tellg();
      chunkInfo.insert (std::pair<int,std::streampos>(chunkNumber, position));
    }
  }
  PajeComponent::startChunk (chunkNumber);
}

void PajeFileReader::endOfChunkLast (bool last)
{
  currentChunk++;
  if (!last){
    if (currentChunk == chunkInfo.size()){
      std::streampos position = file.tellg();
      chunkInfo.insert (std::pair<int,std::streampos>(currentChunk,position));
    }
  }
  PajeComponent::endOfChunkLast (last);
}

bool PajeFileReader::canEndChunk (void)
{
  if (!this->hasMoreData()){
    //file reached end, so we can end current chunk
    return true;
  }

  std::streampos offsetInFile = file.tellg();
  char *buffer = new char [PAJE_DEFAULT_LINE_SIZE];
  file.read (buffer, PAJE_DEFAULT_LINE_SIZE);
  if (file.eof()){
    file.close();
    file.open (filename.c_str());
  }
  std::streamsize length = file.gcount ();
  std::streamsize line_size = std::streamsize(PAJE_DEFAULT_LINE_SIZE);
  if (length < line_size){
    moreData = false;
  }

  //align data based on EOL
  char *bytes = buffer;
  char *eol;
  eol = (char*)memchr (bytes, '\n', length);
  if (eol != NULL){
    std::streamsize newlength = std::streamsize (eol - bytes + 1);
    if (newlength != length){
      length = newlength;
      moreData = true;
      file.seekg (offsetInFile + length);
      buffer[length] = '\0';
    }
  }

  if (length > 0){
    if (PajeComponent::canEndChunkBefore(buffer)){
      file.seekg(offsetInFile);
//      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
//      exit(1);
      return true;
    }else{
      file.seekg(offsetInFile+length);
      return false;
    }
  }else{
    return true;
  }
}

void PajeFileReader::readNextChunk (void)
{
  if (!moreData) return;

  int nextChunk = currentChunk + 1;
  if (nextChunk < chunkInfo.size()){
    // this chunk has already been read, we should know its size
    std::streampos nextChunkPosition = chunkInfo.find (nextChunk)->second;
    std::streampos chunkSize = nextChunkPosition - file.tellg();
    char *buffer = new char [chunkSize];
    file.read (buffer, chunkSize);
    std::streamsize length = file.gcount();
    if (length != chunkSize){
      fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
      exit(1);
    }
    PajeComponent::outputEntity (buffer);
    fprintf (stderr, "%s %d TODO\n", __FILE__, __LINE__);
    exit(1);
  }else{
    // first time reading this chunk.
    // must determine its correct size (must end in a line boundary
    // and in a date-changing event).
    // need to create a NSMutableData from a NSData
    char *buffer = new char [chunkSize];
    double t1 = gettime();
    file.read (buffer, chunkSize);
    double t2 = gettime();
    fprintf (stderr, "%f\n", t2-t1);
    std::streamsize length = file.gcount ();
    if (length < chunkSize){
      moreData = false;
    }else{
      char *bytes;
      int i;
      int offset = 0;
      bytes = buffer;
      for (i = length-1; i >= 0 && bytes[i] != '\n'; i--) {
        offset++;
      }
      if ((i >= 0) && (offset > 0)) {
        file.seekg (file.tellg() - std::streampos(offset));
        length = length - std::streampos(offset);
        buffer[length] = '\0';
      }
    }
    if (length > 0){
      PajeComponent::outputEntity (buffer);
      while (!this->canEndChunk());
    }
  }
}

bool PajeFileReader::hasMoreData (void)
{
  return moreData;
}

void PajeFileReader::setUserChunkSize (std::streamsize userChunkSize)
{
  chunkSize = userChunkSize;
}
