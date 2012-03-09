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

  // file.open (filename.c_str());//, std::ifstream::in);
  // while (!file.eof()){
  //   std::string line;
  //   std::getline (file, line);
  //   std::cout << line << std::endl;
  // }
}

PajeFileReader::~PajeFileReader (void)
{
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
  PajeData *buffer = new PajeData(chunkSize);
  file.read (buffer->bytes, chunkSize);
  if (file.eof()){
    file.close();
    file.open (filename.c_str());
  }
  std::streamsize length = file.gcount ();
  buffer->length = length;
  std::streamsize line_size = std::streamsize(chunkSize);
  if (length < line_size){
    moreData = false;
  }

  //align data based on EOL
  char *bytes = buffer->bytes;
  char *eol;
  eol = (char*)memchr (bytes, '\n', length);
  if (eol != NULL){
    std::streamsize newlength = std::streamsize (eol - bytes + 1);
    if (newlength != length){
      length = newlength;
      moreData = true;
      file.seekg (offsetInFile + length);
      buffer->bytes[length] = '\0';
      buffer->length = length;
    }
  }

  bool ret = true;
  if (length > 0){
    if (PajeComponent::canEndChunkBefore(buffer)){
      file.seekg(offsetInFile);
      ret = true;
    }else{
      file.seekg(offsetInFile+length);
      ret = false;
    }
  }else{
    ret = true;
  }
  delete buffer;
  return ret;
}

void PajeFileReader::readNextChunk (void)
{
  if (!moreData) return;

  int nextChunk = currentChunk + 1;
  if (nextChunk < chunkInfo.size()){
    // this chunk has already been read, we should know its size
    std::streampos nextChunkPosition = chunkInfo.find (nextChunk)->second;
    std::streampos chunkSize = nextChunkPosition - file.tellg();
    PajeData *buffer = new PajeData (chunkSize);
    file.read (buffer->bytes, chunkSize);
    std::streamsize length = file.gcount();
    buffer->length = length;
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
    PajeData *buffer = new PajeData(chunkSize);
    file.read (buffer->bytes, chunkSize);
    std::streamsize length = file.gcount ();
    buffer->length = length;
    if (length < chunkSize){
      moreData = false;
    }else{
      char *bytes = buffer->bytes;
      int i;
      int offset = 0;
      for (i = length-1; i >= 0 && bytes[i] != '\n'; i--) {
        offset++;
      }
      if ((i >= 0) && (offset > 0)) {
        file.seekg (file.tellg() - std::streampos(offset));
        length = length - std::streampos(offset);
        buffer->bytes[length] = '\0';
        buffer->length = length;
      }
    }

    if (length > 0){
      PajeComponent::outputEntity (buffer);
    }
    delete buffer;
    while (!this->canEndChunk());
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
